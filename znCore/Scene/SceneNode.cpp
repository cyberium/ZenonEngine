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

	, m_PositionLocal(0.0f)
	, m_Rotation(glm::vec3(0.0f))
	, m_RotationQuaternion(glm::quat())
	, m_RotationKind(Euler)
	, m_Scale(1.0f)

	, m_LocalTransform(1.0f)
	, m_InverseLocalTransform(1.0f)	// This is the inverse of the local -> world transform.
	, m_WorldTransform(1.0f)
	, m_InverseWorldTransform(1.0f)
{
	m_PropertiesGroup = MakeShared(CPropertiesGroup, "Properties", "Some important scene node 3d properties.");


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
		nameProperty->SetValueSetter(std::bind(&Object::SetName, this, std::placeholders::_1));
		nameProperty->SetValueGetter(std::bind(&Object::GetName, this));
		GetProperties()->AddProperty(nameProperty);
	}

	// Transform properties
	{
		std::shared_ptr<IPropertiesGroup> propertiesGroup = MakeShared(CPropertiesGroup, "Transform", "Transorm of this 3D node. Like translation, rotation and scale.");

		m_PositionProperty = MakeShared(CPropertyWrappedVec3, "Translate", "Relative position to parent.", glm::vec3(0.0f));
		m_PositionProperty->SetValueSetter(std::bind(&CSceneNode::SetPosition, this, std::placeholders::_1));
		m_PositionProperty->SetValueGetter(std::bind(&CSceneNode::GetPosition, this));
		propertiesGroup->AddProperty(m_PositionProperty);

		/*std::shared_ptr<CPropertyWrappedVec3> translateAbsProperty = MakeShared(CPropertyWrappedVec3, "TranslateAbsolute", "Absolute position in world.", glm::vec3(0.0f));
		translateAbsProperty->SetSyntetic(true);
		translateAbsProperty->SetValueSetter(std::bind(&CSceneNode::SetPosition, this, std::placeholders::_1));
		translateAbsProperty->SetValueGetter(std::bind(&CSceneNode::GetPosition, this));
		propertiesGroup->AddProperty(translateAbsProperty);*/

		std::shared_ptr<CPropertyWrappedVec3> rotationProperty = MakeShared(CPropertyWrappedVec3, "Rotate", "Rotation of this node. Relative to parent.", glm::vec3(0.0f));
		rotationProperty->SetValueSetter(std::bind(&CSceneNode::SetRotationEuler, this, std::placeholders::_1));
		rotationProperty->SetValueGetter(std::bind(&CSceneNode::GetRotationEuler, this));
		propertiesGroup->AddProperty(rotationProperty);

		std::shared_ptr<CPropertyWrappedVec3> scaleProperty = MakeShared(CPropertyWrappedVec3, "Scale", "Scale of this node. Relative to parent.", glm::vec3(1.0f));
		scaleProperty->SetValueSetter(std::bind(&CSceneNode::SetScale, this, std::placeholders::_1));
		scaleProperty->SetValueGetter(std::bind(&CSceneNode::GetScale, this));
		propertiesGroup->AddProperty(scaleProperty);

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

std::shared_ptr<IPropertiesGroup> CSceneNode::GetProperties() const
{
	return m_PropertiesGroup;
}

IScene& CSceneNode::GetScene() const
{
	return m_Scene;
}



//
// Transform functional
//
void CSceneNode::SetPosition(glm::vec3 Position)
{
	m_PositionLocal = glm::inverse(GetParentWorldTransform()) * glm::vec4(Position, 1.0f);
	UpdateLocalTransform();
}

glm::vec3 CSceneNode::GetPosition() const
{
	return GetParentWorldTransform() * glm::vec4(GetLocalPosition(), 1.0f);
}

void CSceneNode::SetLocalPosition(glm::vec3 LocalPosition)
{
	m_PositionLocal = LocalPosition;
	m_PositionProperty->RaiseValueChangedCallback();
	UpdateLocalTransform();
}

glm::vec3 CSceneNode::GetLocalPosition() const
{
	return m_PositionLocal;
}

void CSceneNode::SetDirection(glm::vec3 Direction)
{
	throw CException("Not implemented.");
}

glm::vec3 CSceneNode::GetDirection() const
{
	throw CException("Not implemented.");
}

void CSceneNode::SetRotationEuler(glm::vec3 Rotation)
{
	m_Rotation = Rotation;
	m_RotationKind = Euler;
	UpdateLocalTransform();
}

glm::vec3 CSceneNode::GetRotationEuler() const
{
	return m_Rotation;
}

void CSceneNode::SetRotationQuaternion(glm::quat Rotation)
{
	m_RotationQuaternion = Rotation;
	m_RotationKind = Quaternion;
	UpdateLocalTransform();
}

glm::quat CSceneNode::GetRotationQuaternion() const
{
	return m_RotationQuaternion;
}

ISceneNode::ERotationKind CSceneNode::GetRotationKind() const
{
	return m_RotationKind;
}

void CSceneNode::SetScale(glm::vec3 Scale)
{
	m_Scale = Scale;
	UpdateLocalTransform();
}

glm::vec3 CSceneNode::GetScale() const
{
	return m_Scale;
}

void CSceneNode::SetLocalTransform(const glm::mat4& localTransform)
{
	m_LocalTransform = localTransform;
	m_InverseLocalTransform = glm::inverse(localTransform);

	UpdateWorldTransform();
}

glm::mat4 CSceneNode::GetLocalTransform() const
{
	return m_LocalTransform;
}

glm::mat4 CSceneNode::GetInverseLocalTransform() const
{
	return m_InverseLocalTransform;
}

void CSceneNode::SetWorldTransform(const glm::mat4& worldTransform)
{
	SetLocalTransform(glm::inverse(GetParentWorldTransform()) * worldTransform);
}

glm::mat4 CSceneNode::GetWorldTransfom() const
{
	return m_WorldTransform;
}

glm::mat4 CSceneNode::GetInverseWorldTransform() const
{
	return m_InverseWorldTransform;
}

glm::mat4 CSceneNode::GetParentWorldTransform() const
{
	glm::mat4 parentTransform(1.0f);
	if (auto parent = GetParent())
		parentTransform = parent->GetWorldTransfom();
	return parentTransform;
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
	m_Components[ComponentID] = Component;
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
	AddComponentT(GetBaseManager().GetManager<IObjectsFactory>()->GetClassFactoryCast<IComponentFactory>()->CreateComponentT<IModelsComponent3D>(cSceneNodeModelsComponent, *this));
	AddComponentT(GetBaseManager().GetManager<IObjectsFactory>()->GetClassFactoryCast<IComponentFactory>()->CreateComponentT<IColliderComponent3D>(cSceneNodeColliderComponent, *this));
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

	destCast->m_PositionLocal = m_PositionLocal;
	destCast->m_Rotation = m_Rotation;
	destCast->m_RotationQuaternion = m_RotationQuaternion;
	destCast->m_RotationKind = m_RotationKind;
	destCast->m_Scale = m_Scale;
	destCast->m_LocalTransform = m_LocalTransform;
	destCast->m_InverseLocalTransform = m_InverseLocalTransform;
	destCast->m_WorldTransform = m_WorldTransform;
	destCast->m_InverseWorldTransform = m_InverseWorldTransform;

	destCast->m_PropertiesGroup;// TODO

	for (const auto& c : GetComponents())
	{
		const auto& compInOther = destCast->m_Components.find(c.first);
		_ASSERT(compInOther != destCast->m_Components.end());
		c.second->CopyTo(compInOther->second);
		Log::Info("SceneNode::CopyTo: Component '%s' of node '%s' copied.", c.second->GetName().c_str(), GetName().c_str());
	}

	for (const auto& ch : GetChilds())
	{
		std::shared_ptr<ISceneNode> childCopy = GetBaseManager().GetManager<IObjectsFactory>()->GetClassFactoryCast<ISceneNodeFactory>()->CreateSceneNode3D(ch->GetClass(), destCast->GetScene());
		ch->CopyTo(childCopy);
		destCast->AddChild(childCopy);
		Log::Info("SceneNode::CopyTo: Child '%s' of parent '%s' copied.", ch->GetName().c_str(), GetName().c_str());
	}
}

void CSceneNode::Load(const std::shared_ptr<IXMLReader>& Reader)
{
	Object::Load(Reader);

	DoLoadProperties(Reader);

	if (Reader->IsChildExists("Components"))
	{
		auto componentsWriter = Reader->GetChild("Components");
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

	if (Reader->IsChildExists("Childs"))
	{
		auto childsWriter = Reader->GetChild("Childs");
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

	DoSaveProperties(Writer);

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
		throw CException(L"Unable to add nullptr child node to '%s'.", GetName().c_str());

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
glm::mat4 CSceneNode::CalculateLocalTransform() const
{
	glm::mat4 localTransform(1.0f);

	// Position
	localTransform = glm::translate(localTransform, m_PositionLocal);
	
	// Rotation
	if (m_RotationKind == Euler)
	{
		localTransform *= glm::eulerAngleXYZ(m_Rotation.x, m_Rotation.y, m_Rotation.z);
	}
	else if (m_RotationKind == Quaternion)
	{
		localTransform *= glm::toMat4(m_RotationQuaternion);
	}
	else if (m_RotationKind == Direction)
	{
		throw CException("Not implemented.");
	}
	else
		throw CException("Unknown '%d' rotation kind.", m_RotationKind);

	// Scale+
	localTransform = glm::scale(localTransform, m_Scale);

	return localTransform;
}




//
// Protected
//
void CSceneNode::UpdateLocalTransform()
{
	SetLocalTransform(CalculateLocalTransform());
	RaiseComponentMessage(nullptr, UUID_OnLocalTransformChanged);
}

void CSceneNode::UpdateWorldTransform()
{
	m_WorldTransform = GetParentWorldTransform() * m_LocalTransform;
	m_InverseWorldTransform = glm::inverse(m_WorldTransform);
	
	// After world updated, we can update all childs
	for (const auto& it : GetChilds())
		std::dynamic_pointer_cast<CSceneNode>(it)->UpdateWorldTransform();

	RaiseComponentMessage(nullptr, UUID_OnWorldTransformChanged);
}

void CSceneNode::DoLoadProperties(const std::shared_ptr<IXMLReader>& Reader) const
{
	auto reader = Reader->GetChild(GetProperties()->GetName());
	if (reader)
		GetProperties()->Load(reader);
}

void CSceneNode::DoSaveProperties(const std::shared_ptr<IXMLWriter>& Writer) const
{
	CXMLManager xml(GetBaseManager());
	auto propertiesWriter = xml.CreateWriter(GetProperties()->GetName());
	GetProperties()->Save(propertiesWriter);
	
	Writer->AddChild(propertiesWriter);
}

IBaseManager& CSceneNode::GetBaseManager() const
{
	return GetScene().GetBaseManager();
}

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
		throw CException(L"This parent already has this child.");

	m_Children.push_back(ChildNode);

	std::dynamic_pointer_cast<CSceneNode>(ChildNode)->SetParentPrivate(weak_from_this());
	ChildNode->SetName(ChildNode->GetName());
	std::dynamic_pointer_cast<ISceneNodeInternal>(ChildNode)->RaiseOnParentChangedInternal();
}

void CSceneNode::RemoveChildPrivate(std::shared_ptr<ISceneNode> ChildNode)
{
	const auto& childListIter = std::find(m_Children.begin(), m_Children.end(), ChildNode);
	if (childListIter == m_Children.end())
		throw CException(L"Can't remove child because don't found.");

	m_Children.erase(childListIter);

	std::dynamic_pointer_cast<CSceneNode>(ChildNode)->SetParentPrivate(std::weak_ptr<ISceneNode>());
	std::dynamic_pointer_cast<ISceneNodeInternal>(ChildNode)->RaiseOnParentChangedInternal();
}

void CSceneNode::SetParentPrivate(std::weak_ptr<ISceneNode> parentNode)
{
	m_ParentNode = parentNode;
}