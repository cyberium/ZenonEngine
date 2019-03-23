#include "stdafx.h"

// General
#include "Camera.h"

Camera::Camera()
	: m_Translate(0)
	, m_bViewDirty(true)
	, m_bViewProjectionInverseDirty(true)
{}

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
	m_fVFOV = fovy;
	m_fAspect = aspect;
	m_fNear = zNear;
	m_fFar = zFar;
	m_ProjectionMatrix = glm::perspective(glm::radians(fovy), aspect, zNear, zFar);
	m_bViewProjectionInverseDirty = true;
}

void Camera::SetProjectionLH(float fovy, float aspect, float zNear, float zFar)
{
	mat4 fix(
		1.0f, 0.0f, 0.0f, 0.0f,
		0.0f, 1.0f, 0.0f, 0.0f,
		0.0f, 0.0f, 2.0f, 0.0f,
		0.0f, 0.0f, -1.0f, 1.0f
	);

	m_fVFOV = fovy;
	m_fAspect = aspect;
	m_fNear = zNear;
	m_fFar = zFar;

	m_ProjectionMatrix = fix * glm::perspective(glm::radians(fovy), aspect, zNear, zFar);
	m_bViewProjectionInverseDirty = true;
}

float Camera::GetNearClipPlane() const
{
	return m_fNear;
}

float Camera::GetFarClipPlane() const
{
	return m_fFar;
}

void Camera::SetOrthographic(float left, float right, float top, float bottom)
{
	m_ProjectionMatrix = glm::ortho(left, right, bottom, top);
	m_bViewProjectionInverseDirty = true;
}


//
// Translate
//
void Camera::TranslateX(float x, Space space)
{
	switch (space)
	{
	case Camera::Space::Local:
		m_Translate += m_Rotate * vec3(x, 0, 0);
		break;

	case Camera::Space::World:
		m_Translate += vec3(x, 0, 0);
		break;
	}
	m_bViewDirty = true;
}

void Camera::TranslateY(float y, Space space)
{
	switch (space)
	{
	case Space::Local:
		m_Translate += m_Rotate * vec3(0, y, 0);
		break;
	case Space::World:
		m_Translate += vec3(0, y, 0);
		break;
	}

	m_bViewDirty = true;
}

void Camera::TranslateZ(float z, Space space)
{
	switch (space)
	{
	case Space::Local:
		m_Translate += m_Rotate * vec3(0, 0, z);
		break;
	case Space::World:
		m_Translate += vec3(0, 0, z);
		break;
	}

	m_bViewDirty = true;
}

void Camera::SetTranslate(cvec3 translate)
{
	m_Translate = translate;
	m_bViewDirty = true;
}

vec3 Camera::GetTranslation() const
{
	return m_Translate;
}


//
// Rotate
//
void Camera::SetEulerAngles(cvec3 eulerAngles)
{
	m_Rotate = glm::quat(glm::radians(eulerAngles));
	m_bViewDirty = true;
}

vec3 Camera::GetEulerAngles() const
{
	return glm::degrees(glm::eulerAngles(m_Rotate));
}

void Camera::AddPitch(float fPitch, Space space)
{
	switch (space)
	{
	case Space::Local:
		m_Rotate = glm::angleAxis(glm::radians(fPitch), m_Rotate * vec3(1, 0, 0)) * m_Rotate;
		break;
	case Space::World:
		m_Rotate = glm::angleAxis(glm::radians(fPitch), vec3(1, 0, 0)) * m_Rotate;
	}

	m_bViewDirty = true;
}

void Camera::AddYaw(float fYaw, Space space)
{
	switch (space)
	{
	case Space::Local:
		m_Rotate = glm::angleAxis(glm::radians(fYaw), m_Rotate * vec3(0, 1, 0)) * m_Rotate;
		break;
	case Space::World:
		m_Rotate = glm::angleAxis(glm::radians(fYaw), vec3(0, 1, 0)) * m_Rotate;
		break;
	}

	m_bViewDirty = true;
}

void Camera::AddRoll(float fRoll, Space space)
{
	switch (space)
	{
	case Space::Local:
		m_Rotate = glm::angleAxis(glm::radians(fRoll), m_Rotate * vec3(0, 0, 1)) * m_Rotate;
		break;
	case Space::World:
		m_Rotate = glm::angleAxis(glm::radians(fRoll), vec3(0, 0, 1)) * m_Rotate;
		break;
	}
	m_bViewDirty = true;
}

void Camera::AddRotation(const glm::quat& deltaRot)
{
	m_Rotate = m_Rotate * deltaRot;
	m_bViewDirty = true;
}

void Camera::SetRotate(float pitch, float yaw, float roll)
{
	SetRotate(vec3(pitch, yaw, roll));
}

void Camera::SetRotate(cvec3 rotate)
{
	SetRotate(glm::quat(glm::radians(rotate)));
}

void Camera::SetRotate(const glm::quat& rot)
{
	m_Rotate = rot;
	m_bViewDirty = true;
}

glm::quat Camera::GetRotation() const
{
	return m_Rotate;
}



bool Camera::IsDirty() const
{
	return m_bViewDirty;
}


void Camera::UpdateViewMatrix()
{
	if (m_bViewDirty)
	{
		mat4 translateMatrix = glm::translate(m_Translate);
		mat4 rotationMatrix = glm::toMat4(m_Rotate);

		m_ViewMatrix = glm::inverse(translateMatrix * rotationMatrix);

		// Update frustum
		m_Frustum.buildViewFrustum(m_ViewMatrix, m_ProjectionMatrix);

		m_bViewProjectionInverseDirty = true;
		m_bViewDirty = false;
	}
}

void Camera::UpdateViewProjectionInverse()
{
	UpdateViewMatrix();

	if (m_bViewProjectionInverseDirty)
	{
		m_ViewProjectionInverse = glm::inverse(m_ProjectionMatrix * m_ViewMatrix);
		m_bViewProjectionInverseDirty = false;
	}
}

void Camera::SetViewMatrix(cmat4 viewMatrix)
{
	// Inverse the view matrix to get the world matrix of the camera
	mat4 inverseView = glm::inverse(viewMatrix);

	// Extract the translation
	m_Translate = vec3(inverseView[3]);

	// Extract the top-left 3x3 matrix to decompose the scale and rotation
	glm::mat3 tmp = glm::mat3(inverseView);

	// TODO: I don't know if any of the scales are negative.
	// I have to figure out how can I reliably determine if any of the scales are negative?
	float sx = glm::length(tmp[0]);
	float sy = glm::length(tmp[1]);
	float sz = glm::length(tmp[2]);

	glm::mat3 invScale = glm::mat3(glm::scale(vec3(1.0f / sx, 1.0f / sy, 1.0f / sz)));
	// This will remove the scale factor (if there is one) so we can extract
	// the unit quaternion.
	tmp = tmp * invScale;
	m_Rotate = glm::toQuat(tmp);

	// The view matrix needs to be rebuilt from the rotation and translation components.
	m_bViewDirty = true;
}

mat4 Camera::GetViewMatrix() const
{
	const_cast<Camera*>(this)->UpdateViewMatrix();
	return m_ViewMatrix;
}

void Camera::SetProjectionMatrix(cmat4 projectionMatrix)
{
	// TODO: Decompose the projection matrix?
	m_ProjectionMatrix = projectionMatrix;
	m_bViewProjectionInverseDirty = true;
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


