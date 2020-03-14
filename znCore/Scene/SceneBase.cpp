#include "stdafx.h"

// General
#include "SceneBase.h"

// Additonal
#include "3D/SceneNode3D.h"
#include "UI/SceneNodeUI.h"
#include "XML/XMLManager.h"

SceneBase::SceneBase(IBaseManager& BaseManager)
	: m_BaseManager(BaseManager)
	, m_RenderDevice(BaseManager.GetApplication().GetRenderDevice())
{
	
}

SceneBase::~SceneBase()
{}

void SceneBase::Initialize()
{
	m_VideoSettings = GetBaseManager().GetManager<ISettings>()->GetGroup("Video");

	m_FrameQuery = GetRenderDevice().GetObjectsFactory().CreateQuery(IQuery::QueryType::Timer, 1);
	m_TestQuery = GetRenderDevice().GetObjectsFactory().CreateQuery(IQuery::QueryType::CountSamples, 1);

	m_RootNode3D = CreateSceneNode<SceneNode3D>(nullptr);
	m_RootNode3D->SetName("Root node 3D");

	m_RootNodeUI = std::make_shared<CUIBaseNode>();
	std::dynamic_pointer_cast<CUIBaseNode>(m_RootNodeUI)->SetScene(weak_from_this());
	m_RootNodeUI->Initialize();
	m_RootNodeUI->SetParent(nullptr);
	m_RootNodeUI->SetName("Root node UI");

	{
		m_CameraPosText = GetBaseManager().GetManager<ISceneNodesFactory>()->CreateSceneNodeUI(GetRootNodeUI().get(), "TextUI");
		m_CameraPosText->SetTranslate(vec2(700.0f, 0.0f));

		m_CameraRotText = GetBaseManager().GetManager<ISceneNodesFactory>()->CreateSceneNodeUI(GetRootNodeUI().get(), "TextUI");
		m_CameraRotText->SetTranslate(vec2(700.0f, 20.0f));

		m_CameraRot2Text = GetBaseManager().GetManager<ISceneNodesFactory>()->CreateSceneNodeUI(GetRootNodeUI().get(), "TextUI");
		m_CameraRot2Text->SetTranslate(vec2(700.0f, 40.0f));

		m_FPSText = GetBaseManager().GetManager<ISceneNodesFactory>()->CreateSceneNodeUI(GetRootNodeUI().get(), "TextUI");
		m_FPSText->SetTranslate(vec2(700.0f, 60.0f));
	}
}

void SceneBase::Finalize()
{
}

