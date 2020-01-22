#include "stdafx.h"

// General
#include "CameraComponent3D.h"

CCameraComponent3D::CCameraComponent3D(std::shared_ptr<ISceneNode3D> OwnerNode)
    : CComponentBase(OwnerNode)
	, m_RightDirection(0)
	, m_UpDirection(0)
	, m_Yaw_X(0.0f)
	, m_Pitch_Y(0.0f)
	, m_View_Dirty(true)
	, m_ProjectionView_Dirty(true)
{
	m_Yaw_XProperty = std::make_shared<CPropertyWrapped<float>>("Yaw", "Rotation around the Y axis (in degrees).");
	m_Yaw_XProperty->SetValueSetter(std::bind(&CCameraComponent3D::SetYaw, this, std::placeholders::_1));
	m_Yaw_XProperty->SetValueGetter(std::bind(&CCameraComponent3D::GetYaw, this));
	GetPropertiesGroup()->AddProperty(m_Yaw_XProperty);

	m_Pitch_YProperty = std::make_shared<CPropertyWrapped<float>>("Pitch", "Rotation around the X and Y axis (in degrees).");
	m_Pitch_YProperty->SetValueSetter(std::bind(&CCameraComponent3D::SetPitch, this, std::placeholders::_1));
	m_Pitch_YProperty->SetValueGetter(std::bind(&CCameraComponent3D::GetPitch, this));
	GetPropertiesGroup()->AddProperty(m_Pitch_YProperty);
}

CCameraComponent3D::~CCameraComponent3D()
{

}



//
// ICameraComponent3D
//
void CCameraComponent3D::DoMoveFront(float Value)
{
	std::shared_ptr<ISceneNode3D> sceneNode3D = std::dynamic_pointer_cast<ISceneNode3D>(GetOwnerNode());
	_ASSERT(sceneNode3D != nullptr);
	sceneNode3D->AddTranslate(sceneNode3D->GetRotation() * Value);
}

void CCameraComponent3D::DoMoveBack(float Value)
{
	std::shared_ptr<ISceneNode3D> sceneNode3D = std::dynamic_pointer_cast<ISceneNode3D>(GetOwnerNode());
	_ASSERT(sceneNode3D != nullptr);
	sceneNode3D->AddTranslate(-(sceneNode3D->GetRotation() * Value));
}

void CCameraComponent3D::DoMoveLeft(float Value)
{
	std::shared_ptr<ISceneNode3D> sceneNode3D = std::dynamic_pointer_cast<ISceneNode3D>(GetOwnerNode());
	_ASSERT(sceneNode3D != nullptr);
	sceneNode3D->AddTranslate(-(m_RightDirection * Value));
}

void CCameraComponent3D::DoMoveRight(float Value)
{
	std::shared_ptr<ISceneNode3D> sceneNode3D = std::dynamic_pointer_cast<ISceneNode3D>(GetOwnerNode());
	_ASSERT(sceneNode3D != nullptr);
	sceneNode3D->AddTranslate(m_RightDirection * Value);
}

void CCameraComponent3D::SetTranslation(glm::vec3 Translation) const
{
	std::shared_ptr<ISceneNode3D> sceneNode3D = std::dynamic_pointer_cast<ISceneNode3D>(GetOwnerNode());
	_ASSERT(sceneNode3D != nullptr);
	sceneNode3D->SetTranslate(Translation);
}

glm::vec3 CCameraComponent3D::GetTranslation() const
{
	std::shared_ptr<ISceneNode3D> sceneNode3D = std::dynamic_pointer_cast<ISceneNode3D>(GetOwnerNode());
	_ASSERT(sceneNode3D != nullptr);
	return sceneNode3D->GetTranslation();
}

void CCameraComponent3D::SetDirection(glm::vec3 Direction) const
{
	std::shared_ptr<ISceneNode3D> sceneNode3D = std::dynamic_pointer_cast<ISceneNode3D>(GetOwnerNode());
	_ASSERT(sceneNode3D != nullptr);
	sceneNode3D->SetRotation(Direction);
}

glm::vec3 CCameraComponent3D::GetDirection() const
{
	std::shared_ptr<ISceneNode3D> sceneNode3D = std::dynamic_pointer_cast<ISceneNode3D>(GetOwnerNode());
	_ASSERT(sceneNode3D != nullptr);
	return sceneNode3D->GetRotation();
}

