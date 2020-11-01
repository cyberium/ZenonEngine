#include "stdafx.h"

// General
#include "SceneBase.h"

SceneBase::SceneBase(IBaseManager& BaseManager, IRenderWindow& RenderWindow)
	: m_BaseManager(BaseManager)
	, m_RenderDevice(BaseManager.GetApplication().GetRenderDevice())
	, m_RenderWindow(RenderWindow)
	, m_IsFreezed(false)
	, m_Finder(std::make_unique<CSceneFinder>(*this))
{
	
}

SceneBase::~SceneBase()
{}




//
// IScene
//
IBaseManager& SceneBase::GetBaseManager() const
{
	return m_BaseManager;
}

IRenderDevice& SceneBase::GetRenderDevice() const
{
	return m_RenderDevice;
}

IRenderWindow& SceneBase::GetRenderWindow() const
{
	return m_RenderWindow;
}

void SceneBase::AddEventListener(std::shared_ptr<ISceneEventsListener> Listener)
{
	const auto& it = std::find(m_EventListeners.begin(), m_EventListeners.end(), Listener);
	if (it != m_EventListeners.end())
	{
		Log::Error("Scene: Unable to add 'ISceneEventsListener' because already exists.");
		return;
	}
	m_EventListeners.push_back(Listener);
}

void SceneBase::RemoveEventListener(std::shared_ptr<ISceneEventsListener> Listener)
{
	const auto& it = std::find(m_EventListeners.begin(), m_EventListeners.end(), Listener);
	if (it == m_EventListeners.end())
	{
		Log::Error("Scene: Unable to remove 'ISceneEventsListener' because non exists.");
		return;
	}
	m_EventListeners.erase(it);
}





