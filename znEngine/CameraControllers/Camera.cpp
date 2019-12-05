#include "stdafx.h"

// General
#include "Camera.h"

Camera::Camera()
	: m_Translate(0)
	, m_Front(0)
	, m_Right(0)
	, m_Up(0)
	, m_Yaw_X(0.0f)
	, m_Pitch_Y(0.0f)
	, m_ViewDirty(true)
	, m_ViewProjectionInverseDirty(true)
{}

Camera::Camera(float left, float right, float top, float bottom)
	: m_Translate(0)
	, m_Front(0)
	, m_Right(0)
	, m_Up(0)
	, m_Yaw_X(0.0f)
	, m_Pitch_Y(0.0f)
	, m_ViewDirty(true)
	, m_ViewProjectionInverseDirty(true)
{
	m_ProjectionMatrix = glm::ortho<float>(left, right, bottom, top);
}

Camera::Camera(ProjectionHand ProjectionHand, float fovy, float aspect, float zNear, float zFar)
	: m_Translate(0)
	, m_Front(0)
	, m_Right(0)
	, m_Up(0)
    , m_Yaw_X(0.0f)
    , m_Pitch_Y(0.0f)
	, m_ViewDirty(true)
	, m_ViewProjectionInverseDirty(true)
{
	
}

Camera::~Camera()
{
}



//
// IznCamera
//
const Frustum* Camera::GetFrustum() const
{
	return &m_Frustum;
}

vec3 Camera::GetTranslation() const
{
	return m_Translate;
}

float Camera::GetYaw() const
{
	return m_Yaw_X;
}

float Camera::GetPitch() const
{
	return m_Pitch_Y;
}

glm::vec3 Camera::GetDirection() const
{
	return m_Front;
}

mat4 Camera::GetViewMatrix() const
{
	const_cast<Camera*>(this)->UpdateViewMatrix();
	return m_ViewMatrix;
}

mat4 Camera::GetProjectionMatrix() const
{
	return m_ProjectionMatrix;
}

mat4 Camera::GetViewProjectionInverseMatrix() const
{
	const_cast<Camera*>(this)->UpdateViewProjectionInverse();
	return m_ViewProjectionInverse;
}

void Camera::SetProjection(ProjectionHand ProjectionHand, float fovy, float aspect, float zNear, float zFar)
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
	if (ProjectionHand == ProjectionHand::Right)
	{
		m_ProjectionMatrix = perspectiveMatrix;
	}
	else
	{
		m_ProjectionMatrix = fix * perspectiveMatrix;
	}

	m_ViewProjectionInverseDirty = true;
}

void Camera::SetOrthographic(float left, float right, float top, float bottom)
{
	m_ProjectionMatrix = glm::ortho<float>(left, right, bottom, top);
	m_ViewProjectionInverseDirty = true;
}



//
// ICameraMovement
//
void Camera::TranslateX(float x, Space space)
{
	switch (space)
	{
	case Camera::Space::Local:
		AddTranslate(m_Front * vec3(x, 0, 0));
		break;

	case Camera::Space::World:
		AddTranslate(vec3(x, 0, 0));
		break;
	}
}

void Camera::TranslateY(float y, Space space)
{
	switch (space)
	{
	case Space::Local:
		AddTranslate(m_Front * vec3(0, y, 0));
		break;
	case Space::World:
		AddTranslate(vec3(0, y, 0));
		break;
	}
}

void Camera::TranslateZ(float z, Space space)
{
	switch (space)
	{
	case Space::Local:
		AddTranslate(m_Front * vec3(0, 0, z));
		break;
	case Space::World:
		AddTranslate(vec3(0, 0, z));
		break;
	}
}

void Camera::DoMoveFront(float Value)
{
	AddTranslate(m_Front * Value);
}

void Camera::DoMoveBack(float Value)
{
	AddTranslate(-(m_Front * Value));
}

void Camera::DoMoveLeft(float Value)
{
	AddTranslate(-(m_Right * Value));
}

void Camera::DoMoveRight(float Value)
{
	AddTranslate(m_Right * Value);
}

void Camera::SetTranslate(vec3 Translate)
{
	m_Translate = Translate;
	m_ViewDirty = true;
}

void Camera::AddTranslate(vec3 Translate)
{
	m_Translate += Translate;
	m_ViewDirty = true;
}

void Camera::SetYaw(float Yaw)
{
    m_Yaw_X = Yaw;
    m_ViewDirty = true;
}

void Camera::AddYaw(float Yaw)
{
    m_Yaw_X += Yaw;
    m_ViewDirty = true;
}

void Camera::SetPitch(float Pitch)
{
    m_Pitch_Y = Pitch;
    m_ViewDirty = true;
}

void Camera::AddPitch(float Pitch)
{
    m_Pitch_Y += Pitch;
    m_ViewDirty = true;
}



//
// Protected
//
void Camera::UpdateDirections()
{
    // Calculate the new Front vector
    m_Front.x = cos(glm::radians(m_Yaw_X)) * cos(glm::radians(m_Pitch_Y)); // y
    m_Front.y = sin(glm::radians(m_Pitch_Y));                              // z
    m_Front.z = sin(glm::radians(m_Yaw_X)) * cos(glm::radians(m_Pitch_Y)); // x
    m_Front = glm::normalize(m_Front);

    // Also re-calculate the Right and Up vector
    m_Right = glm::normalize(glm::cross(m_Front, vec3(0.0f, 1.0f, 0.0f))); 
    m_Up    = glm::cross(m_Right, m_Front);
}

void Camera::UpdateViewMatrix()
{
	if (m_ViewDirty)
	{
        UpdateDirections();

        m_ViewMatrix = glm::lookAt(m_Translate, m_Translate + m_Front, m_Up);

        // Update frustum
        m_Frustum.buildViewFrustum(m_ViewMatrix, m_ProjectionMatrix);

        m_ViewProjectionInverseDirty = true;
        m_ViewDirty = false;
	}
}

void Camera::UpdateViewProjectionInverse()
{
	UpdateViewMatrix();

	if (m_ViewProjectionInverseDirty)
	{
		m_ViewProjectionInverse = glm::inverse(m_ProjectionMatrix * m_ViewMatrix);
		m_ViewProjectionInverseDirty = false;
	}
}
