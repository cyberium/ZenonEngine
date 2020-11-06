#include "stdafx.h"

// General
#include "SceneBase.h"

#include <sstream>
#include <iomanip>

namespace
{
	template <typename T>
	std::string toStringPrec(const T Value, const int N = 3)
	{
		std::string strVal(std::to_string(Value));
		const auto& it = strVal.find('.');
		if (it == std::string::npos)
			return strVal;

		size_t offset = it + 1;
		if (offset + N < strVal.length())
			offset += N;
		else
			offset += strVal.length() - offset;

		return strVal.substr(0, offset);
	}
}

SceneBase::SceneBase(IBaseManager& BaseManager, IRenderWindow& RenderWindow)
	: m_BaseManager(BaseManager)
	, m_RenderDevice(BaseManager.GetApplication().GetRenderDevice())
	, m_RenderWindow(RenderWindow)
	, m_Finder(*this)
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

const ISceneFinder& SceneBase::GetFinder() const
{
	return m_Finder;
}


void SceneBase::Initialize()
{
	m_VideoSettings = GetBaseManager().GetManager<ISettings>()->GetGroup("Video");

	m_FrameQuery = GetRenderDevice().GetObjectsFactory().CreateQuery(IQuery::QueryType::Timer, 1);
	m_TestQuery = GetRenderDevice().GetObjectsFactory().CreateQuery(IQuery::QueryType::CountSamples, 1);

	m_RootSceneNode = CreateSceneNode<ISceneNode>();
	m_RootSceneNode->SetName("RootSceneNode");
	std::dynamic_pointer_cast<ISceneNodeInternal>(m_RootSceneNode)->SetPersistanceInternal(true);

	m_RootUIControl = GetBaseManager().GetManager<IObjectsFactory>()->GetClassFactoryCast<IUIControlFactory>()->CreateSceneNodeUI(cSceneNodeUI, *this);
	m_RootUIControl->SetName("RootUIControl");

	m_StatisticText = GetBaseManager().GetManager<IObjectsFactory>()->GetClassFactoryCast<IUIControlFactory>()->CreateSceneNodeUI(cSceneNodeUI_Text, *this, GetRootUIControl());
	m_StatisticText->SetTranslate(glm::vec2(5.0f, 5.0f));
	m_StatisticText->GetProperties()->GetPropertyT<std::string>("Text")->Set("");

}

void SceneBase::Finalize()
{
}


void SceneBase::AddSceneEventsListener(ISceneEventsListener* Listener)
{
	const auto& it = std::find(m_EventListeners.begin(), m_EventListeners.end(), Listener);
	if (it != m_EventListeners.end())
		throw CException("Unable to add 'ISceneEventsListener' because already exists.");
	m_EventListeners.push_back(Listener);
}

void SceneBase::RemoveSceneEventsListener(ISceneEventsListener* Listener)
{
	const auto& it = std::find(m_EventListeners.begin(), m_EventListeners.end(), Listener);
	if (it == m_EventListeners.end())
		throw CException("Unable to remove 'ISceneEventsListener' because non exists.");
	m_EventListeners.erase(it);
}



//
// IScene
//
std::shared_ptr<ISceneNode> SceneBase::GetRootSceneNode() const
{
	return m_RootSceneNode;
}

