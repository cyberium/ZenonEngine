#include "stdafx.h"

// General
#include "SceneNode.h"

//
// Global transform
//
void CSceneNode::SetPosition(glm::vec3 Position)
{
	const glm::mat4 localPositionMatrix = glm::inverse(GetParentWorldTransform()) * glm::translate(Position);
	SetLocalPosition(DecomposePositionMatrix(localPositionMatrix));
}

glm::vec3 CSceneNode::GetPosition() const
{
	const glm::mat4 globalPositionMatrix = GetParentWorldTransform() * glm::translate(GetLocalPosition());
	return DecomposePositionMatrix(globalPositionMatrix);
}

// Rotation

void CSceneNode::SetRotationQuaternion(glm::quat Rotation)
{
	const glm::mat4 localRotationMatrix = glm::inverse(GetParentWorldTransform()) * glm::toMat4(Rotation);
	SetLocalRotationQuaternion(DecomposeRotationMatrix(localRotationMatrix));
}

glm::quat CSceneNode::GetRotationQuaternion() const
{
	const glm::mat4 globalRotationMatrix = GetParentWorldTransform() * glm::toMat4(GetLocalRotationQuaternion());
	return DecomposeRotationMatrix(globalRotationMatrix);
}

// Scale

void CSceneNode::SetScale(glm::vec3 Scale)
{
	const glm::mat4 localScaleMatrix = glm::inverse(GetParentWorldTransform()) * glm::scale(Scale);
	SetLocalScale(DecomposeScaleMatrix(localScaleMatrix));
}

glm::vec3 CSceneNode::GetScale() const
{
	const glm::mat4 globalScaleMatrix = GetParentWorldTransform() * glm::scale(GetLocalScale());
	return DecomposeScaleMatrix(globalScaleMatrix);
}

// Matrix

