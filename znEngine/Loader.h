#pragma once

#define LOADER_ENABLED
//#define SORTER_ENABLED



class ZN_API CLoaderQueue
{
public:
	inline void Add(ILoadable* item)
	{
		std::lock_guard<std::mutex> lock(m_Lock);

		m_Queue.push_back(item);
	}

	inline bool GetNextItem(ILoadable ** result)
	{
		std::lock_guard<std::mutex> lock(m_Lock);

		_ASSERT(result != NULL);
		_ASSERT(*result == NULL);

		if (m_Queue.empty())
		{
			return false;
		}

		ILoadable* loadable = m_Queue.front();
		if (loadable == NULL)
		{
			m_Queue.pop_front();
			return false;
		}

		if (loadable->GetState() == ILoadable::ELoadableState::Loading || loadable->GetState() == ILoadable::ELoadableState::Loaded)
		{
			m_Queue.pop_front();
			return false;
		}

		if (loadable->getDepends() != nullptr && loadable->getDepends()->GetState() != ILoadable::ELoadableState::Loaded)
		{
			m_Queue.pop_front();
			m_Queue.push_back(loadable);
			return false;
		}

		m_Queue.pop_front();
		
		(*result) = loadable;
		return true;
	}

	inline bool IsEmpty()
	{
		std::lock_guard<std::mutex> lock(m_Lock);

		return m_Queue.empty();
	}

private:
	std::mutex m_Lock;
	std::list<ILoadable*> m_Queue;
};



class ZN_API CLoader 
	: public ILoader
{
public:
	CLoader();
	virtual ~CLoader();

	void Start() override;
	void Stop() override;

	void AddToLoadQueue(ILoadable* _item) override;
	void LoadAll() override;

	void AddToDeleteQueue(ILoadable* _item) override;
	void DeleteAll() override;

	void SetCamera(std::shared_ptr<ICameraComponent3D> _camera);

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
	const static uint32                    c_PoolSize = 4;

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