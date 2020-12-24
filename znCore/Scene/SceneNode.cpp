#include "stdafx.h"

// General
#include "SceneNode.h"

// Additional
#include "ObjectsFactories/Scene/SceneNodeFactory.h"
#include "XML/XMLManager.h"

CSceneNode::CSceneNode(IScene& Scene)
	: Object(Scene.GetBaseManager())
	, m_Scene(Scene)

	, m_IsEnabled(true)

	, m_IsPersistance(false)

	, m_LocalPosition(0.0f)
	, m_RotationEulerAngles(glm::vec3(0.0f))
	, m_RotationQuaternion(glm::quat())
	, m_RotationDirection(glm::vec3(0.0f))
	, m_Scale(1.0f)

	, m_LocalTransform(1.0f)
	, m_InverseLocalTransform(1.0f)	// This is the inverse of the local -> world transform.
	, m_WorldTransform(1.0f)
	, m_InverseWorldTransform(1.0f)
{
}

CSceneNode::~CSceneNode()
{
	Log::Info("SceneNode '%s' deleted.", GetName().c_str());
}



//
// ISceneNode
//
void CSceneNode::Initialize()
{
	// Name properties
	{
		std::shared_ptr<CPropertyWrapped<std::string>> nameProperty = MakeShared(CPropertyWrapped<std::string>, "Name", "Scene node name.", "");
		nameProperty->SetSyntetic(true);
		nameProperty->SetNonCopyable(true);
		nameProperty->SetValueSetter(std::bind(&Object::SetName, this, std::placeholders::_1));
		nameProperty->SetValueGetter(std::bind(&Object::GetName, this));
		GetProperties()->AddProperty(nameProperty);
	}

	// Transform properties
	{
		std::shared_ptr<IPropertiesGroup> propertiesGroup = MakeShared(CPropertiesGroup, "Transform", "Transorm of this 3D node. Like translation, rotation and scale.");
		propertiesGroup->SetNonCopyable(true);

		m_LocalPositionProperty = MakeShared(CPropertyWrappedVec3, "Translate", "TODO", glm::vec3(0.0f));
		m_LocalPositionProperty->SetValueSetter(std::bind(&CSceneNode::SetPosition, this, std::placeholders::_1));
		m_LocalPositionProperty->SetValueGetter(std::bind(&CSceneNode::GetPosition, this));
		propertiesGroup->AddProperty(m_LocalPositionProperty);

		m_RotationEulerAnglesProperty = MakeShared(CPropertyWrappedVec3, "RotationEuler", "TODO", glm::vec3(0.0f));
		m_RotationEulerAnglesProperty->SetValueSetter(std::bind(&CSceneNode::SetLocalRotationEuler, this, std::placeholders::_1));
		m_RotationEulerAnglesProperty->SetValueGetter(std::bind(&CSceneNode::GetLocalRotationEuler, this));
		propertiesGroup->AddProperty(m_RotationEulerAnglesProperty);

		//m_RotationQuaternionProperty = MakeShared(CPropertyWrappedQuat, "RotationQuat", "TODO", glm::quat(0.0f, 0.0f, 0.0f, 1.0f));
		//m_RotationQuaternionProperty->SetValueSetter(std::bind(&CSceneNode::SetRotationQuaternion, this, std::placeholders::_1));
		//m_RotationQuaternionProperty->SetValueGetter(std::bind(&CSceneNode::GetRotationQuaternion, this));
		//propertiesGroup->AddProperty(m_RotationQuaternionProperty);

		//m_RotationDirectionProperty = MakeShared(CPropertyWrappedVec3, "RotationDir", "TODO", glm::vec3(1.0f, 0.0f, 0.0f));
		//m_RotationDirectionProperty->SetValueSetter(std::bind(&CSceneNode::SetLocalRotationDirection, this, std::placeholders::_1));
		//m_RotationDirectionProperty->SetValueGetter(std::bind(&CSceneNode::GetLocalRotationDirection, this));
		//propertiesGroup->AddProperty(m_RotationDirectionProperty);

		m_ScaleProperty = MakeShared(CPropertyWrappedVec3, "Scale", "TODO", glm::vec3(1.0f));
		m_ScaleProperty->SetValueSetter(std::bind(&CSceneNode::SetLocalScale, this, std::placeholders::_1));
		m_ScaleProperty->SetValueGetter(std::bind(&CSceneNode::GetLocalScale, this));
		propertiesGroup->AddProperty(m_ScaleProperty);

		GetProperties()->AddProperty(propertiesGroup);
	}

	// Actions
	{
		auto removeAction = MakeShared(CAction, "Remove", "Remove this node from world. this action affected on childs!");
		removeAction->SetAction([this]() -> bool {
			GetParent()->RemoveChild(shared_from_this());
			return true;
		});

		removeAction->SetActionPrecondition([this]() -> bool {
			return false == IsPersistance();
		});

		GetProperties()->AddProperty(removeAction);


		/*auto saveAsXMLAction = MakeShared(CAction, "Save as preset", "Save this node as ");
		removeAction->SetAction([this]() -> bool {
			GetParent()->RemoveChild(shared_from_this());
			return true;
		});*/
	}
}

