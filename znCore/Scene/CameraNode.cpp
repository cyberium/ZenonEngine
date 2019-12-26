#include "stdafx.h"

// General
#include "CameraNode.h"

const float cPitchBottomBorder = -80.0f;
const float cPitchUpperBorder = 80.0f;

CSceneNodeCamera::CSceneNodeCamera()
	: SceneNodeBase()
	, m_Translate(0.0f)
	, m_Direction(0)
	, m_RightDirection(0)
	, m_UpDirection(0)
	, m_Yaw_X(0.0f)
	, m_Pitch_Y(0.0f)
	, m_ViewDirty(true)
	, m_ViewProjectionInverseDirty(true)
{
	SetName("CSceneNodeCamera");

	std::shared_ptr<IPropertiesGroup> propertiesGroup = std::make_shared<CPropertiesGroup>("Transform", "Transorm of this 3D node. Like translation, rotation and scale.");

	m_TranslateProperty = std::make_shared<CPropertyWrapped<glm::vec3>>("Position", "Camera position.");
	m_TranslateProperty->SetValueSetter(std::bind(&CSceneNodeCamera::SetTranslate, this, std::placeholders::_1));
	m_TranslateProperty->SetValueGetter(std::bind(&CSceneNodeCamera::GetTranslation, this));
	propertiesGroup->AddProperty(m_TranslateProperty);

	m_Yaw_XProperty = std::make_shared<CPropertyWrapped<float>>("Yaw", "Rotation around the Y axis (in degrees).");
	m_Yaw_XProperty->SetValueSetter(std::bind(&CSceneNodeCamera::SetYaw, this, std::placeholders::_1));
	m_Yaw_XProperty->SetValueGetter(std::bind(&CSceneNodeCamera::GetYaw, this));
	propertiesGroup->AddProperty(m_Yaw_XProperty);

	m_Pitch_YProperty = std::make_shared<CPropertyWrapped<float>>("Pitch", "Rotation around the X and Y axis (in degrees).");
	m_Pitch_YProperty->SetValueSetter(std::bind(&CSceneNodeCamera::SetPitch, this, std::placeholders::_1));
	m_Pitch_YProperty->SetValueGetter(std::bind(&CSceneNodeCamera::GetPitch, this));
	propertiesGroup->AddProperty(m_Pitch_YProperty);

	GetProperties()->AddProperty(propertiesGroup);
}

CSceneNodeCamera::~CSceneNodeCamera()
{
}


//
// ICamera
//
glm::vec3 CSceneNodeCamera::GetTranslation() const
{
	return m_Translate;
}

glm::vec3 CSceneNodeCamera::GetDirection() const
{
	return m_Direction;
}

float CSceneNodeCamera::GetYaw() const
{
	return m_Yaw_X;
}

float CSceneNodeCamera::GetPitch() const
{
	return m_Pitch_Y;
}

const Frustum* CSceneNodeCamera::GetFrustum() const
{
	return &m_Frustum;
}

mat4 CSceneNodeCamera::GetViewMatrix() const
{
	const_cast<CSceneNodeCamera*>(this)->UpdateViewMatrix();
	return m_ViewMatrix;
}

mat4 CSceneNodeCamera::GetProjectionMatrix() const
{
	return m_ProjectionMatrix;
}

mat4 CSceneNodeCamera::GetViewProjectionInverseMatrix() const
{
	const_cast<CSceneNodeCamera*>(this)->UpdateViewProjectionInverse();
	return m_ViewProjectionInverse;
}