void SceneBase::Initialize()
{
	m_VideoSettings = GetBaseManager().GetManager<ISettings>()->GetGroup("Video");

	m_FrameQuery = GetRenderDevice().GetObjectsFactory().CreateQuery(IQuery::QueryType::Timer, 1);
	m_TestQuery = GetRenderDevice().GetObjectsFactory().CreateQuery(IQuery::QueryType::CountSamples, 1);

	m_RootNode3D = GetBaseManager().GetManager<IObjectsFactory>()->GetClassFactoryCast<ISceneNode3DFactory>()->CreateSceneNode3D(cSceneNode3D, this);
	m_RootNode3D->SetName("RootNode3D");
	std::dynamic_pointer_cast<ISceneNode3DInternal>(m_RootNode3D)->SetPersistanceInternal(true);

	m_RootNodeUI = GetBaseManager().GetManager<IObjectsFactory>()->GetClassFactoryCast<ISceneNodeUIFactory>()->CreateSceneNodeUI(this, cSceneNodeUI);
	m_RootNodeUI->SetName("RootNodeUI");

	{
		m_CameraPosText = GetBaseManager().GetManager<IObjectsFactory>()->GetClassFactoryCast<ISceneNodeUIFactory>()->CreateSceneNodeUI(this, cSceneNodeUI_Text);
		GetRootNodeUI()->AddChild(m_CameraPosText);
		m_CameraPosText->SetTranslate(glm::vec2(5.0f, 5.0f));

		m_CameraRotText = GetBaseManager().GetManager<IObjectsFactory>()->GetClassFactoryCast<ISceneNodeUIFactory>()->CreateSceneNodeUI(this, cSceneNodeUI_Text);
		GetRootNodeUI()->AddChild(m_CameraRotText);
		m_CameraRotText->SetTranslate(glm::vec2(5.0f, 25.0f));

		m_CameraRot2Text = GetBaseManager().GetManager<IObjectsFactory>()->GetClassFactoryCast<ISceneNodeUIFactory>()->CreateSceneNodeUI(this, cSceneNodeUI_Text);
		GetRootNodeUI()->AddChild(m_CameraRot2Text);
		m_CameraRot2Text->SetTranslate(glm::vec2(5.0f, 45.0f));

		m_FPSText = GetBaseManager().GetManager<IObjectsFactory>()->GetClassFactoryCast<ISceneNodeUIFactory>()->CreateSceneNodeUI(this, cSceneNodeUI_Text);
		GetRootNodeUI()->AddChild(m_FPSText);
		m_FPSText->SetTranslate(glm::vec2(5.0f, 65.0f));

		m_StatisticUpdateText = GetBaseManager().GetManager<IObjectsFactory>()->GetClassFactoryCast<ISceneNodeUIFactory>()->CreateSceneNodeUI(this, cSceneNodeUI_Text);
		GetRootNodeUI()->AddChild(m_StatisticUpdateText);
		m_StatisticUpdateText->SetTranslate(glm::vec2(5.0f, 85.0f));
		
		m_StatisticPreRenderText = GetBaseManager().GetManager<IObjectsFactory>()->GetClassFactoryCast<ISceneNodeUIFactory>()->CreateSceneNodeUI(this, cSceneNodeUI_Text);
		GetRootNodeUI()->AddChild(m_StatisticPreRenderText);
		m_StatisticPreRenderText->SetTranslate(glm::vec2(5.0f, 105.0f));
		
		m_StatisticRenderText = GetBaseManager().GetManager<IObjectsFactory>()->GetClassFactoryCast<ISceneNodeUIFactory>()->CreateSceneNodeUI(this, cSceneNodeUI_Text);
		GetRootNodeUI()->AddChild(m_StatisticRenderText);
		m_StatisticRenderText->SetTranslate(glm::vec2(5.0f, 125.0f));
		
		m_StatisticPostRenderText = GetBaseManager().GetManager<IObjectsFactory>()->GetClassFactoryCast<ISceneNodeUIFactory>()->CreateSceneNodeUI(this, cSceneNodeUI_Text);
		GetRootNodeUI()->AddChild(m_StatisticPostRenderText);
		m_StatisticPostRenderText->SetTranslate(glm::vec2(5.0f, 145.0f));
		
		m_StatisticRenderUIText = GetBaseManager().GetManager<IObjectsFactory>()->GetClassFactoryCast<ISceneNodeUIFactory>()->CreateSceneNodeUI(this, cSceneNodeUI_Text);
		GetRootNodeUI()->AddChild(m_StatisticRenderUIText);
		m_StatisticRenderUIText->SetTranslate(glm::vec2(5.0f, 165.0f));

		m_StatisticSummaText = GetBaseManager().GetManager<IObjectsFactory>()->GetClassFactoryCast<ISceneNodeUIFactory>()->CreateSceneNodeUI(this, cSceneNodeUI_Text);
		GetRootNodeUI()->AddChild(m_StatisticSummaText);
		m_StatisticSummaText->SetTranslate(glm::vec2(5.0f, 185.0f));
	}


}

void SceneBase::Finalize()
{
}






//
// IScene
//
std::shared_ptr<ISceneNode3D> SceneBase::GetRootNode3D() const
{
	return m_RootNode3D;
}

std::shared_ptr<ISceneNodeUI> SceneBase::GetRootNodeUI() const
{
	return m_RootNodeUI;
}

void SceneBase::SetRenderer(std::shared_ptr<IRenderer> Renderer)
{
	m_Renderer = Renderer;
}

std::shared_ptr<IRenderer> SceneBase::GetRenderer() const
{
	return m_Renderer;
}

void SceneBase::SetCameraController(std::shared_ptr<ICameraController> CameraController)
{
	_ASSERT(CameraController != nullptr);
	m_CameraController = CameraController;
}

std::shared_ptr<ICameraController> SceneBase::GetCameraController() const
{
	return m_CameraController;
}

const ISceneFinder& SceneBase::GetFinder() const
{
	return *m_Finder.get();
}




void SceneBase::Accept(IVisitor * visitor)
{
	std::lock_guard<std::mutex> lock(m_SceneIsBusy);

	if (GetRootNode3D())
		GetRootNode3D()->Accept(visitor);

	if (GetRootNodeUI())
		GetRootNodeUI()->Accept(visitor);
}

