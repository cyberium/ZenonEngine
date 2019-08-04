#pragma once

//#define LOADER_ENABLED

#ifndef _MANAGED
#include <future>
#endif

class OW_ENGINE_API CLoader : public ILoader
{
public:
	CLoader();
	~CLoader();

	void AddToLoadQueue(std::shared_ptr<ILoadable> _item) override;
	void LoadAll() override;

	void AddToDeleteQueue(std::shared_ptr<ILoadable> _item) override;
	void DeleteAll() override;

	void SetCamera(std::shared_ptr<Camera> _camera);

#ifndef _MANAGED
	void LoaderThread(std::future<void> _promiseExiter);
	void SorterThread(std::future<void> futureObj);
#endif

	struct sortFunctor 
	{
		const std::shared_ptr<Camera>& camera;

		sortFunctor(const std::shared_ptr<Camera>& _camera)
			: camera(_camera)
		{ }

		bool operator()(const std::shared_ptr<ILoadable>& first, const std::shared_ptr<ILoadable>& second);
	};

private:
	const static uint32                    c_PoolSize = 8;

private:
	LockedQueue<std::shared_ptr<ILoadable>> m_QueueLoad;
	LockedQueue<std::shared_ptr<ILoadable>> m_QueueDelete;

#ifndef _MANAGED
	std::promise<void>					   m_Thread_Loader_Promise_Exiter[c_PoolSize];
	std::thread                            m_Thread_Loader[c_PoolSize];

	std::promise<void>					   m_Thread_Sorter_Promise;
	std::thread                            m_Thread_Sorter;
#endif

	std::shared_ptr<Camera>                m_Camera;
};