void CSceneNodeCamera::SetPerspectiveProjection(EPerspectiveProjectionHand PerspectiveProjectionHand, float fovy, float aspect, float zNear, float zFar)
{
	mat4 fix(
		1.0f, 0.0f, 0.0f, 0.0f,
		0.0f, 1.0f, 0.0f, 0.0f,
		0.0f, 0.0f, 2.0f, 0.0f,
		0.0f, 0.0f, -1.0f, 1.0f
	);

	m_VFOV = fovy;
	m_Aspect = aspect;
	m_Near = zNear;
	m_Far = zFar;

	mat4 perspectiveMatrix = glm::perspective(glm::radians(fovy), aspect, zNear, zFar);
	if (PerspectiveProjectionHand == EPerspectiveProjectionHand::Right)
	{
		m_ProjectionMatrix = perspectiveMatrix;
	}
	else
	{
		m_ProjectionMatrix = fix * perspectiveMatrix;
	}

	m_ViewProjectionInverseDirty = true;
}

void CSceneNodeCamera::SetOrthographicProjection(float left, float right, float top, float bottom)
{
	m_Left = left;
	m_Right = right;
	m_Top = top;
	m_Bottom = bottom;

	m_ProjectionMatrix = glm::ortho<float>(m_Left, m_Right, m_Top, m_Bottom);
	m_ViewProjectionInverseDirty = true;
}



//
// ICameraMovement
//
void CSceneNodeCamera::TranslateX(float x, Space space)
{
	switch (space)
	{
	case CSceneNodeCamera::Space::Local:
		AddTranslate(m_Direction * vec3(x, 0, 0));
		break;

	case CSceneNodeCamera::Space::World:
		AddTranslate(vec3(x, 0, 0));
		break;
	}
}

void CSceneNodeCamera::TranslateY(float y, Space space)
{
	switch (space)
	{
	case Space::Local:
		AddTranslate(m_Direction * vec3(0, y, 0));
		break;
	case Space::World:
		AddTranslate(vec3(0, y, 0));
		break;
	}
}

void CSceneNodeCamera::TranslateZ(float z, Space space)
{
	switch (space)
	{
	case Space::Local:
		AddTranslate(m_Direction * vec3(0, 0, z));
		break;
	case Space::World:
		AddTranslate(vec3(0, 0, z));
		break;
	}
}

void CSceneNodeCamera::DoMoveFront(float Value)
{
	AddTranslate(m_Direction * Value);
}

void CSceneNodeCamera::DoMoveBack(float Value)
{
	AddTranslate(-(m_Direction * Value));
}

void CSceneNodeCamera::DoMoveLeft(float Value)
{
	AddTranslate(-(m_RightDirection * Value));
}

void CSceneNodeCamera::DoMoveRight(float Value)
{
	AddTranslate(m_RightDirection * Value);
}

void CSceneNodeCamera::SetTranslate(vec3 Translate)
{
	m_Translate = Translate;
	m_TranslateProperty->RaiseSetValueChangedCallback();

	m_ViewDirty = true;
}

void CSceneNodeCamera::AddTranslate(vec3 Translate)
{
	SetTranslate(m_Translate + Translate);
}

void CSceneNodeCamera::SetYaw(float Yaw)
{
	m_Yaw_X = Yaw;
	if (m_Yaw_X > 360.0f)
		m_Yaw_X -= 360.0f;
	if (m_Yaw_X < 0.0f)
		m_Yaw_X += 360.0f;

	m_Yaw_XProperty->RaiseSetValueChangedCallback();

	EulerAnglesToDirectionVector();

	m_ViewDirty = true;
}

void CSceneNodeCamera::AddYaw(float Yaw)
{
	SetYaw(m_Yaw_X + Yaw);
}

void CSceneNodeCamera::SetPitch(float Pitch)
{
	m_Pitch_Y = Pitch;
	if (m_Pitch_Y > cPitchUpperBorder)
		m_Pitch_Y = cPitchUpperBorder;
	if (m_Pitch_Y < cPitchBottomBorder)
		m_Pitch_Y = cPitchBottomBorder;

	m_Pitch_YProperty->RaiseSetValueChangedCallback();

	EulerAnglesToDirectionVector();

	m_ViewDirty = true;
}

void CSceneNodeCamera::AddPitch(float Pitch)
{
	SetPitch(m_Pitch_Y + Pitch);
}