void SceneBase::Freeze()
{
	//std::lock_guard<std::mutex> lock(m_ListsAreBusy);

	m_IsFreezed = true;
}

void SceneBase::Unfreeze()
{
	//std::lock_guard<std::mutex> lock(m_ListsAreBusy);

	m_IsFreezed = false;
}

void SceneBase::AddChild(const std::shared_ptr<ISceneNode3D>& ParentNode, const std::shared_ptr<ISceneNode3D>& ChildNode)
{
	if (ParentNode == nullptr)
	{
		Log::Warn("Can't add child instanse to nullptr parent.");
		return;
	}

	if (m_IsFreezed || m_SceneIsBusy.try_lock() == false)
	{
		std::lock_guard<std::mutex> lock(m_ListsAreBusy);
		m_AddChildList.push_back(std::make_pair(ParentNode, ChildNode));
	}
	else
	{
		try
		{
			ParentNode->AddChild(ChildNode);
		}
		catch (...)
		{
			m_SceneIsBusy.unlock();
			throw;
		}

		m_SceneIsBusy.unlock();
	}
}

void SceneBase::RemoveChild(const std::shared_ptr<ISceneNode3D>& ParentNode, const std::shared_ptr<ISceneNode3D>& ChildNode)
{
	if (ParentNode == nullptr)
	{
		Log::Warn("Can't remove child instanse from nullptr parent.");
		return;
	}

	if (m_IsFreezed || m_SceneIsBusy.try_lock() == false)
	{
		std::lock_guard<std::mutex> lock(m_ListsAreBusy);
		m_RemoveChildList.push_back(std::make_pair(ParentNode, ChildNode));
	}
	else
	{
		try
		{
			ParentNode->RemoveChild(ChildNode);
		}
		catch (...)
		{
			m_SceneIsBusy.unlock();
			throw;
		}

		m_SceneIsBusy.unlock();
	}
}



//
// ISceneInternal
//
void SceneBase::RaiseSceneChangeEvent(ESceneChangeType SceneChangeType, const std::shared_ptr<ISceneNode3D>& OwnerNode, const std::shared_ptr<ISceneNode3D>& ChildNode)
{
	//m_SceneChangeEvent(SceneChangeEventArgs(this, SceneChangeType, OwnerNode, ChildNode));
}



//
// IRenderWindowEventListener
//
void SceneBase::OnUpdate(UpdateEventArgs& e)
{
	if (GetCameraController())
	{
		GetCameraController()->OnUpdate(e);

		e.Camera           = GetCameraController()->GetCamera().get();
		e.CameraForCulling = GetCameraController()->GetCamera().get();
	}

	std::lock_guard<std::mutex> lock(m_SceneIsBusy);

	if (!m_IsFreezed)
	{
		std::lock_guard<std::mutex> lock(m_ListsAreBusy);

		for (const auto& it : m_AddChildList)
			it.first->AddChild(it.second);
		m_AddChildList.clear();

		for (const auto& it : m_RemoveChildList)
			it.first->RemoveChild(it.second);
		m_RemoveChildList.clear();
	}

	if (GetRootNode3D())
		DoUpdate_Rec(GetRootNode3D(), e);
}

void SceneBase::OnPreRender(RenderEventArgs & e)
{
	m_Start = std::chrono::high_resolution_clock::now();
}

void SceneBase::OnRender(RenderEventArgs & e)
{
	if (auto cameraController = GetCameraController())
	{
		e.Camera = cameraController->GetCamera().get();
		e.CameraForCulling = cameraController->GetCamera().get();
	}
	else
		_ASSERT(false);

	if (auto renderer = GetRenderer())
		renderer->Render3D(e);
}

