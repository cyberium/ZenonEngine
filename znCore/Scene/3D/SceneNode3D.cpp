#include "stdafx.h"

// General
#include "SceneNode3D.h"

// Additional
#include "Scene/Properties.h"

#include "MeshComponent3D.h"
#include "ColliderComponent3D.h"
#include "LightComponent3D.h"

SceneNode3D::SceneNode3D()
	: SceneNodeBase()
	, m_Translate(0.0f)
	, m_Rotate(vec3(0.0f))
	, m_RotateQuat(quat())
	, m_IsRotateQuat(false)
	, m_Scale(1.0f)
{
	SetName("SceneNode3D");

	std::shared_ptr<IPropertiesGroup> propertiesGroup = std::make_shared<CPropertiesGroup>("Transform", "Transorm of this 3D node. Like translation, rotation and scale.");

	std::shared_ptr<CPropertyWrapped<glm::vec3>> translateProperty = std::make_shared<CPropertyWrapped<glm::vec3>>("Translate", "Position of this node in world. Relative to parent.");
	translateProperty->SetValueSetter(std::bind(&SceneNode3D::SetTranslate, this, std::placeholders::_1));
	translateProperty->SetValueGetter(std::bind(&SceneNode3D::GetTranslation, this));
	propertiesGroup->AddProperty(translateProperty);

	std::shared_ptr<CPropertyWrapped<glm::vec3>> rotationProperty = std::make_shared<CPropertyWrapped<glm::vec3>>("Rotate", "Rotation of this node. Relative to parent.");
	rotationProperty->SetValueSetter(std::bind(&SceneNode3D::SetRotation, this, std::placeholders::_1));
	rotationProperty->SetValueGetter(std::bind(&SceneNode3D::GetRotation, this));
	propertiesGroup->AddProperty(rotationProperty);

	std::shared_ptr<CPropertyWrapped<glm::vec3>> scaleProperty = std::make_shared<CPropertyWrapped<glm::vec3>>("Scale", "Scale of this node. Relative to parent.");
	scaleProperty->SetValueSetter(std::bind(&SceneNode3D::SetScale, this, std::placeholders::_1));
	scaleProperty->SetValueGetter(std::bind(&SceneNode3D::GetScale, this));
	propertiesGroup->AddProperty(scaleProperty);

	GetProperties()->AddProperty(propertiesGroup);
}

SceneNode3D::~SceneNode3D()
{}


std::shared_ptr<SceneNode3D> SceneNode3D::shared_from_this()
{
    return std::dynamic_pointer_cast<SceneNode3D>(SceneNodeBase::shared_from_this());
}

std::weak_ptr<SceneNode3D> SceneNode3D::weak_from_this()
{
    return std::weak_ptr<SceneNode3D>(shared_from_this());
}



//
// SceneNodeBase
//
void SceneNode3D::SetLocalTransform(cmat4 localTransform)
{
	SceneNodeBase::SetLocalTransform(localTransform);

	/*

	glm::vec3 scale;
	glm::quat rotation;
	glm::vec3 translation;
	glm::vec3 skew;
	glm::vec4 perspective;
	glm::decompose(m_LocalTransform, scale, rotation, translation, skew, perspective);

	m_Translate = translation;
	m_Scale = scale;

	glm::vec3 eulerAngles = glm::degrees(glm::eulerAngles(rotation));

	glm::vec3 direction = glm::vec3();
	direction.x = glm::cos(glm::radians(eulerAngles.x)) * glm::cos(glm::radians(direction.y)); // y
	direction.y = glm::sin(glm::radians(direction.y));                              // z
	direction.z = glm::sin(glm::radians(eulerAngles.x)) * glm::cos(glm::radians(direction.y)); // x

	*/

	//printf("Converted: %f %f %f\n", eulerAngles.x, eulerAngles.y, eulerAngles.z);
}



//
// ISceneNode3D
//
void SceneNode3D::SetTranslate(cvec3 _translate)
{
	m_Translate = _translate;
	UpdateLocalTransform();
}
void SceneNode3D::AddTranslate(vec3 Translate)
{
	SetTranslate(GetTranslation() + Translate);
}
cvec3 SceneNode3D::GetTranslation() const
{
	return m_Translate;
}

void SceneNode3D::SetRotation(cvec3 _rotate)
{
	m_Rotate = _rotate;
	UpdateLocalTransform();
}
cvec3 SceneNode3D::GetRotation() const
{
	return m_Rotate;
}

void SceneNode3D::SetRotationQuaternion(cquat _rotate)
{
	m_RotateQuat = _rotate;
	m_IsRotateQuat = true;
	UpdateLocalTransform();
}
cquat SceneNode3D::GetRotationQuaternion() const
{
	return m_RotateQuat;
}

