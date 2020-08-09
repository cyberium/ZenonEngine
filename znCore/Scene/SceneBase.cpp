#include "stdafx.h"

// General
#include "SceneBase.h"

// Additonal
#include "SceneNode3D.h"
#include "SceneNodeUI.h"

SceneBase::SceneBase(IBaseManager& BaseManager)
	: m_BaseManager(BaseManager)
	, m_RenderDevice(BaseManager.GetApplication().GetRenderDevice())
	, m_IsFreezed(false)
	, m_Finder(std::make_unique<CSceneFinder>(*this))
{
	
}

SceneBase::~SceneBase()
{}

void SceneBase::Initialize()
{
	m_VideoSettings = GetBaseManager().GetManager<ISettings>()->GetGroup("Video");

	m_FrameQuery = GetRenderDevice().GetObjectsFactory().CreateQuery(IQuery::QueryType::Timer, 1);
	m_TestQuery = GetRenderDevice().GetObjectsFactory().CreateQuery(IQuery::QueryType::CountSamples, 1);

	m_RootNode3D = GetBaseManager().GetManager<IObjectsFactory>()->GetClassFactoryCast<ISceneNode3DFactory>()->CreateSceneNode3D(cSceneNode3D, this);
	m_RootNode3D->SetName("RootNode3D");

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
	}


}

void SceneBase::Finalize()
{
}

void SceneBase::SetRenderWindow(const std::shared_ptr<IRenderWindow>& RenderWindow)
{
	m_RenderWindow = RenderWindow;
}

std::shared_ptr<IRenderWindow> SceneBase::GetRenderWindow() const
{
	std::shared_ptr<IRenderWindow> renderWindow = m_RenderWindow.lock();
	_ASSERT(renderWindow);
	return std::move(renderWindow);
}

void SceneBase::ConnectEvents(const std::shared_ptr<IRenderWindowEvents>& WindowEvents)
{
	// RenderWindowEvents
	m_OnUpdateConnection = WindowEvents->Update().connect(&SceneBase::OnUpdate, this, std::placeholders::_1);
	m_OnPreRenderConnection = WindowEvents->PreRender().connect(&SceneBase::OnPreRender, this, std::placeholders::_1);
	m_OnRenderConnection = WindowEvents->Render().connect(&SceneBase::OnRender, this, std::placeholders::_1);
	m_OnPostRenderConnection = WindowEvents->PostRender().connect(&SceneBase::OnPostRender, this, std::placeholders::_1);
	m_OnRenderUIConnection = WindowEvents->RenderUI().connect(&SceneBase::OnRenderUI, this, std::placeholders::_1);

	// Window events connections
	m_OnResizeConnection = WindowEvents->WindowResize().connect(&SceneBase::OnWindowResize, this, std::placeholders::_1);

	// Keyboard
	m_OnKeyPressedConnection = WindowEvents->WindowKeyPressed().connect(&SceneBase::OnWindowKeyPressed, this, std::placeholders::_1);
	m_OnKeyReleasedConnection = WindowEvents->WindowKeyReleased().connect(&SceneBase::OnWindowKeyReleased, this, std::placeholders::_1);

	// Mouse
	m_OnMouseButtonPressedConnection = WindowEvents->WindowMouseButtonPressed().connect(&SceneBase::OnWindowMouseButtonPressed, this, std::placeholders::_1);
	m_OnMouseButtonReleasedConnection = WindowEvents->WindowMouseButtonReleased().connect(&SceneBase::OnWindowMouseButtonReleased, this, std::placeholders::_1);
	m_OnMouseMovedConnection = WindowEvents->WindowMouseMoved().connect(&SceneBase::OnWindowMouseMoved, this, std::placeholders::_1);
	m_OnMouseWheelConnection = WindowEvents->WindowMouseWheel().connect(&SceneBase::OnWindowMouseWheel, this, std::placeholders::_1);
}

void SceneBase::DisconnectEvents(const std::shared_ptr<IRenderWindowEvents>& WindowEvents)
{
	WindowEvents->Update().disconnect(m_OnUpdateConnection);
	WindowEvents->PreRender().disconnect(m_OnPreRenderConnection);
	WindowEvents->Render().disconnect(m_OnRenderConnection);
	WindowEvents->PostRender().disconnect(m_OnPostRenderConnection);
	WindowEvents->RenderUI().disconnect(m_OnRenderUIConnection);

	WindowEvents->WindowKeyPressed().disconnect(m_OnKeyPressedConnection);
	WindowEvents->WindowKeyReleased().disconnect(m_OnKeyReleasedConnection);

	WindowEvents->WindowMouseButtonPressed().disconnect(m_OnMouseButtonPressedConnection);
	WindowEvents->WindowMouseButtonReleased().disconnect(m_OnMouseButtonReleasedConnection);
	WindowEvents->WindowMouseMoved().disconnect(m_OnMouseMovedConnection);
	WindowEvents->WindowMouseWheel().disconnect(m_OnMouseWheelConnection);

	SetRenderWindow(nullptr);
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
	m_DefaultCameraController = CameraController;
}