void CSceneNode::Finalize()
{}



void CSceneNode::SetEnabled(bool Value)
{
	m_IsEnabled = Value;
}

bool CSceneNode::IsEnabled() const
{
	return m_IsEnabled;
}



//
// Childs functional
//
void CSceneNode::AddChild(std::shared_ptr<ISceneNode> childNode)
{
	dynamic_cast<ISceneInternal&>(GetScene()).AddChildInternal(shared_from_this(), childNode);
}

void CSceneNode::RemoveChild(std::shared_ptr<ISceneNode> childNode)
{
	dynamic_cast<ISceneInternal&>(GetScene()).RemoveChildInternal(shared_from_this(), childNode);
}

void CSceneNode::MakeMeOrphan()
{
	dynamic_cast<ISceneInternal&>(GetScene()).RemoveChildInternal(GetParent(), shared_from_this());
}

std::shared_ptr<ISceneNode> CSceneNode::GetParent() const
{
	return m_ParentNode.lock();
}

const CSceneNode::SceneNodesList& CSceneNode::GetChilds() const
{
	return m_Children;
}

std::shared_ptr<ISceneNode> CSceneNode::GetChild(std::string Name) const
{
	std::string currClearName = /*GetClearName(*/Name/*).first*/;
	for (const auto& ch : GetChilds())
	{
		std::string childClearName = /*GetClearName(*/ch->GetName()/*).first*/;
		if (childClearName == currClearName)
			return ch;
	}
	return nullptr;
}

bool CSceneNode::IsPersistance() const
{
	return m_IsPersistance;
}


IScene& CSceneNode::GetScene() const
{
	return m_Scene;
}









//
// Components engine
//
bool CSceneNode::IsComponentExists(ObjectClass ComponentID) const
{
	const auto& components = GetComponents();
	return components.find(ComponentID) != components.end();
}

std::shared_ptr<ISceneNodeComponent> CSceneNode::GetComponent(ObjectClass ComponentID) const
{
	const auto& components = GetComponents();
	const auto& component = components.find(ComponentID);
	if (component == components.end())
		return nullptr;
	return component->second;
}

std::shared_ptr<ISceneNodeComponent> CSceneNode::AddComponent(ObjectClass ComponentID, std::shared_ptr<ISceneNodeComponent> Component)
{
	const auto& componentIt = m_Components.find(ComponentID);
	if (componentIt != m_Components.end())
		throw CException("Component with id '%d' already exists in node '%s'", ComponentID, GetName().c_str());

	m_Components[ComponentID] = Component;

	// Add proxy properties
	auto copmonentPropertiesProxy = MakeShared(CPropertyGroupProxy, Component->GetProperties());
	GetProperties()->AddProperty(copmonentPropertiesProxy);

	return Component;
}

const ComponentsMap& CSceneNode::GetComponents() const
{
	return m_Components;
}