void SceneNode3D::SetScale(cvec3 _scale)
{
	m_Scale = _scale;
	UpdateLocalTransform();
}
cvec3 SceneNode3D::GetScale() const
{
	return m_Scale;
}



//
// Components engine
//
void SceneNode3D::RegisterComponents()
{
	AddComponent<CLightComponent3D>(std::make_shared<CLightComponent3D>(shared_from_this()));
    SetMeshComponent(AddComponent(std::make_shared<CMeshComponent3D>(shared_from_this())));
    SetColliderComponent(AddComponent(std::make_shared<CColliderComponent3D>(shared_from_this())));
}

bool SceneNode3D::Load(std::shared_ptr<IXMLReader> Reader)
{
	return false;
}

bool SceneNode3D::Save(std::shared_ptr<IXMLWriter> Writer)
{
	/*

	std::shared_ptr<IXMLWriter> componentWriter = Writer->CreateChild("TransformComponent3D");

	std::shared_ptr<IXMLWriter> translate = componentWriter->CreateChild("Translate");
	translate->AddAttribute("X", std::to_string(GetTranslation().x));
	translate->AddAttribute("Y", std::to_string(GetTranslation().y));
	translate->AddAttribute("Z", std::to_string(GetTranslation().z));

	std::shared_ptr<IXMLWriter> rotate = componentWriter->CreateChild("Rotate");
	rotate->AddAttribute("X", std::to_string(GetRotation().x));
	rotate->AddAttribute("Y", std::to_string(GetRotation().y));
	rotate->AddAttribute("Z", std::to_string(GetRotation().z));

	std::shared_ptr<IXMLWriter> scale = componentWriter->CreateChild("Scale");
	scale->AddAttribute("X", std::to_string(GetScale().x));
	scale->AddAttribute("Y", std::to_string(GetScale().y));
	scale->AddAttribute("Z", std::to_string(GetScale().z));
	
	*/

	return SceneNodeBase::Save(Writer);
}

bool SceneNode3D::Accept(IVisitor* visitor)
{
	bool visitResult = visitor->Visit3D(this);

	//if (visitResult && !GetComponent<IColliderComponent3D>()->CheckFrustum(((AbstractPass*)visitor)->GetRenderEventArgs()->Camera))
	//{
	//	return false;
	//}

	if (visitResult)
	{
		const auto& components = GetComponents();
		std::for_each(components.begin(), components.end(), [&visitor](const std::pair<GUID, std::shared_ptr<ISceneNodeComponent>>& Component)
		{
			Component.second->Accept(visitor);
		});
	}

	// Now visit children
	const auto& childs = GetChilds();
	std::for_each(childs.begin(), childs.end(), [&visitor](const std::shared_ptr<ISceneNode>& Child)
	{
#ifdef LOADER_ENABLED
		std::shared_ptr<ILoadable> loadable = std::dynamic_pointer_cast<ILoadable>(child);
		if (loadable != nullptr && !loadable->isLoaded())
			continue;
#endif
		Child->Accept(visitor);
	});

	return visitResult;
}



//
// Protected
//
void SceneNode3D::UpdateLocalTransform()
{
	glm::mat4 localTransform = glm::mat4(1.0f);

	localTransform = glm::translate(localTransform, m_Translate);
	if (m_IsRotateQuat)
	{
		localTransform *= glm::toMat4(m_RotateQuat);
	}
	else
	{
		localTransform = glm::rotate(localTransform, m_Rotate.x, glm::vec3(1, 0, 0));
		localTransform = glm::rotate(localTransform, m_Rotate.y, glm::vec3(0, 1, 0));
		localTransform = glm::rotate(localTransform, m_Rotate.z, glm::vec3(0, 0, 1));
	}
	localTransform = glm::scale(localTransform, m_Scale);

	SetLocalTransform(localTransform);

	RaiseComponentMessage(nullptr, UUID_OnTransformChanged);
}

void SceneNode3D::UpdateWorldTransform()
{
	m_WorldTransform = GetParentWorldTransform() * m_LocalTransform;
	m_InverseWorldTransform = glm::inverse(m_WorldTransform);
	
	RaiseComponentMessage(nullptr, UUID_OnTransformChanged);
}

void SceneNode3D::SetMeshComponent(std::shared_ptr<IMeshComponent3D> MeshComponent)
{
	m_Components_Mesh = MeshComponent;
}

void SceneNode3D::SetColliderComponent(std::shared_ptr<IColliderComponent3D> ColliderComponent)
{
    m_Components_Collider = ColliderComponent;
}