void SceneBase::OnPostRender(RenderEventArgs & e)
{
	if (auto cameraController = GetCameraController())
	{
		glm::vec3 cameraTrans = cameraController->GetCamera()->GetTranslation();
		m_CameraPosText->GetProperties()->GetPropertyT<std::string>("Text")->Set("Pos: x = " + std::to_string(cameraTrans.x) + ", y = " + std::to_string(cameraTrans.y) + ", z = " + std::to_string(cameraTrans.z));
		m_CameraRotText->GetProperties()->GetPropertyT<std::string>("Text")->Set("Rot: yaw = " + std::to_string(cameraController->GetCamera()->GetYaw()) + ", pitch = " + std::to_string(cameraController->GetCamera()->GetPitch()));
		m_CameraRot2Text->GetProperties()->GetPropertyT<std::string>("Text")->Set("Rot: [" + std::to_string(cameraController->GetCamera()->GetDirection().x) + ", " + std::to_string(cameraController->GetCamera()->GetDirection().y) + ", " + std::to_string(GetCameraController()->GetCamera()->GetDirection().z) + "].");
	}

	e.Camera = nullptr;
	e.CameraForCulling = nullptr;
}

void SceneBase::OnRenderUI(RenderEventArgs & e)
{
	m_End = std::chrono::high_resolution_clock::now();

	if (auto cameraController = GetCameraController())
	{
		e.Camera = cameraController->GetCamera().get();
		e.CameraForCulling = cameraController->GetCamera().get();
	}

	if (auto renderer = GetRenderer())
		renderer->RenderUI(e);

	{
		/*IQuery::QueryResult frameResult = m_FrameQuery->GetQueryResult(e.FrameCounter);
		if (frameResult.IsValid)
		{
			if (GetRenderDevice()->GetDeviceType() == RenderDeviceType::RenderDeviceType_DirectX)
				m_FrameTime = frameResult.DeltaTime * 1000.0;
			else
				m_FrameTime = frameResult.DeltaTime / 1000000.0;

			double fpsValue = 1000.0f / m_FrameTime;

			m_FPSText->GetProperties()->GetPropertyT<std::string>("Text")->Set("FPS: " + std::to_string(uint64(fpsValue)));
		}*/

		//int elapsed_microseconds = std::chrono::duration_cast<std::chrono::microseconds>(m_End - m_Start).count();
		double fpsValue = 1000.0 / double(e.DeltaTime);
		m_FPSText->GetProperties()->GetPropertyT<std::string>("Text")->Set("FPS: " + std::to_string(uint64(fpsValue)));


		m_StatisticUpdateText->GetProperties()    ->GetPropertyT<std::string>("Text")->Set("W Update   : " + std::to_string(GetRenderWindow().GetUpdateDeltaTime()));
		m_StatisticPreRenderText->GetProperties() ->GetPropertyT<std::string>("Text")->Set("W PreRender: " + std::to_string(GetRenderWindow().GetPreRenderDeltaTime()));
		m_StatisticRenderText->GetProperties()    ->GetPropertyT<std::string>("Text")->Set("W Render   : " + std::to_string(GetRenderWindow().GetRenderDeltaTime()));
		m_StatisticPostRenderText->GetProperties()->GetPropertyT<std::string>("Text")->Set("W PostRen  : " + std::to_string(GetRenderWindow().GetPostRenderDeltaTime()));
		m_StatisticRenderUIText->GetProperties()  ->GetPropertyT<std::string>("Text")->Set("W RenderUI : " + std::to_string(GetRenderWindow().GetRenderUIDeltaTime()));
		m_StatisticSummaText->GetProperties()     ->GetPropertyT<std::string>("Text")->Set("W          : " + std::to_string(GetRenderWindow().GetSummaDeltaTime()));
	}
}



//
// INativeWindowEventListener
//
void SceneBase::OnWindowResize(ResizeEventArgs & e)
{
	if (GetCameraController())
		GetCameraController()->OnResize(e);

	if (m_Renderer)
		m_Renderer->Resize(e.Width, e.Height);
}

// Keyboard events