void CSceneNode::RaiseComponentMessage(const ISceneNodeComponent* Component, ComponentMessageType Message) const
{
	const auto& components = GetComponents();
	std::for_each(components.begin(), components.end(), [&Component, &Message](const std::pair<ObjectClass, std::shared_ptr<ISceneNodeComponent>>& ComponentMapIter) {
		ComponentMapIter.second->OnMessage(Component, Message);
	});
}

void CSceneNode::RegisterComponents()
{
	AddComponentT(GetBaseManager().GetManager<IObjectsFactory>()->GetClassFactoryCast<IComponentFactory>()->CreateComponentT<IModelComponent>(cSceneNodeModelsComponent, *this));
	AddComponentT(GetBaseManager().GetManager<IObjectsFactory>()->GetClassFactoryCast<IComponentFactory>()->CreateComponentT<IColliderComponent>(cSceneNodeColliderComponent, *this));
}




//
// Others
//
void CSceneNode::Update(const UpdateEventArgs& e)
{
	_ASSERT(IsEnabled());

	const auto& components = GetComponents();
	std::for_each(components.begin(), components.end(), [e](const std::pair<ObjectClass, std::shared_ptr<ISceneNodeComponent>>& ComponentMapIter) {
		ComponentMapIter.second->Update(e);
	});
}

void CSceneNode::Accept(IVisitor* visitor)
{
	//if (const auto& loadableObject = dynamic_cast<ILoadable*>(this))
	//	if (loadableObject->GetState() != ILoadable::ELoadableState::Loaded)
	//		return;

	_ASSERT(IsEnabled());

	EVisitResult visitResult = visitor->Visit(shared_from_this());

	if (visitResult & EVisitResult::AllowVisitContent)
	{
		const auto& components = GetComponents();
		std::for_each(components.begin(), components.end(), [visitor](const std::pair<ObjectClass, std::shared_ptr<ISceneNodeComponent>>& Component) {
			if (Component.second)
				Component.second->Accept(visitor);
		});
	}

	if (visitResult & EVisitResult::AllowVisitChilds)
	{
		const auto& childs = GetChilds();
		std::for_each(childs.begin(), childs.end(), [visitor](const std::shared_ptr<ISceneNode>& Child) {
			if (Child != nullptr)
				Child->Accept(visitor);
		});
	}
}




//
// IObject
//
/*void CSceneNode::SetName(const std::string& Name)
{
	std::string resultName = Name;

	if (false == GetGUID().IsEmpty())
	{
		if (auto parent = GetParent())
		{
			auto childs = parent->GetChilds();
			auto childIt = childs.end();
			do
			{
				childIt = std::find_if(childs.begin(), childs.end(), [this, &resultName](const std::shared_ptr<ISceneNode>& Child) -> bool {
					if (Child.get() == this)
						return false;
					return resultName == Child->GetName();
				});
				if (childIt != childs.end())
				{
					resultName = GetClearName(resultName).first + "#" + std::to_string(GetGUID().GetCounter());
				}
			} while (childIt != childs.end());

			_ASSERT(std::find_if(childs.begin(), childs.end(), [this, &resultName](const std::shared_ptr<ISceneNode>& Child) -> bool {
				if (Child.get() == this)
					return false;
				return resultName == Child->GetName();
			}) == childs.end());
		}
	}

	Object::SetName(resultName);
}*/



//
// IObjectLoadSave
//
std::shared_ptr<IObject> CSceneNode::Copy() const
{
	auto sameObject = GetBaseManager().GetManager<IObjectsFactory>()->GetClassFactoryCast<ISceneNodeFactory>()->CreateSceneNode3D(GetGUID().GetObjectClass(), m_Scene);
	CopyTo(sameObject);
	return sameObject;
}

