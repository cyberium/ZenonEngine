#include "stdafx.h"

// General
#include "Loader.h"

CLoader::CLoader()
{

}

CLoader::~CLoader()
{
	Stop();
}

void CLoader::Start()
{
#ifdef LOADER_ENABLED
	for (int i = 0; i < c_PoolSize; i++)
	{
		std::future<void> futureObj = m_Thread_Loader_Promise_Exiter[i].get_future();
		m_Thread_Loader[i] = std::thread(&CLoader::LoaderThread, this, std::move(futureObj));
		//m_Thread_Loader[i].detach();
	}

#ifdef DELETER_ENABLED
	std::future<void> futureObj = m_Thread_Deleter_Promise.get_future();
	m_Thread_Deleter = std::thread(&CLoader::DeleterThread, this, std::move(futureObj));
#endif

#ifdef SORTER_ENABLED
	std::future<void> futureObj = m_Thread_Sorter_Promise.get_future();
	m_Thread_Sorter = std::thread(&CLoader::SorterThread, this, std::move(futureObj));
#endif
#endif
}

void CLoader::Stop()
{
#ifdef LOADER_ENABLED
	for (int i = 0; i < c_PoolSize; i++)
	{
		m_Thread_Loader_Promise_Exiter[i].set_value();
		_ASSERT(m_Thread_Loader[i].joinable());
		m_Thread_Loader[i].join();
	}
	m_QueueLoad.Clear();

#ifdef DELETER_ENABLED
	m_Thread_Deleter_Promise.set_value();
	_ASSERT(m_Thread_Deleter.joinable());
	m_Thread_Deleter.join();
	m_QueueDelete.Clear();
#endif

#ifdef SORTER_ENABLED
	m_Thread_Sorter_Promise.set_value();
	if (m_Thread_Sorter.joinable())
		m_Thread_Sorter.join();
#endif
#endif
}

void CLoader::SetCamera(std::shared_ptr<ICameraComponent3D> _camera)
{
	m_Camera = _camera;
}

void CLoader::AddToLoadQueue(const std::weak_ptr<ILoadable>& LoadableItemWPtr)
{
#ifdef LOADER_ENABLED
	m_QueueLoad.Add(LoadableItemWPtr);
#else
	auto loadableItem = LoadableItemWPtr.lock();
	loadableItem->Load();
	loadableItem->SetState(ILoadable::ELoadableState::Loaded);
#endif
}

void CLoader::AddToDeleteQueue(const std::shared_ptr<ILoadable>& LoadableItemWPtr)
{
	m_QueueDelete.Add(LoadableItemWPtr);
}


#ifdef LOADER_ENABLED

void CLoader::LoaderThread(std::future<void> _promiseExiter)
{
	while (_promiseExiter.wait_for(std::chrono::milliseconds(0)) == std::future_status::timeout)
	{
		if (m_QueueLoad.IsEmpty())
		{
			std::this_thread::sleep_for(std::chrono::milliseconds(20));
			continue;
		}

		std::shared_ptr<ILoadable> objectToLoad = nullptr;
		if (!m_QueueLoad.GetNextItem(&objectToLoad))
			continue;

		objectToLoad->SetState(ILoadable::ELoadableState::Loading);
		objectToLoad->Load();
		objectToLoad->SetState(ILoadable::ELoadableState::Loaded);
	}

	Log::Green("Loader thread stopped.");
}

#ifdef DELETER_ENABLED
void CLoader::DeleterThread(std::future<void> _promiseExiter)
{
	while (_promiseExiter.wait_for(std::chrono::milliseconds(0)) == std::future_status::timeout)
	{
		if (m_QueueDelete.IsEmpty())
		{
			std::this_thread::sleep_for(std::chrono::milliseconds(20));
			continue;
		}

		m_QueueDelete.DeleteItem();
	}
}
#endif

#ifdef SORTER_ENABLED
void CLoader::SorterThread(std::future<void> _promiseExiter)
{
	while (_promiseExiter.wait_for(std::chrono::milliseconds(0)) == std::future_status::timeout)
	{
		if (m_QueueLoad.empty())
		{
			std::this_thread::sleep_for(std::chrono::milliseconds(20));
			continue;
		}

		m_QueueLoad.lock();
		m_QueueLoad.getList().sort(sortFunctor(m_Camera));
		m_QueueLoad.unlock();

		std::this_thread::sleep_for(std::chrono::milliseconds(1000));
	}
}

bool CLoader::sortFunctor::operator()(const ILoadable* first, const ILoadable* second)
{
	const SceneNode3D* node3DFirst = dynamic_cast<const SceneNode3D*>(first);
	if (node3DFirst == nullptr)
		return false;

	const SceneNode3D* node3DSecond = dynamic_cast<const SceneNode3D*>(second);
	if (node3DSecond == nullptr)
		return true;

	if (camera)
	{
		bool cullFirst = !camera->GetFrustum().cullSphere(node3DFirst->GetTranslation(), 15.0f);
		bool cullSecond = !camera->GetFrustum().cullSphere(node3DSecond->GetTranslation(), 15.0f);

		if (cullFirst && !cullSecond)
			return true;

		if (!cullFirst && cullSecond)
			return false;

		if (!cullFirst && !cullSecond)
			return false;
	}

	if (first->getPriority() == second->getPriority())
	{
		if (camera)
		{
			float distToCamera2DFirst = glm::length(Fix_X0Z(camera->GetTranslation()) - node3DFirst->GetTranslation());
			float distToCamera2DSecond = glm::length(Fix_X0Z(camera->GetTranslation()) - node3DSecond->GetTranslation());
			return distToCamera2DFirst < distToCamera2DSecond;
		}
	}

	return first->getPriority() < second->getPriority();
}
#endif

#endif