//
// Protected
//
void CSceneNodeCamera::UpdateLocalTransform()
{
	m_LocalTransform = glm::mat4(1.0f);

	m_LocalTransform = glm::translate(m_LocalTransform, m_Translate);
	m_LocalTransform = glm::rotate(m_LocalTransform, m_Direction.x, glm::vec3(1, 0, 0));
	m_LocalTransform = glm::rotate(m_LocalTransform, m_Direction.y, glm::vec3(0, 1, 0));
	m_LocalTransform = glm::rotate(m_LocalTransform, m_Direction.z, glm::vec3(0, 0, 1));
	m_InverseLocalTransform = glm::inverse(m_LocalTransform);

	//RaiseComponentMessage(UUID_TransformComponent_OnLocalTransformChanged);

	// Don't forget to update world transform
	UpdateWorldTransform();
}

void CSceneNodeCamera::UpdateWorldTransform()
{
	m_WorldTransform = GetParentWorldTransform() * m_LocalTransform;
	m_InverseWorldTransform = glm::inverse(m_WorldTransform);

	//RaiseComponentMessage(UUID_TransformComponent_OnWorldTransformChanged);
}

void CSceneNodeCamera::EulerAnglesToDirectionVector()
{
	// Calculate the new Front vector
	m_Direction.x = glm::cos(glm::radians(m_Yaw_X)) * glm::cos(glm::radians(m_Pitch_Y)); // y
	m_Direction.y = glm::sin(glm::radians(m_Pitch_Y));                              // z
	m_Direction.z = glm::sin(glm::radians(m_Yaw_X)) * glm::cos(glm::radians(m_Pitch_Y)); // x
	//m_Direction = glm::normalize(m_Direction);

	// Also re-calculate the Right and Up vector
	m_RightDirection = glm::normalize(glm::cross(m_Direction, vec3(0.0f, 1.0f, 0.0f)));
	m_UpDirection = glm::cross(m_RightDirection, m_Direction);

	DirectionVectorToEulerAngles();
}

template <typename T> 
int sign(T val) 
{
	return (T(0) < val) - (val < T(0));
}

void CSceneNodeCamera::DirectionVectorToEulerAngles()
{
	// https://gamedev.stackexchange.com/questions/172147/convert-3d-direction-vectors-to-yaw-pitch-roll-angles

	// Yaw is the bearing of the forward vector's shadow in the xy plane.
	float yaw = atan2(m_Direction.z, m_Direction.x);

	// Pitch is the altitude of the forward vector off the xy plane, toward the down direction.
	float pitch = asin(m_Direction.y);

	// Find the vector in the xy plane 90 degrees to the right of our bearing.
	float planeRightX = sin(yaw);
	float planeRightZ = -cos(yaw);

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
		_ASSERT_EXPR(false, "Error m_Yaw_X");

	if (glm::abs(m_Pitch_Y - pitchDegress) > 1.0f)
		_ASSERT_EXPR(false, "Error m_Pitch_Y");


	//SetYaw(m_Yaw_X2);
	//SetPitch(m_Pitch_Y2);
}

void CSceneNodeCamera::UpdateViewMatrix()
{
	if (m_ViewDirty)
	{
		m_ViewMatrix = glm::lookAt(m_Translate, m_Translate + m_Direction, m_UpDirection);

		// Update frustum
		m_Frustum.buildViewFrustum(m_ViewMatrix, m_ProjectionMatrix);

		m_ViewProjectionInverseDirty = true;
		m_ViewDirty = false;
	}
}

void CSceneNodeCamera::UpdateViewProjectionInverse()
{
	UpdateViewMatrix();

	if (m_ViewProjectionInverseDirty)
	{
		m_ViewProjectionInverse = glm::inverse(m_ProjectionMatrix * m_ViewMatrix);
		m_ViewProjectionInverseDirty = false;
	}
}