void CSceneNode::CopyTo(std::shared_ptr<IObject> Destination) const
{
	Object::CopyTo(Destination);

	auto destCast = std::dynamic_pointer_cast<CSceneNode>(Destination);

	destCast->m_IsPersistance = m_IsPersistance;

	destCast->m_LocalPosition = m_LocalPosition;

	destCast->m_RotationEulerAngles = m_RotationEulerAngles;
	destCast->m_RotationQuaternion = m_RotationQuaternion;
	destCast->m_RotationDirection = m_RotationDirection;

	destCast->m_Scale = m_Scale;

	destCast->m_LocalTransform = m_LocalTransform;
	destCast->m_InverseLocalTransform = m_InverseLocalTransform;
	destCast->m_WorldTransform = m_WorldTransform;
	destCast->m_InverseWorldTransform = m_InverseWorldTransform;

	GetProperties()->CopyTo(destCast->GetProperties());

	for (const auto& existingComponentIt : GetComponents())
	{
		const auto& existingComponent = existingComponentIt.second;

		ObjectClass existsingComponentClass = existingComponent->GetClass();
		if (destCast->IsComponentExists(existsingComponentClass))
		{
			const auto& destinationComponent = destCast->GetComponent(existsingComponentClass);
			existingComponent->CopyTo(destinationComponent);
		}
		else
		{
			std::shared_ptr<ISceneNodeComponent> copiedComponent = GetBaseManager().GetManager<IObjectsFactory>()->GetClassFactoryCast<IComponentFactory>()->CreateComponent(existsingComponentClass, *destCast);
			existingComponent->CopyTo(copiedComponent);
			destCast->AddComponent(existsingComponentClass, copiedComponent);
		}
	}

	for (const auto& existingChild : GetChilds())
	{
		ObjectClass existingChildClass = existingChild->GetClass();
		std::shared_ptr<ISceneNode> copiedChild = GetBaseManager().GetManager<IObjectsFactory>()->GetClassFactoryCast<ISceneNodeFactory>()->CreateSceneNode3D(existingChildClass, destCast->GetScene());
		existingChild->CopyTo(copiedChild);
		destCast->AddChild(copiedChild);
		Log::Info("SceneNode::CopyTo: Child '%s' of parent '%s' copied.", existingChild->GetName().c_str(), GetName().c_str());
	}
}

void CSceneNode::Load(const std::shared_ptr<IXMLReader>& Reader)
{
	Object::Load(Reader);

	// Properties
	if (auto reader = Reader->GetChild(GetProperties()->GetName()))
		GetProperties()->Load(reader);

	if (auto componentsWriter = Reader->GetChild("Components"))
	{
		for (const auto& readerChild : componentsWriter->GetChilds())
		{
			try
			{
				std::string objectClassName = readerChild->GetName();
				_ASSERT(false == objectClassName.empty());
				ObjectClass objectClass = GetBaseManager().GetManager<IObjectsFactory>()->GetObjectClassByObjectClassName(objectClassName);
				const auto& component = GetComponent(objectClass);
				if (component != nullptr)
				{
					component->Load(readerChild);
					continue;
				}

				auto child = GetBaseManager().GetManager<IObjectsFactory>()->GetClassFactoryCast<IComponentFactory>()->LoadComponentXML(readerChild, *this);
				AddComponent(child->GetGUID().GetObjectClass(), child);
			}
			catch (const CException& e)
			{
				Log::Error("Unable to load node '%s' component '%s'.", GetName().c_str(), readerChild->GetName().c_str());
				Log::Error("---> '%s'", e.MessageCStr());
			}
		}
	}

	if (auto childsWriter = Reader->GetChild("Childs"))
	{
		for (const auto& ch : childsWriter->GetChilds())
		{
			try
			{
				auto child = GetBaseManager().GetManager<IObjectsFactory>()->GetClassFactoryCast<ISceneNodeFactory>()->LoadSceneNode3DXML(ch, GetScene());
				AddChild(child);
			}
			catch (const CException& e)
			{
				Log::Error("Unable to load node '%s' child '%s'.", GetName().c_str(), ch->GetName().c_str());
				Log::Error("---> '%s'", e.MessageCStr());
			}
		}
	}
}

