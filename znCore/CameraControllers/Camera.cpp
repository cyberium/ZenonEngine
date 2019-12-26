#include "stdafx.h"

// General
#include "Camera.h"

CCamera::CCamera()
	: m_Translate(0)
	, m_Front(0)
	, m_Right(0)
	, m_Up(0)
	, m_Yaw_X(0.0f)
	, m_Pitch_Y(0.0f)
	, m_ViewDirty(true)
	, m_ViewProjectionInverseDirty(true)
{}

CCamera::CCamera(float left, float right, float top, float bottom)
	: m_Translate(0)
	, m_Front(0)
	, m_Right(0)
	, m_Up(0)
	, m_Yaw_X(0.0f)
	, m_Pitch_Y(0.0f)
	, m_ViewDirty(true)
	, m_ViewProjectionInverseDirty(true)
{
	UpdateViewMatrix();
	m_ProjectionMatrix = glm::ortho<float>(left, right, bottom, top);
}

CCamera::CCamera(ProjectionHand ProjectionHand, float fovy, float aspect, float zNear, float zFar)
	: m_Translate(0)
	, m_Front(0)
	, m_Right(0)
	, m_Up(0)
    , m_Yaw_X(0.0f)
    , m_Pitch_Y(0.0f)
	, m_ViewDirty(true)
	, m_ViewProjectionInverseDirty(true)
{
	UpdateViewMatrix();
}

CCamera::~CCamera()
{
}



//
// IznCamera
//
const Frustum* CCamera::GetFrustum() const
{
	return &m_Frustum;
}

vec3 CCamera::GetTranslation() const
{
	return m_Translate;
}

float CCamera::GetYaw() const
{
	return m_Yaw_X;
}

float CCamera::GetPitch() const
{
	return m_Pitch_Y;
}

glm::vec3 CCamera::GetDirection() const
{
	return m_Front;
}

mat4 CCamera::GetViewMatrix() const
{
	const_cast<CCamera*>(this)->UpdateViewMatrix();
	return m_ViewMatrix;
}

mat4 CCamera::GetProjectionMatrix() const
{
	return m_ProjectionMatrix;
}

mat4 CCamera::GetViewProjectionInverseMatrix() const
{
	const_cast<CCamera*>(this)->UpdateViewProjectionInverse();
	return m_ViewProjectionInverse;
}

void CCamera::SetProjection(ProjectionHand ProjectionHand, float fovy, float aspect, float zNear, float zFar)
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

void CCamera::SetOrthographic(float left, float right, float top, float bottom)
{
	m_ProjectionMatrix = glm::ortho<float>(left, right, bottom, top);
	m_ViewProjectionInverseDirty = true;
}



//
// ICameraMovement
//
void CCamera::TranslateX(float x, Space space)
{
	switch (space)
	{
	case CCamera::Space::Local:
		AddTranslate(m_Front * vec3(x, 0, 0));
		break;

	case CCamera::Space::World:
		AddTranslate(vec3(x, 0, 0));
		break;
	}
}

void CCamera::TranslateY(float y, Space space)
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

void CCamera::TranslateZ(float z, Space space)
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

void CCamera::DoMoveFront(float Value)
{
	AddTranslate(m_Front * Value);
}

void CCamera::DoMoveBack(float Value)
{
	AddTranslate(-(m_Front * Value));
}

void CCamera::DoMoveLeft(float Value)
{
	AddTranslate(-(m_Right * Value));
}

void CCamera::DoMoveRight(float Value)
{
	AddTranslate(m_Right * Value);
}

void CCamera::SetTranslate(vec3 Translate)
{
	m_Translate = Translate;
	m_ViewDirty = true;
}

void CCamera::AddTranslate(vec3 Translate)
{
	m_Translate += Translate;
	m_ViewDirty = true;
}

void CCamera::SetYaw(float Yaw)
{
    m_Yaw_X = Yaw;
    m_ViewDirty = true;
}

void CCamera::AddYaw(float Yaw)
{
    m_Yaw_X += Yaw;
    m_ViewDirty = true;
}

void CCamera::SetPitch(float Pitch)
{
    m_Pitch_Y = Pitch;
    m_ViewDirty = true;
}

void CCamera::AddPitch(float Pitch)
{
    m_Pitch_Y += Pitch;
    m_ViewDirty = true;
}



//
// Protected
//
void CCamera::UpdateDirections()
{
    // Calculate the new Front vector
    m_Front.x = glm::cos(glm::radians(m_Yaw_X)) * glm::cos(glm::radians(m_Pitch_Y)); // y
    m_Front.y = glm::sin(glm::radians(m_Pitch_Y));                              // z
    m_Front.z = glm::sin(glm::radians(m_Yaw_X)) * glm::cos(glm::radians(m_Pitch_Y)); // x
    m_Front = glm::normalize(m_Front);

    // Also re-calculate the Right and Up vector
    m_Right = glm::normalize(glm::cross(m_Front, vec3(0.0f, 1.0f, 0.0f))); 
    m_Up    = glm::cross(m_Right, m_Front);
}

void CCamera::UpdateViewMatrix()
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

void CCamera::UpdateViewProjectionInverse()
{
	UpdateViewMatrix();

	if (m_ViewProjectionInverseDirty)
	{
		m_ViewProjectionInverse = glm::inverse(m_ProjectionMatrix * m_ViewMatrix);
		m_ViewProjectionInverseDirty = false;
	}
}