std::shared_ptr<ICameraController> SceneBase::GetCameraController() const
{
	return m_DefaultCameraController;
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
// Scene events
//
Delegate<SceneChangeEventArgs>& SceneBase::SceneChangeEvent()
{
	return m_SceneChangeEvent;
}

void SceneBase::RaiseSceneChangeEvent(ESceneChangeType SceneChangeType, const std::shared_ptr<ISceneNode3D>& OwnerNode, const std::shared_ptr<ISceneNode3D>& ChildNode)
{
	m_SceneChangeEvent(SceneChangeEventArgs(this, SceneChangeType, OwnerNode, ChildNode));
}



//
// RenderWindow events
//
void SceneBase::OnUpdate(UpdateEventArgs& e)
{
	if (GetCameraController())
	{
		GetCameraController()->OnUpdate(e);
		e.Camera = GetCameraController()->GetCamera().get();
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
	if (GetCameraController())
	{
		e.Camera = GetCameraController()->GetCamera().get();
		e.CameraForCulling = GetCameraController()->GetCamera().get();
	}

	if (m_Renderer)
		m_Renderer->Render3D(e);
}

void SceneBase::OnPostRender(RenderEventArgs & e)
{


	//m_TestQuery->End(e.FrameCounter);
	//m_FrameQuery->End(e.FrameCounter);


	if (GetCameraController())
	{
		glm::vec3 cameraTrans = GetCameraController()->GetCamera()->GetTranslation();
		m_CameraPosText->GetProperties()->GetPropertyT<std::string>("Text")->Set("Pos: x = " + std::to_string(cameraTrans.x) + ", y = " + std::to_string(cameraTrans.y) + ", z = " + std::to_string(cameraTrans.z));
		m_CameraRotText->GetProperties()->GetPropertyT<std::string>("Text")->Set("Rot: yaw = " + std::to_string(GetCameraController()->GetCamera()->GetYaw()) + ", pitch = " + std::to_string(GetCameraController()->GetCamera()->GetPitch()));
		m_CameraRot2Text->GetProperties()->GetPropertyT<std::string>("Text")->Set("Rot: [" + std::to_string(GetCameraController()->GetCamera()->GetDirection().x) + ", " + std::to_string(GetCameraController()->GetCamera()->GetDirection().y) + ", " + std::to_string(GetCameraController()->GetCamera()->GetDirection().z) + "].");
	}


}

void SceneBase::OnRenderUI(RenderEventArgs & e)
{
	m_End = std::chrono::high_resolution_clock::now();

	if (GetCameraController())
	{
		e.Camera = GetCameraController()->GetCamera().get();
		e.CameraForCulling = GetCameraController()->GetCamera().get();
	}

	if (m_Renderer)
		m_Renderer->RenderUI(e);

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
	}
}



//
// Window events
//
void SceneBase::OnWindowResize(ResizeEventArgs & e)
{
	if (GetCameraController())
		GetCameraController()->OnResize(e);

	if (m_Renderer)
		m_Renderer->Resize(e.Width, e.Height);
}



//
// Keyboard events
//
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



//
// Mouse events
//
void SceneBase::OnWindowMouseMoved(MouseMotionEventArgs & e)
{
	if (GetCameraController() == nullptr)
		throw CException("You must set CameraController to scene!");
		
	GetCameraController()->OnMouseMoved(e);

	OnMouseMoved(e, GetCameraController()->ScreenToRay(GetRenderWindow()->GetViewport(), e.GetPoint()));

	if (GetRootNodeUI())
		DoMouseMoved_Rec(GetRootNodeUI(), e);
}

bool SceneBase::OnWindowMouseButtonPressed(MouseButtonEventArgs & e)
{	
	if (GetCameraController() == nullptr)
		throw CException("You must set CameraController to scene!");

	GetCameraController()->OnMouseButtonPressed(e);

	if (OnMousePressed(e, GetCameraController()->ScreenToRay(GetRenderWindow()->GetViewport(), e.GetPoint())))
		return true;

	if (GetRootNodeUI())
		return DoMouseButtonPressed_Rec(GetRootNodeUI(), e);

	return false;
}

void SceneBase::OnWindowMouseButtonReleased(MouseButtonEventArgs & e)
{
	if (GetCameraController() == nullptr)
		throw CException("You must set CameraController to scene!");
		
	GetCameraController()->OnMouseButtonReleased(e);

	OnMouseReleased(e, GetCameraController()->ScreenToRay(GetRenderWindow()->GetViewport(), e.GetPoint()));

	if (GetRootNodeUI())
		DoMouseButtonReleased_Rec(GetRootNodeUI(), e);
}

bool SceneBase::OnWindowMouseWheel(MouseWheelEventArgs & e)
{
	if (GetCameraController())
		GetCameraController()->OnMouseWheel(e);

	if (GetRootNodeUI())
		return DoMouseWheel_Rec(GetRootNodeUI(), e);

	return false;
}



//
// Mouse in world events
//
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
// IBaseManagerHolder
//
IBaseManager& SceneBase::GetBaseManager() const
{
	return m_BaseManager;
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

IRenderDevice& SceneBase::GetRenderDevice() const
{
	return m_RenderDevice;
}



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