void SceneBase::ConnectEvents(const std::shared_ptr<IRenderWindowEvents>& WindowEvents)
{
	m_RenderWindow = std::dynamic_pointer_cast<IRenderWindow>(WindowEvents);

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

	m_RenderWindow.reset();
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

void SceneBase::SetCameraController(std::shared_ptr<ICameraController> CameraController)
{
	_ASSERT(CameraController != nullptr);
	m_DefaultCameraController = CameraController;
}

std::shared_ptr<ICameraController> SceneBase::GetCameraController() const
{
	return m_DefaultCameraController;
}

void SceneBase::Accept(IVisitor * visitor)
{
	std::lock_guard<std::mutex> lock(m_SceneIsBusy);

	if (m_RootNode3D)
		m_RootNode3D->Accept(visitor);

	if (m_RootNodeUI)
		m_RootNodeUI->Accept(visitor);
}

void SceneBase::AddChild(const std::shared_ptr<ISceneNode3D>& ParentNode, const std::shared_ptr<ISceneNode3D>& ChildNode)
{
	if (ParentNode == nullptr)
	{
		Log::Warn("Can't add child instanse to nullptr parent.");
		return;
	}

	if (m_SceneIsBusy.try_lock() == false)
	{
		std::lock_guard<std::mutex> lock(m_ListsAreBusy);
		m_AddChildList.push_back(std::make_pair(ParentNode, ChildNode));
	}
	else
	{
		ParentNode->AddChild(ChildNode);
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

	if (m_SceneIsBusy.try_lock() == false)
	{
		std::lock_guard<std::mutex> lock(m_ListsAreBusy);

		m_RemoveChildList.push_back(std::make_pair(ParentNode, ChildNode));
	}
	else
	{
		ParentNode->RemoveChild(ChildNode);
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

void SceneBase::RaiseSceneChangeEvent(ESceneChangeType SceneChangeType, const ISceneNode3D* OwnerNode, const ISceneNode3D* ChildNode)
{
	m_SceneChangeEvent(SceneChangeEventArgs(this, this, SceneChangeType, OwnerNode, ChildNode));
}

void SceneBase::RaiseRayIntersected(const glm::vec3 & Point)
{
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

	{
		std::lock_guard<std::mutex> lock(m_ListsAreBusy);

		for (const auto& it : m_AddChildList)
			it.first->AddChild(it.second);
		m_AddChildList.clear();

		for (const auto& it : m_RemoveChildList)
			it.first->RemoveChild(it.second);
		m_RemoveChildList.clear();
	}

	if (m_RootNode3D)
		DoUpdate_Rec(m_RootNode3D, e);
}

void SceneBase::OnPreRender(RenderEventArgs & e)
{
	//m_FrameQuery->Begin(e.FrameCounter);
//m_TestQuery->Begin(e.FrameCounter);
	m_Start = std::chrono::high_resolution_clock::now();
}

void SceneBase::OnRender(RenderEventArgs & e)
{
	if (GetCameraController())
	{
		e.Camera = GetCameraController()->GetCamera().get();
		e.CameraForCulling = GetCameraController()->GetCamera().get();
	}

	//Sleep(15);
	m_Technique3D.Render(e);
}

void SceneBase::OnPostRender(RenderEventArgs & e)
{
	//m_TestQuery->End(e.FrameCounter);
	//m_FrameQuery->End(e.FrameCounter);


	if (GetCameraController())
	{
		vec3 cameraTrans = GetCameraController()->GetCamera()->GetTranslation();
		m_CameraPosText->GetProperties()->GetPropertyT<std::string>("Text")->Set("Pos: x = " + std::to_string(cameraTrans.x) + ", y = " + std::to_string(cameraTrans.y) + ", z = " + std::to_string(cameraTrans.z));
		m_CameraRotText->GetProperties()->GetPropertyT<std::string>("Text")->Set("Rot: yaw = " + std::to_string(GetCameraController()->GetCamera()->GetYaw()) + ", pitch = " + std::to_string(GetCameraController()->GetCamera()->GetPitch()));
		m_CameraRot2Text->GetProperties()->GetPropertyT<std::string>("Text")->Set("Rot: [" + std::to_string(GetCameraController()->GetCamera()->GetDirection().x) + ", " + std::to_string(GetCameraController()->GetCamera()->GetDirection().y) + ", " + std::to_string(GetCameraController()->GetCamera()->GetDirection().z) + "].");
	}


}

void SceneBase::OnRenderUI(RenderEventArgs & e)
{
	if (GetCameraController())
		e.Camera = GetCameraController()->GetCamera().get();



	m_TechniqueUI.Render(e);

	m_End = std::chrono::high_resolution_clock::now();

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

	m_Technique3D.UpdateViewport(GetRenderWindow()->GetViewport());
	m_TechniqueUI.UpdateViewport(GetRenderWindow()->GetViewport());
}



//
// Keyboard events
//
bool SceneBase::OnWindowKeyPressed(KeyEventArgs & e)
{
	if (GetCameraController())
		GetCameraController()->OnKeyPressed(e);

	/*if (e.Key == KeyCode::J)
	{
		CXMLManager xmlM;

		// Writer
		std::shared_ptr<IXMLWriter> writer = xmlM.CreateWriter();

		Save(writer);

		std::shared_ptr<IFile> xmlFile = writer->SaveToFile("Scene.xml");
		GetBaseManager().GetManager<IFilesManager>()->GetFilesStorage("ZenonGamedata")->SaveFile(xmlFile);
	}*/

	if (m_RootNodeUI)
		return DoKeyPressed_Rec(m_RootNodeUI, e);

	return false;
}

void SceneBase::OnWindowKeyReleased(KeyEventArgs & e)
{
	if (GetCameraController())
		GetCameraController()->OnKeyReleased(e);

	if (m_RootNodeUI)
		DoKeyReleased_Rec(m_RootNodeUI, e);
}



//
// Mouse events
//
void SceneBase::OnWindowMouseMoved(MouseMotionEventArgs & e)
{
	if (GetCameraController())
		GetCameraController()->OnMouseMoved(e);

	if (m_RootNodeUI)
		DoMouseMoved_Rec(m_RootNodeUI, e);
}

bool SceneBase::OnWindowMouseButtonPressed(MouseButtonEventArgs & e)
{	
	if (GetCameraController())
	{
		GetCameraController()->OnMouseButtonPressed(e);

		if (e.LeftButton)
		{
			Ray cameraDownRay = Ray(GetCameraController()->GetCamera()->GetTranslation(), glm::vec3(0.0f, -1.0f, 0.0f));
			Ray resultRay = GetCameraController()->ScreenPointToRay(GetRenderWindow()->GetViewport(), glm::vec2(e.X, e.Y));

			float cosAlpha = (resultRay.GetDirection().x * cameraDownRay.GetDirection().x) + (resultRay.GetDirection().y * cameraDownRay.GetDirection().y) + (resultRay.GetDirection().z * cameraDownRay.GetDirection().z);
			cosAlpha /=
				(
					sqrt
					(
					(resultRay.GetDirection().x * resultRay.GetDirection().x) +
						(resultRay.GetDirection().y * resultRay.GetDirection().y) +
						(resultRay.GetDirection().z * resultRay.GetDirection().z)
					)
					*
					sqrt
					(
					(cameraDownRay.GetDirection().x * cameraDownRay.GetDirection().x) +
						(cameraDownRay.GetDirection().y * cameraDownRay.GetDirection().y) +
						(cameraDownRay.GetDirection().z * cameraDownRay.GetDirection().z)
					)
					);

			float d = GetCameraController()->GetCamera()->GetTranslation().y / cosAlpha;
			if (d < 10000.0f)
			{
				glm::vec3 point = resultRay.GetPointOnRay(d);
				RaiseRayIntersected(point);
			}
		}
	}

	if (m_RootNodeUI)
		return DoMouseButtonPressed_Rec(m_RootNodeUI, e);

	return false;
}

void SceneBase::OnWindowMouseButtonReleased(MouseButtonEventArgs & e)
{
	if (GetCameraController())
		GetCameraController()->OnMouseButtonReleased(e);

	if (m_RootNodeUI)
		DoMouseButtonReleased_Rec(m_RootNodeUI, e);
}

bool SceneBase::OnWindowMouseWheel(MouseWheelEventArgs & e)
{
	if (GetCameraController())
		GetCameraController()->OnMouseWheel(e);

	if (m_RootNodeUI)
		return DoMouseWheel_Rec(m_RootNodeUI, e);

	return false;
}



//
// IBaseManagerHolder
//
IBaseManager& SceneBase::GetBaseManager() const
{
	return m_BaseManager;
}



//
// Protected
//

IRenderDevice& SceneBase::GetRenderDevice() const
{
	return m_RenderDevice;
}

std::shared_ptr<IRenderWindow> SceneBase::GetRenderWindow() const
{
	std::shared_ptr<IRenderWindow> renderWindow = m_RenderWindow.lock();
	_ASSERT(renderWindow);
	return std::move(renderWindow);
}



void SceneBase::DoUpdate_Rec(const std::shared_ptr<ISceneNode3D>& Node, const UpdateEventArgs & e)
{
	Node->Update(e);

	const auto& components = Node->GetComponents();
	std::for_each(components.begin(), components.end(), [&e](const std::pair<GUID, std::shared_ptr<ISceneNodeComponent>>& Component)
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
	std::shared_ptr<CUIBaseNode> NodeAsUINode = std::dynamic_pointer_cast<CUIBaseNode>(Node);
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
	std::shared_ptr<CUIBaseNode> NodeAsUINode = std::dynamic_pointer_cast<CUIBaseNode>(Node);
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
	std::shared_ptr<CUIBaseNode> NodeAsUINode = std::dynamic_pointer_cast<CUIBaseNode>(Node);
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
	std::shared_ptr<CUIBaseNode> NodeAsUINode = std::dynamic_pointer_cast<CUIBaseNode>(Node);
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
	std::shared_ptr<CUIBaseNode> NodeAsUINode = std::dynamic_pointer_cast<CUIBaseNode>(Node);
	if (NodeAsUINode != nullptr)
	{
		for (auto child : NodeAsUINode->GetChilds())
			DoMouseButtonReleased_Rec(child, e);

		NodeAsUINode->OnMouseButtonReleased(e);
	}
}

bool SceneBase::DoMouseWheel_Rec(const std::shared_ptr<ISceneNodeUI>& Node, MouseWheelEventArgs & e)
{
	std::shared_ptr<CUIBaseNode> NodeAsUINode = std::dynamic_pointer_cast<CUIBaseNode>(Node);
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