std::shared_ptr<IUIControl> SceneBase::GetRootUIControl() const
{
	return m_RootUIControl;
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




void SceneBase::Accept(IVisitor * visitor)
{
	std::lock_guard<std::mutex> lock(m_ChildModifyLock);

	if (GetRootSceneNode())
		GetRootSceneNode()->Accept(visitor);

	if (GetRootUIControl())
		GetRootUIControl()->Accept(visitor);
}



//
// ISceneInternal
//
void SceneBase::AddChildInternal(const std::shared_ptr<ISceneNode>& ParentNode, const std::shared_ptr<ISceneNode>& ChildNode)
{
	if (ParentNode == nullptr)
	{
		Log::Warn("Can't add child instanse to nullptr parent.");
		return;
	}

	if (m_ChildModifyLock.try_lock())
	{
		try
		{
			std::dynamic_pointer_cast<ISceneNodeInternal>(ParentNode)->AddChildInternal(ChildNode);
			RaiseSceneChangeEvent(ESceneChangeType::NodeAddedToParent, ParentNode, ChildNode);
		}
		catch (...)
		{
			m_ChildModifyLock.unlock();
			throw;
		}

		m_ChildModifyLock.unlock();
	}
	else
	{
		std::lock_guard<std::mutex> lock(m_SceneChangeDelayEventsLock);
		m_SceneChangeDelayEvents.push_back({ ESceneChangeType::NodeAddedToParent, ParentNode, ChildNode });
	}
}

void SceneBase::RemoveChildInternal(const std::shared_ptr<ISceneNode>& ParentNode, const std::shared_ptr<ISceneNode>& ChildNode)
{
	if (ParentNode == nullptr)
	{
		Log::Error("Can't remove child node '%s' from nullptr parent.", ChildNode->GetName().c_str());
		return;
	}

	if (m_ChildModifyLock.try_lock())
	{
		try
		{
			std::dynamic_pointer_cast<ISceneNodeInternal>(ParentNode)->RemoveChildInternal(ChildNode);
			RaiseSceneChangeEvent(ESceneChangeType::NodeRemovedFromParent, ParentNode, ChildNode);
		}
		catch (...)
		{
			m_ChildModifyLock.unlock();
			throw;
		}

		m_ChildModifyLock.unlock();
	}
	else
	{
		std::lock_guard<std::mutex> lock(m_SceneChangeDelayEventsLock);
		m_SceneChangeDelayEvents.push_back({ ESceneChangeType::NodeRemovedFromParent, ParentNode, ChildNode });
	}
}



//
// IRenderWindowEventListener
//
void SceneBase::OnUpdate(UpdateEventArgs& e)
{
	if (GetCameraController())
	{
		GetCameraController()->OnUpdate(e);

		e.Camera = GetCameraController()->GetCamera().get();
		e.CameraForCulling = GetCameraController()->GetCamera().get();
	}

	std::lock_guard<std::mutex> lock(m_ChildModifyLock);
	{
		std::lock_guard<std::mutex> sceneChangeDelayEventsLockGuard(m_SceneChangeDelayEventsLock);

		for (const auto& it : m_SceneChangeDelayEvents)
		{
			auto parent = it.Parent;
			auto child = it.Child;

			if (it.EventType == ESceneChangeType::NodeAddedToParent)
			{
				parent->AddChild(child);
			}
			else if (it.EventType == ESceneChangeType::NodeRemovedFromParent)
			{
				parent->RemoveChild(child);
			}
			else
			{
				throw CException("Unknown SceneChangeType '%d'.", it.EventType);
			}

			RaiseSceneChangeEvent(it.EventType, parent, child);
		}
		m_SceneChangeDelayEvents.clear();
	}

	if (GetRootSceneNode())
		DoUpdate_Rec(GetRootSceneNode(), e);
}

void SceneBase::OnRender(RenderEventArgs & e)
{
	auto cameraController = GetCameraController();
	if (cameraController == nullptr)
		throw CException("You must set CameraController to scene!");

	auto renderer = GetRenderer();
	if (renderer == nullptr)
		throw CException("You must set Renderer to scene!");

	// 3D render
	{
		e.Camera = cameraController->GetCamera().get();
		e.CameraForCulling = cameraController->GetCamera().get();

		renderer->Render3D(e);
	}

	// UI render
	{
		glm::vec3 cameraPos = e.Camera->GetTranslation();
		glm::vec3 cameraRot = e.Camera->GetDirection();

		double fpsValue = 1000.0 / double(GetRenderWindow().GetSummaDeltaTime());

		std::string fullText = "FPS: " + toStringPrec(uint64(fpsValue)) + "\n";
		fullText += "CamPos: (" + toStringPrec(cameraPos.x) + ", " + toStringPrec(cameraPos.y) + ", " + toStringPrec(cameraPos.z) + ")\n";
		fullText += "CamRot: yaw " + toStringPrec(e.Camera->GetYaw()) + ", pitch " + toStringPrec(e.Camera->GetPitch()) + "\n";
		fullText += "CamRot: (" + toStringPrec(cameraRot.x) + ", " + toStringPrec(cameraRot.y) + ", " + toStringPrec(cameraRot.z) + ")\n";
		fullText += "Frame Update: " + toStringPrec(GetRenderWindow().GetUpdateDeltaTime()) + " ms\n";
		fullText += "Frame Render: " + toStringPrec(GetRenderWindow().GetRenderDeltaTime()) + " ms\n";
		fullText += "Frame Total : " + toStringPrec(GetRenderWindow().GetSummaDeltaTime()) + " ms\n";

		m_StatisticText->GetProperties()->GetPropertyT<std::string>("Text")->Set(fullText);

		renderer->RenderUI(e);
	}
}



//
// IznNativeWindowEventListener
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

	if (GetRootUIControl())
		return DoKeyPressed_Rec(GetRootUIControl(), e);

	return false;
}