void CCameraComponent3D::SetYaw(float Yaw)
{
	m_Yaw_X = Yaw;
	if (m_Yaw_X > 360.0f)
		m_Yaw_X -= 360.0f;
	if (m_Yaw_X < 0.0f)
		m_Yaw_X += 360.0f;

	m_Yaw_XProperty->RaiseValueChangedCallback();

	std::shared_ptr<ISceneNode3D> sceneNode3D = std::dynamic_pointer_cast<ISceneNode3D>(GetOwnerNode());
	_ASSERT(sceneNode3D != nullptr);
	sceneNode3D->SetRotation(EulerAnglesToDirectionVector(m_Yaw_X, m_Pitch_Y));

	m_View_Dirty = true;
}

void CCameraComponent3D::AddYaw(float Yaw)
{
	SetYaw(m_Yaw_X + Yaw);
}

float CCameraComponent3D::GetYaw() const
{
	return m_Yaw_X;
}

void CCameraComponent3D::SetPitch(float Pitch)
{
	m_Pitch_Y = Pitch;
	if (m_Pitch_Y > cPitchUpperBorder)
		m_Pitch_Y = cPitchUpperBorder;
	if (m_Pitch_Y < cPitchBottomBorder)
		m_Pitch_Y = cPitchBottomBorder;

	m_Pitch_YProperty->RaiseValueChangedCallback();

	std::shared_ptr<ISceneNode3D> sceneNode3D = std::dynamic_pointer_cast<ISceneNode3D>(GetOwnerNode());
	_ASSERT(sceneNode3D != nullptr);
	sceneNode3D->SetRotation(EulerAnglesToDirectionVector(m_Yaw_X, m_Pitch_Y));

	m_View_Dirty = true;
}

void CCameraComponent3D::AddPitch(float Pitch)
{
	SetPitch(m_Pitch_Y + Pitch);
}

float CCameraComponent3D::GetPitch() const
{
	return m_Pitch_Y;
}

void CCameraComponent3D::SetPerspectiveProjection(EPerspectiveProjectionHand PerspectiveProjectionHand, float fovy, float aspect, float zNear, float zFar)
{
	m_Perspective_FOV = fovy;
	m_Perspective_Aspect = aspect;
	m_Near = zNear;
	m_Far = zFar;

	if (PerspectiveProjectionHand == EPerspectiveProjectionHand::Right)
	{
		m_Projection = glm::perspectiveRH_NO(glm::radians(fovy), aspect, zNear, zFar);
	}
	else
	{
		m_Projection = glm::perspectiveLH_NO(glm::radians(fovy), aspect, zNear, zFar);
	}

	m_Inverse_Projection = glm::inverse(m_Projection);
}

void CCameraComponent3D::SetOrthographicProjection(float left, float right, float top, float bottom, float zNear, float zFar)
{
	m_Orthographic_Left = left;
	m_Orthographic_Right = right;
	m_Orthographic_Top = top;
	m_Orthographic_Bottom = bottom;
	m_Near = zNear;
	m_Far = zFar;

	m_Projection = glm::ortho<float>(m_Orthographic_Left, m_Orthographic_Right, m_Orthographic_Top, m_Orthographic_Bottom, m_Near, m_Far);
	m_Inverse_Projection = glm::inverse(m_Projection);
}

const glm::mat4& CCameraComponent3D::GetViewMatrix() const
{
	const_cast<CCameraComponent3D*>(this)->UpdateView();
	return m_View;
}

const glm::mat4 & CCameraComponent3D::GetInverseViewMatrix() const
{
	const_cast<CCameraComponent3D*>(this)->UpdateView();
	return m_Inverse_View;
}

const glm::mat4& CCameraComponent3D::GetProjectionMatrix() const
{
	return m_Projection;
}

const glm::mat4& CCameraComponent3D::GetInverseProjectionMatrix() const
{
	return m_Inverse_Projection;
}

const glm::mat4 & CCameraComponent3D::GetProjectionViewMatrix() const
{
	const_cast<CCameraComponent3D*>(this)->UpdateProjectionView();
	return m_ProjectionView;
}

const glm::mat4 & CCameraComponent3D::GetInverseProjectionViewMatrix() const
{
	const_cast<CCameraComponent3D*>(this)->UpdateProjectionView();
	return m_Inverse_ProjectionView;
}