bool SceneBase::OnWindowKeyPressed(KeyEventArgs & e)
{
	if (GetCameraController())
		GetCameraController()->OnKeyPressed(e);

	if (GetRootNodeUI())
		return DoKeyPressed_Rec(GetRootNodeUI(), e);

	return false;
}

void SceneBase::OnWindowKeyReleased(KeyEventArgs & e)
{
	if (GetCameraController())
		GetCameraController()->OnKeyReleased(e);

	if (GetRootNodeUI())
		DoKeyReleased_Rec(GetRootNodeUI(), e);
}

// Mouse events

void SceneBase::OnWindowMouseMoved(MouseMotionEventArgs & e)
{
	OnMouseMoved(e, GetCameraController()->ScreenToRay(GetRenderWindow().GetViewport(), e.GetPoint()));

	if (GetCameraController() == nullptr)
		throw CException("You must set CameraController to scene!");
	GetCameraController()->OnMouseMoved(e);

	if (GetRootNodeUI())
		DoMouseMoved_Rec(GetRootNodeUI(), e);
}

bool SceneBase::OnWindowMouseButtonPressed(MouseButtonEventArgs & e)
{	
	if (OnMousePressed(e, GetCameraController()->ScreenToRay(GetRenderWindow().GetViewport(), e.GetPoint())))
		return true;

	if (GetCameraController() == nullptr)
		throw CException("You must set CameraController to scene!");
	GetCameraController()->OnMouseButtonPressed(e);

	if (GetRootNodeUI())
		return DoMouseButtonPressed_Rec(GetRootNodeUI(), e);

	return false;
}

void SceneBase::OnWindowMouseButtonReleased(MouseButtonEventArgs & e)
{
	OnMouseReleased(e, GetCameraController()->ScreenToRay(GetRenderWindow().GetViewport(), e.GetPoint()));

	if (GetCameraController() == nullptr)
		throw CException("You must set CameraController to scene!");
	GetCameraController()->OnMouseButtonReleased(e);

	if (GetRootNodeUI())
		DoMouseButtonReleased_Rec(GetRootNodeUI(), e);
}

bool SceneBase::OnWindowMouseWheel(MouseWheelEventArgs & e)
{
	if (GetCameraController())
		if (GetCameraController()->OnMouseWheel(e))
			return true;

	if (GetRootNodeUI())
		if (DoMouseWheel_Rec(GetRootNodeUI(), e))
			return true;

	return false;
}

// Mouse in world events

bool SceneBase::OnMousePressed(const MouseButtonEventArgs & e, const Ray& RayToWorld)
{
	return false;
}

void SceneBase::OnMouseReleased(const MouseButtonEventArgs & e, const Ray & RayToWorld)
{
}

void SceneBase::OnMouseMoved(const MouseMotionEventArgs & e, const Ray& RayToWorld)
{

}



//
// IObjectSaveLoad
//
void SceneBase::Load(const std::shared_ptr<IXMLReader>& Reader)
{
	_ASSERT(FALSE);
}

void SceneBase::Save(const std::shared_ptr<IXMLWriter>& Writer) const
{
	_ASSERT(FALSE);
}



//
// Protected
//





void SceneBase::DoUpdate_Rec(const std::shared_ptr<ISceneNode3D>& Node, const UpdateEventArgs & e)
{
	Node->Update(e);

	const auto& components = Node->GetComponents();
	std::for_each(components.begin(), components.end(), [&e](const std::pair<ObjectClass, std::shared_ptr<ISceneNodeComponent>>& Component)
	{
		_ASSERT(Component.second);
		Component.second->Update(e);
	});

	const auto& childs = Node->GetChilds();
	std::for_each(childs.begin(), childs.end(), [this, &e](const std::shared_ptr<ISceneNode3D>& Child)
	{
		if (Child != nullptr)
			DoUpdate_Rec(Child, e);
	});
}