void CSceneNode::Save(const std::shared_ptr<IXMLWriter>& Writer) const
{
	Object::Save(Writer);

	{
		auto propertiesWriter = Writer->CreateChild(GetProperties()->GetName());
		GetProperties()->Save(propertiesWriter);
	}

	const auto& components = GetComponents();
	if (false == components.empty())
	{
		auto componentsWriter = Writer->CreateChild("Components");
		for (const auto& ch : components)
		{
			try
			{
				auto childWriter = GetBaseManager().GetManager<IObjectsFactory>()->GetClassFactoryCast<IComponentFactory>()->SaveComponentXML(ch.second);
				componentsWriter->AddChild(childWriter);
			}
			catch (const CException& e)
			{
				Log::Error("Unable to save node '%s' component '%s'.", GetName().c_str(), ch.second->GetName().c_str());
				Log::Error("---> '%s'", e.MessageCStr());
			}
		}
	}

	const auto& childs = GetChilds();
	if (false == childs.empty())
	{
		auto childsWriter = Writer->CreateChild("Childs");
		for (const auto& ch : childs)
		{
			try
			{
				auto childWriter = GetBaseManager().GetManager<IObjectsFactory>()->GetClassFactoryCast<ISceneNodeFactory>()->SaveSceneNode3DXML(ch);
				childsWriter->AddChild(childWriter);
			}
			catch (const CException& e)
			{
				Log::Error("Unable to save node '%s' child '%s'.", GetName().c_str(), ch->GetName().c_str());
				Log::Error("---> '%s'", e.MessageCStr());
			}
		}
	}
}



//
// ISceneNodeInternal
//
void CSceneNode::AddChildInternal(std::shared_ptr<ISceneNode> childNode)
{
	if (childNode == nullptr)
		throw CException("Unable to add nullptr child node to '%s'.", GetName().c_str());

	if (auto currentChildParent = childNode->GetParent())
	{
		if (currentChildParent != shared_from_this())
		{
			std::dynamic_pointer_cast<CSceneNode>(currentChildParent)->RemoveChildPrivate(childNode);
		}
	}

	this->AddChildPrivate(childNode);
}

void CSceneNode::RemoveChildInternal(std::shared_ptr<ISceneNode> childNode)
{
	if (childNode == nullptr)
		throw CException("Unable to remove nullptr node from '%s'.", GetName().c_str());

	if (childNode->IsPersistance())
		throw CException("Unable to remove persistan node '%s' from '%s'.", childNode->GetName(), GetName().c_str());

	this->RemoveChildPrivate(childNode);
}

void CSceneNode::SetPersistanceInternal(bool Value)
{
	m_IsPersistance = Value;
}

void CSceneNode::RaiseOnParentChangedInternal()
{
	UpdateWorldTransform();

	for (auto c : m_Components)
		c.second->OnMessage(nullptr, UUID_OnParentChanged);
}



//
// Protected
//
IRenderDevice& CSceneNode::GetRenderDevice() const
{
	return GetScene().GetRenderDevice();
}



//
// Private
//
void CSceneNode::AddChildPrivate(std::shared_ptr<ISceneNode> ChildNode)
{
	_ASSERT(ChildNode != nullptr);

	const auto& iter = std::find(m_Children.begin(), m_Children.end(), ChildNode);
	if (iter != m_Children.end())
		throw CException("This parent already has this child.");

	m_Children.push_back(ChildNode);

	std::dynamic_pointer_cast<CSceneNode>(ChildNode)->SetParentPrivate(weak_from_this());
	ChildNode->SetName(ChildNode->GetName());
	std::dynamic_pointer_cast<ISceneNodeInternal>(ChildNode)->RaiseOnParentChangedInternal();
}

void CSceneNode::RemoveChildPrivate(std::shared_ptr<ISceneNode> ChildNode)
{
	const auto& childListIter = std::find(m_Children.begin(), m_Children.end(), ChildNode);
	if (childListIter == m_Children.end())
		throw CException("Can't remove child because don't found.");

	m_Children.erase(childListIter);

	std::dynamic_pointer_cast<CSceneNode>(ChildNode)->SetParentPrivate(std::weak_ptr<ISceneNode>());
	std::dynamic_pointer_cast<ISceneNodeInternal>(ChildNode)->RaiseOnParentChangedInternal();
}

void CSceneNode::SetParentPrivate(std::weak_ptr<ISceneNode> parentNode)
{
	m_ParentNode = parentNode;
}