//
// ISceneNodeComponent
//
void CCameraComponent3D::OnMessage(ISceneNodeComponent* Component, ComponentMessageType Message)
{
	if (Component == nullptr && Message == UUID_OnTransformChanged)
	{
		m_View_Dirty = true;
	}
}





//
// Protected
//
glm::vec3 CCameraComponent3D::EulerAnglesToDirectionVector(float Yaw, float Pitch)
{
	// Calculate the new Front vector
	glm::vec3 direction = { 0.0f, 0.0f, 0.0f };
	direction.x = glm::cos(glm::radians(Yaw)) * glm::cos(glm::radians(Pitch)); // y
	direction.y = glm::sin(glm::radians(Pitch));                               // z
	direction.z = glm::sin(glm::radians(Yaw)) * glm::cos(glm::radians(Pitch)); // x
	direction = glm::normalize(direction);

	// Also re-calculate the Right and Up vector
	m_RightDirection = glm::normalize(glm::cross(direction, vec3(0.0f, 1.0f, 0.0f)));
	m_UpDirection = glm::cross(m_RightDirection, direction);

	return direction;
}

namespace
{
	template <typename T>
	int sign(T val)
	{
		return (T(0) < val) - (val < T(0));
	}
}

void CCameraComponent3D::DirectionVectorToEulerAngles(const glm::vec3& Direction)
{
	std::shared_ptr<ISceneNode3D> sceneNode3D = std::dynamic_pointer_cast<ISceneNode3D>(GetOwnerNode());
	_ASSERT(sceneNode3D != nullptr);

	// https://gamedev.stackexchange.com/questions/172147/convert-3d-direction-vectors-to-yaw-pitch-roll-angles

	// Yaw is the bearing of the forward vector's shadow in the xy plane.
	float yaw = atan2(sceneNode3D->GetRotation().z, sceneNode3D->GetRotation().x);

	// Pitch is the altitude of the forward vector off the xy plane, toward the down direction.
	float pitch = asin(sceneNode3D->GetRotation().y);

	// Find the vector in the xy plane 90 degrees to the right of our bearing.
	float planeRightX = glm::sin(yaw);
	float planeRightZ = -glm::cos(yaw);

	// Roll is the rightward lean of our up vector, computed here using a dot product.
	float roll = asin(m_UpDirection.x * planeRightX + m_UpDirection.z * planeRightZ);

	// If we're twisted upside-down, return a roll in the range +-(pi/2, pi)
	if (m_UpDirection.y < 0)
		roll = sign(roll) * glm::pi<float>() - roll;


	float yawDegrees = glm::degrees(yaw);
	if (yawDegrees > 360.0f)
		yawDegrees -= 360.0f;
	if (yawDegrees < 0.0f)
		yawDegrees += 360.0f;

	float pitchDegress = glm::degrees(pitch);
	if (pitchDegress > cPitchUpperBorder)
		pitchDegress = cPitchUpperBorder;
	if (pitchDegress < cPitchBottomBorder)
		pitchDegress = cPitchBottomBorder;

	if (glm::abs(m_Yaw_X - yawDegrees) > 1.0f)
		_ASSERT_EXPR(false, "CCameraComponent3D: Error m_Yaw_X");

	if (glm::abs(m_Pitch_Y - pitchDegress) > 1.0f)
		_ASSERT_EXPR(false, "CCameraComponent3D: Error m_Pitch_Y");

	//SetYaw(m_Yaw_X2);
	//SetPitch(m_Pitch_Y2);
}

void CCameraComponent3D::UpdateView()
{
	if (!m_View_Dirty)
		return;

	std::shared_ptr<ISceneNode3D> sceneNode3D = std::dynamic_pointer_cast<ISceneNode3D>(GetOwnerNode());
	_ASSERT(sceneNode3D != nullptr);

	m_View = glm::lookAt(sceneNode3D->GetTranslation(), sceneNode3D->GetTranslation() + sceneNode3D->GetRotation(), m_UpDirection);
	m_Inverse_View = glm::inverse(m_View);

	m_View_Dirty = false;
	m_ProjectionView_Dirty = true;
}

void CCameraComponent3D::UpdateProjectionView()
{
	UpdateView();

	if (!m_ProjectionView_Dirty)
		return;

	m_ProjectionView = m_Projection * m_View;
	m_Inverse_ProjectionView = glm::inverse(m_ProjectionView);
	m_ProjectionView_Dirty = false;
}