//
// Input events process recursive
//
bool SceneBase::DoKeyPressed_Rec(const std::shared_ptr<ISceneNodeUI>& Node, KeyEventArgs & e)
{
	std::shared_ptr<SceneNodeUI> NodeAsUINode = std::dynamic_pointer_cast<SceneNodeUI>(Node);
	if (NodeAsUINode != nullptr)
	{
		for (auto child : NodeAsUINode->GetChilds())
			if (DoKeyPressed_Rec(child, e))
				return true;

		if (NodeAsUINode->OnKeyPressed(e))
			return true;
	}

	return false;
}

void SceneBase::DoKeyReleased_Rec(const std::shared_ptr<ISceneNodeUI>& Node, KeyEventArgs & e)
{
	std::shared_ptr<SceneNodeUI> NodeAsUINode = std::dynamic_pointer_cast<SceneNodeUI>(Node);
	if (NodeAsUINode != nullptr)
	{

		for (auto child : NodeAsUINode->GetChilds())
		{
			DoKeyReleased_Rec(child, e);
		}

		NodeAsUINode->OnKeyReleased(e);
	}
}

void SceneBase::DoMouseMoved_Rec(const std::shared_ptr<ISceneNodeUI>& Node, MouseMotionEventArgs & e)
{
	std::shared_ptr<SceneNodeUI> NodeAsUINode = std::dynamic_pointer_cast<SceneNodeUI>(Node);
	if (NodeAsUINode != nullptr)
	{
		for (auto child : NodeAsUINode->GetChilds())
		{
			DoMouseMoved_Rec(child, e);
		}

		NodeAsUINode->OnMouseMoved(e);

		// Synteric events impl
		if (NodeAsUINode->IsPointInBoundsAbs(e.GetPoint()))
		{
			if (!NodeAsUINode->IsMouseOnNode())
			{
				NodeAsUINode->OnMouseEntered();
				NodeAsUINode->DoMouseEntered();
			}
		}
		else
		{
			if (NodeAsUINode->IsMouseOnNode())
			{
				NodeAsUINode->OnMouseLeaved();
				NodeAsUINode->DoMouseLeaved();
			}
		}
	}
}

bool SceneBase::DoMouseButtonPressed_Rec(const std::shared_ptr<ISceneNodeUI>& Node, MouseButtonEventArgs & e)
{
	std::shared_ptr<SceneNodeUI> NodeAsUINode = std::dynamic_pointer_cast<SceneNodeUI>(Node);
	if (NodeAsUINode != nullptr)
	{
		for (auto child : NodeAsUINode->GetChilds())
			if (DoMouseButtonPressed_Rec(child, e))
				return true;

		if (NodeAsUINode->IsPointInBoundsAbs(e.GetPoint()))
			if (NodeAsUINode->OnMouseButtonPressed(e))
				return true;
	}

	return false;
}

void SceneBase::DoMouseButtonReleased_Rec(const std::shared_ptr<ISceneNodeUI>& Node, MouseButtonEventArgs & e)
{
	std::shared_ptr<SceneNodeUI> NodeAsUINode = std::dynamic_pointer_cast<SceneNodeUI>(Node);
	if (NodeAsUINode != nullptr)
	{
		for (auto child : NodeAsUINode->GetChilds())
			DoMouseButtonReleased_Rec(child, e);

		NodeAsUINode->OnMouseButtonReleased(e);
	}
}

bool SceneBase::DoMouseWheel_Rec(const std::shared_ptr<ISceneNodeUI>& Node, MouseWheelEventArgs & e)
{
	std::shared_ptr<SceneNodeUI> NodeAsUINode = std::dynamic_pointer_cast<SceneNodeUI>(Node);
	if (NodeAsUINode != nullptr)
	{

		for (auto child : NodeAsUINode->GetChilds())
		{
			if (DoMouseWheel_Rec(child, e))
				return true;
		}

		if (NodeAsUINode->IsPointInBoundsAbs(e.GetPoint()))
			if (NodeAsUINode->OnMouseWheel(e))
				return true;
	}

	return false;
}