void SceneBase::OnWindowKeyReleased(KeyEventArgs & e)
{
	if (GetCameraController())
		GetCameraController()->OnKeyReleased(e);

	if (GetRootUIControl())
		DoKeyReleased_Rec(GetRootUIControl(), e);
}

// Mouse events

void SceneBase::OnWindowMouseMoved(MouseMotionEventArgs & e)
{
	OnMouseMoved(e, GetCameraController()->ScreenToRay(GetRenderWindow().GetViewport(), e.GetPoint()));

	if (GetCameraController() == nullptr)
		throw CException("You must set CameraController to scene!");
	GetCameraController()->OnMouseMoved(e);

	if (GetRootUIControl())
		DoMouseMoved_Rec(GetRootUIControl(), e);
}

bool SceneBase::OnWindowMouseButtonPressed(MouseButtonEventArgs & e)
{
	if (OnMousePressed(e, GetCameraController()->ScreenToRay(GetRenderWindow().GetViewport(), e.GetPoint())))
		return true;

	if (GetCameraController() == nullptr)
		throw CException("You must set CameraController to scene!");
	GetCameraController()->OnMouseButtonPressed(e);

	if (GetRootUIControl())
		return DoMouseButtonPressed_Rec(GetRootUIControl(), e);

	return false;
}

void SceneBase::OnWindowMouseButtonReleased(MouseButtonEventArgs & e)
{
	OnMouseReleased(e, GetCameraController()->ScreenToRay(GetRenderWindow().GetViewport(), e.GetPoint()));

	if (GetCameraController() == nullptr)
		throw CException("You must set CameraController to scene!");
	GetCameraController()->OnMouseButtonReleased(e);

	if (GetRootUIControl())
		DoMouseButtonReleased_Rec(GetRootUIControl(), e);
}

bool SceneBase::OnWindowMouseWheel(MouseWheelEventArgs & e)
{
	if (GetCameraController())
		if (GetCameraController()->OnMouseWheel(e))
			return true;

	if (GetRootUIControl())
		if (DoMouseWheel_Rec(GetRootUIControl(), e))
			return true;

	return false;
}



//
//
//
void SceneBase::LoadFromFile(const std::string& FileName)
{
	auto file = GetBaseManager().GetManager<IFilesManager>()->Open(FileName);
	if (file == nullptr)
		throw CException("Scene file '%s' not found.", FileName.c_str());

	ResetScene();

	auto root = GetRootSceneNode();

	CXMLManager xml(GetBaseManager());
	auto xmlReader = xml.CreateReader(file);
	auto xmlRootChild = xmlReader->GetChilds()[0];

	auto tempFakeRoot = CreateSceneNode<ISceneNode>(root);
	tempFakeRoot->SetName("TempFakeRoot");

	auto newRootNode = GetBaseManager().GetManager<IObjectsFactory>()->GetClassFactoryCast<ISceneNodeFactory>()->LoadSceneNode3DXML(xmlRootChild, *this, tempFakeRoot);


	// Update persistance nodes
	for (const auto& existingRootChild : root->GetChilds())
	{
		if (existingRootChild->IsPersistance())
		{
			auto xmlChild = newRootNode->GetChild(existingRootChild->GetName());
			if (xmlChild != nullptr)
			{
				if (auto loadSave = std::dynamic_pointer_cast<IObjectLoadSave>(xmlChild))
				{
					loadSave->CopyTo(existingRootChild);

					// To delete persistance node, we must clear this flag
					std::dynamic_pointer_cast<ISceneNodeInternal>(xmlChild)->SetPersistanceInternal(false);
					newRootNode->RemoveChild(xmlChild);
				}
			}
		}
	}

	// Add new childs
	while (false == newRootNode->GetChilds().empty())
	{
		auto newRootChild = *(newRootNode->GetChilds().begin());
		root->AddChild(newRootChild);
	}

	tempFakeRoot->MakeMeOrphan();
}

void SceneBase::SaveToFile(const std::string& FileName) const
{
	std::shared_ptr<IFile> file = m_BaseManager.GetManager<IFilesManager>()->Create(FileName);

	CXMLManager manager(GetBaseManager());

	auto rootNode = GetRootSceneNode();
	rootNode->SetName(file->Name_NoExtension());

	auto rootWriter = GetBaseManager().GetManager<IObjectsFactory>()->GetClassFactoryCast<ISceneNodeFactory>()->SaveSceneNode3DXML(rootNode);

	auto xml = manager.CreateWriter();
	xml->AddChild(rootWriter);

	manager.SaveWriterToFile(xml, file)->Save();
}