void CSceneNode::SetWorldTransform(const glm::mat4& worldTransform)
{
	const glm::mat4 localTransform = glm::inverse(GetParentWorldTransform()) * worldTransform;
	SetLocalTransform(localTransform);
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
// Local transform
//
void CSceneNode::SetLocalPosition(glm::vec3 LocalPosition)
{
	m_LocalPosition = LocalPosition;
	m_LocalPositionProperty->RaiseValueChangedCallback();
	UpdateLocalTransform();
}

glm::vec3 CSceneNode::GetLocalPosition() const
{
	return m_LocalPosition;
}

void CSceneNode::SetLocalRotationEuler(glm::vec3 Rotation)
{
	// Rotation (eulerAngles)
	m_RotationEulerAngles = Rotation;
	m_RotationEulerAnglesProperty->RaiseValueChangedCallback();

	// Rotation (quaternion)
	glm::vec3 rotationEulerRadians = glm::radians(Rotation);
	m_RotationQuaternion = glm::toQuat(glm::eulerAngleXYZ(rotationEulerRadians.x, rotationEulerRadians.y, rotationEulerRadians.z));
	//m_RotationQuaternionProperty->RaiseValueChangedCallback();

	// Rotation (direction)
	m_RotationDirection.x = glm::cos(rotationEulerRadians.x) * glm::cos(rotationEulerRadians.y); // y
	m_RotationDirection.y = glm::sin(rotationEulerRadians.y);                                    // z
	m_RotationDirection.z = glm::sin(rotationEulerRadians.x) * glm::cos(rotationEulerRadians.y); // x
	m_RotationDirection = glm::normalize(m_RotationDirection);
	//m_RotationDirectionProperty->RaiseValueChangedCallback();

	UpdateLocalTransform();
}

glm::vec3 CSceneNode::GetLocalRotationEuler() const
{
	return m_RotationEulerAngles;
}

void CSceneNode::SetLocalRotationQuaternion(glm::quat Rotation)
{
	// Rotation (eulerAngles)
	//glm::quat conjurated = glm::conjugate(Rotation);
	glm::vec3 rotationEuler = glm::eulerAngles(Rotation);
	m_RotationEulerAngles = glm::degrees(rotationEuler);
	m_RotationEulerAnglesProperty->RaiseValueChangedCallback();

	// Rotation (quaternion)
	m_RotationQuaternion = Rotation;
	//m_RotationQuaternionProperty->RaiseValueChangedCallback();

	// Rotation (direction)
	m_RotationDirection.x = glm::cos(rotationEuler.x) * glm::cos(rotationEuler.y); // y
	m_RotationDirection.y = glm::sin(rotationEuler.y);                             // z
	m_RotationDirection.z = glm::sin(rotationEuler.x) * glm::cos(rotationEuler.y); // x
	m_RotationDirection = glm::normalize(m_RotationDirection);
	//m_RotationDirectionProperty->RaiseValueChangedCallback();

	UpdateLocalTransform();
}

glm::quat CSceneNode::GetLocalRotationQuaternion() const
{
	return m_RotationQuaternion;
}

void CSceneNode::SetLocalRotationDirection(glm::vec3 Direction)
{
	glm::vec3 normalizedDirection = glm::normalize(Direction);

	glm::quat lootAtQuat = glm::conjugate(glm::toQuat(glm::lookAt(glm::vec3(0.0f), normalizedDirection, glm::vec3(0.0f, 1.0f, 0.0f))));

	// Rotation (eulerAngles)
	glm::vec3 rotationEuler = glm::eulerAngles(lootAtQuat);
	m_RotationEulerAngles = glm::degrees(rotationEuler);
	m_RotationEulerAnglesProperty->RaiseValueChangedCallback();

	// Rotation (quaternion)
	m_RotationQuaternion = lootAtQuat;
	//m_RotationQuaternionProperty->RaiseValueChangedCallback();

	// Rotation (direction)
	m_RotationDirection.x = glm::cos(rotationEuler.x) * glm::cos(rotationEuler.y); // y
	m_RotationDirection.y = glm::sin(rotationEuler.y);                             // z
	m_RotationDirection.z = glm::sin(rotationEuler.x) * glm::cos(rotationEuler.y); // x
	m_RotationDirection = glm::normalize(m_RotationDirection);
	//m_RotationDirectionProperty->RaiseValueChangedCallback();

	UpdateLocalTransform();
}

glm::vec3 CSceneNode::GetLocalRotationDirection() const
{
	return m_RotationDirection;
}

void CSceneNode::SetLocalScale(glm::vec3 Scale)
{
	m_Scale = Scale;
	m_ScaleProperty->RaiseValueChangedCallback();
	UpdateLocalTransform();
}

glm::vec3 CSceneNode::GetLocalScale() const
{
	return m_Scale;
}

void CSceneNode::SetLocalTransform(const glm::mat4& localTransform)
{
	m_LocalTransform = localTransform;
	m_InverseLocalTransform = glm::inverse(localTransform);

	UpdateWorldTransform();

	glm::vec3 scale;
	glm::quat rotationQuat;
	glm::vec3 position;
	glm::vec3 skew;
	glm::vec4 perspective;
	if (false == glm::decompose(m_LocalTransform, scale, rotationQuat, position, skew, perspective))
		throw CException("Unable to decompose local transform.");

	// Position
	m_LocalPosition = position;
	m_LocalPositionProperty->RaiseValueChangedCallback();

	// Rotation (eulerAngles)
	glm::vec3 rotationEuler = glm::eulerAngles(rotationQuat);
	m_RotationEulerAngles = glm::degrees(rotationEuler);
	m_RotationEulerAnglesProperty->RaiseValueChangedCallback();

	// Rotation (quaternion)
	m_RotationQuaternion = rotationQuat; // glm::eulerAngles(glm::conjugate(rotation)); //  glm::vec3(glm::pitch(rotation), -glm::yaw(rotation), glm::roll(rotation));
	//m_RotationQuaternionProperty->RaiseValueChangedCallback();

	// Rotation (direction)
	//m_RotationDirection.x = glm::cos(rotationEuler.x) * glm::cos(rotationEuler.y); // y
	//m_RotationDirection.y = glm::sin(rotationEuler.y);                             // z
	//m_RotationDirection.z = glm::sin(rotationEuler.x) * glm::cos(rotationEuler.y); // x
	//m_RotationDirection = glm::normalize(m_RotationDirection);
	//m_RotationDirectionProperty->RaiseValueChangedCallback();

	// Scale
	m_Scale = scale;
	m_ScaleProperty->RaiseValueChangedCallback();
}

glm::mat4 CSceneNode::GetLocalTransform() const
{
	return m_LocalTransform;
}

glm::mat4 CSceneNode::GetInverseLocalTransform() const
{
	return m_InverseLocalTransform;
}



//
// Protected
// 
glm::mat4 CSceneNode::CalculateLocalTransform() const
{
	glm::mat4 localTransform(1.0f);

	// Position
	localTransform = glm::translate(localTransform, m_LocalPosition);

	// Rotation
	//if (m_RotationKind == Euler)
	//{
	//	glm::vec3 rotationEulerRadians(glm::radians(m_RotationEulerAngles));
	//	localTransform *= glm::eulerAngleXYZ(rotationEulerRadians.x, rotationEulerRadians.y, rotationEulerRadians.z);
	//}
	//else if (m_RotationKind == Quaternion)
	//{
		localTransform *= glm::toMat4(m_RotationQuaternion);
	//}
	//else if (m_RotationKind == Direction)
	//{
	//	throw CException("Not implemented.");
	//}
	//else
	//	throw CException("Unknown '%d' rotation kind.", m_RotationKind);

	// Scale+
	localTransform = glm::scale(localTransform, m_Scale);

	return localTransform;
}

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