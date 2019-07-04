#include "stdafx.h"

// General
#include "Camera.h"

Camera::Camera()
	: m_Translate(0)
    , m_Yaw_X(0.0f)
    , m_Pitch_Y(0.0f)
	, m_ViewDirty(true)
	, m_ViewProjectionInverseDirty(true)
{}

Camera::~Camera()
{
}

void Camera::SetViewport(const Viewport& viewport)
{
	m_Viewport = viewport;
}

const Viewport& Camera::GetViewport() const
{
	return m_Viewport;
}

const Frustum& Camera::GetFrustum() const
{
	return m_Frustum;
}

void Camera::SetProjectionRH(float fovy, float aspect, float zNear, float zFar)
{
	m_VFOV = fovy;
	m_Aspect = aspect;
	m_Near = zNear;
	m_Far = zFar;

	m_ProjectionMatrix = glm::perspective(glm::radians(fovy), aspect, zNear, zFar);
	m_ViewProjectionInverseDirty = true;
}

void Camera::SetProjectionLH(float fovy, float aspect, float zNear, float zFar)
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

	m_ProjectionMatrix = fix * glm::perspective(glm::radians(fovy), aspect, zNear, zFar);
	m_ViewProjectionInverseDirty = true;
}

void Camera::SetOrthographic(float left, float right, float top, float bottom)
{
    m_ProjectionMatrix = glm::ortho<float>(left, right, bottom, top);
    m_ViewProjectionInverseDirty = true;
}

float Camera::GetNearClipPlane() const
{
	return m_Near;
}

float Camera::GetFarClipPlane() const
{
	return m_Far;
}



//
// Translate
//
void Camera::TranslateX(float x, Space space)
{
	switch (space)
	{
	case Camera::Space::Local:
		m_Translate += m_Front * vec3(x, 0, 0);
		break;

	case Camera::Space::World:
		m_Translate += vec3(x, 0, 0);
		break;
	}
	m_ViewDirty = true;
}

void Camera::TranslateY(float y, Space space)
{
	switch (space)
	{
	case Space::Local:
		m_Translate += m_Front * vec3(0, y, 0);
		break;
	case Space::World:
		m_Translate += vec3(0, y, 0);
		break;
	}

	m_ViewDirty = true;
}

void Camera::TranslateZ(float z, Space space)
{
	switch (space)
	{
	case Space::Local:
		m_Translate += m_Front * vec3(0, 0, z);
		break;
	case Space::World:
		m_Translate += vec3(0, 0, z);
		break;
	}

	m_ViewDirty = true;
}

void Camera::SetTranslate(cvec3 translate)
{
	m_Translate = translate;
	m_ViewDirty = true;
}

vec3 Camera::GetTranslation() const
{
	return m_Translate;
}

void Camera::DoMoveFront(float Value)
{
    m_Translate += m_Front * Value;
    m_ViewDirty = true;
}

void Camera::DoMoveBack(float Value)
{
    m_Translate -= m_Front * Value;
    m_ViewDirty = true;
}

void Camera::DoMoveLeft(float Value)
{
    m_Translate -= m_Right * Value;
    m_ViewDirty = true;
}

void Camera::DoMoveRight(float Value)
{
    m_Translate += m_Right * Value;
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

float Camera::GetYaw() const
{
    return m_Yaw_X;
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

float Camera::GetPitch() const
{
    return m_Pitch_Y;
}

glm::vec3 Camera::GetDirection() const
{
    return m_Front;
}




bool Camera::IsDirty() const
{
	return m_ViewDirty;
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


