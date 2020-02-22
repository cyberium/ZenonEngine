#pragma once

#define LOADER_ENABLED
//#define SORTER_ENABLED



class ZN_API CLoaderQueue
{
public:
	inline void Add(const std::weak_ptr<ILoadable>& item)
	{
		std::lock_guard<std::mutex> lock(m_Lock);

		m_Queue.push_back(item);
	}

	inline bool GetNextItem(std::shared_ptr<ILoadable> * Result)
	{
		_ASSERT(Result != NULL);
		_ASSERT(*Result == NULL);

		std::lock_guard<std::mutex> lock(m_Lock);

		if (m_Queue.empty())
			return false;

		auto loadableWPtr = m_Queue.front();
		m_Queue.pop_front();

		// Объект уже кто-то удалил, загружать не будем
		auto loadable = loadableWPtr.lock();
		if (loadable == nullptr)
			return false;

		// Загружать можно только созданные объекты
		if (loadable->GetState() != ILoadable::ELoadableState::Created)
			return false;

		if (loadable->getDepends() != nullptr && loadable->getDepends()->GetState() != ILoadable::ELoadableState::Loaded)
		{
			m_Queue.push_back(loadable);
			return false;
		}
		
		(*Result) = loadable;
		return true;
	}

	inline bool IsEmpty()
	{
		std::lock_guard<std::mutex> lock(m_Lock);

		return m_Queue.empty();
	}

private:
	std::mutex m_Lock;
	std::list<std::weak_ptr<ILoadable>> m_Queue;
};



class ZN_API CLoader 
	: public ILoader
{
public:
	CLoader();
	virtual ~CLoader();

	void Start() override;
	void Stop() override;

	void SetCamera(std::shared_ptr<ICameraComponent3D> _camera);

	void AddToLoadQueue(const std::weak_ptr<ILoadable>& LoadableItemWPtr) override;
	void AddToDeleteQueue(const std::weak_ptr<ILoadable>& LoadableItemWPtr) override;


#ifdef LOADER_ENABLED
	void LoaderThread(std::future<void> _promiseExiter);

#ifdef SORTER_ENABLED
	void SorterThread(std::future<void> futureObj);
#endif

#ifdef SORTER_ENABLED
	struct sortFunctor 
	{
		const std::shared_ptr<ICameraComponent3D>& camera;

		sortFunctor(const std::shared_ptr<ICameraComponent3D>& _camera)
			: camera(_camera)
		{ }

		bool operator()(const ILoadable* first, const ILoadable* second);
	};
#endif
#endif

private:
	const static uint32                    c_PoolSize = 8;

private:
	CLoaderQueue m_QueueLoad;
	CLoaderQueue m_QueueDelete;

#ifdef LOADER_ENABLED
	std::promise<void>					   m_Thread_Loader_Promise_Exiter[c_PoolSize];
	std::thread                            m_Thread_Loader[c_PoolSize];

#ifdef SORTER_ENABLED
	std::promise<void>					   m_Thread_Sorter_Promise;
	std::thread                            m_Thread_Sorter;
#endif
#endif

	std::shared_ptr<ICameraComponent3D>    m_Camera;
};