void SceneBase::ResetScene()
{
	auto root = GetRootSceneNode();

	// Remove existing childs
	Log::Info("ResetScene: Root contains '%d' childs BEFORE reset.", root->GetChilds().size());
	size_t mustLeaveExisting = 0;
	while (root->GetChilds().size() > mustLeaveExisting)
	{
		auto editorChild = *(root->GetChilds().begin() + mustLeaveExisting);
		if (editorChild->IsPersistance())
		{
			mustLeaveExisting++;
			Log::Info("ResetScene: Child '%s' was not removed, because is persistant.", editorChild->GetName().c_str());
			continue;
		}

		root->RemoveChild(editorChild);
	}
	Log::Info("ResetScene: Root contains '%d' childs AFTER reset.", root->GetChilds().size());
}




//
// Protected
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

void SceneBase::RaiseSceneChangeEvent(ESceneChangeType SceneChangeType, const std::shared_ptr<ISceneNode>& ParentNode, const std::shared_ptr<ISceneNode>& ChildNode)
{
	if (SceneChangeType == ESceneChangeType::NodeAddedToParent)
	{
		for (const auto& el : m_EventListeners)
			el->OnSceneNodeAdded(ParentNode, ChildNode);
	}
	else if (SceneChangeType == ESceneChangeType::NodeRemovedFromParent)
	{
		for (const auto& el : m_EventListeners)
			el->OnSceneNodeRemoved(ParentNode, ChildNode);
	}
}



//
// Protected
//
void SceneBase::DoUpdate_Rec(const std::shared_ptr<ISceneNode>& Node, const UpdateEventArgs & e)
{
	Node->Update(e);

	const auto& components = Node->GetComponents();
	std::for_each(components.begin(), components.end(), [&e](const std::pair<ObjectClass, std::shared_ptr<ISceneNodeComponent>>& Component)
	{
		_ASSERT(Component.second);
		Component.second->Update(e);
	});

	const auto& childs = Node->GetChilds();
	std::for_each(childs.begin(), childs.end(), [this, &e](const std::shared_ptr<ISceneNode>& Child)
	{
		if (Child != nullptr)
			DoUpdate_Rec(Child, e);
	});
}

bool SceneBase::DoKeyPressed_Rec(const std::shared_ptr<IUIControl>& Node, KeyEventArgs & e)
{
	std::shared_ptr<CUIControl> NodeAsUINode = std::dynamic_pointer_cast<CUIControl>(Node);
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

void SceneBase::DoKeyReleased_Rec(const std::shared_ptr<IUIControl>& Node, KeyEventArgs & e)
{
	std::shared_ptr<CUIControl> NodeAsUINode = std::dynamic_pointer_cast<CUIControl>(Node);
	if (NodeAsUINode != nullptr)
	{

		for (auto child : NodeAsUINode->GetChilds())
		{
			DoKeyReleased_Rec(child, e);
		}

		NodeAsUINode->OnKeyReleased(e);
	}
}

void SceneBase::DoMouseMoved_Rec(const std::shared_ptr<IUIControl>& Node, MouseMotionEventArgs & e)
{
	std::shared_ptr<CUIControl> NodeAsUINode = std::dynamic_pointer_cast<CUIControl>(Node);
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

bool SceneBase::DoMouseButtonPressed_Rec(const std::shared_ptr<IUIControl>& Node, MouseButtonEventArgs & e)
{
	std::shared_ptr<CUIControl> NodeAsUINode = std::dynamic_pointer_cast<CUIControl>(Node);
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

void SceneBase::DoMouseButtonReleased_Rec(const std::shared_ptr<IUIControl>& Node, MouseButtonEventArgs & e)
{
	std::shared_ptr<CUIControl> NodeAsUINode = std::dynamic_pointer_cast<CUIControl>(Node);
	if (NodeAsUINode != nullptr)
	{
		for (auto child : NodeAsUINode->GetChilds())
			DoMouseButtonReleased_Rec(child, e);

		NodeAsUINode->OnMouseButtonReleased(e);
	}
}

bool SceneBase::DoMouseWheel_Rec(const std::shared_ptr<IUIControl>& Node, MouseWheelEventArgs & e)
{
	std::shared_ptr<CUIControl> NodeAsUINode = std::dynamic_pointer_cast<CUIControl>(Node);
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
