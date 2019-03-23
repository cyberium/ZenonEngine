#pragma once

#include "Ray.h"
#include "RenderEvents.h"
#include "Viewport.h"

class Camera : public Object
{
public:
	enum class Space
	{
		Local,
		World
	};

public:
	Camera();
	virtual ~Camera() {};

	// TODO: Viewports belong in the rasterizer state.. not the camera. Set view port parameters
	void SetViewport(const Viewport& viewport);
	const Viewport& GetViewport() const;

	const Frustum& GetFrustum() const;

	void SetProjectionRH(float fovy, float aspect, float zNear, float zFar);
	void SetProjectionLH(float fovy, float aspect, float zNear, float zFar);

	float GetNearClipPlane() const;
	float GetFarClipPlane() const;

	void SetOrthographic(float left, float right, float top, float bottom);

	// Translate

	void TranslateX(float x, Space space = Space::Local);
	void TranslateY(float y, Space space = Space::Local);
	void TranslateZ(float z, Space space = Space::Local);

	void SetTranslate(cvec3 translate);
	vec3 GetTranslation() const;

	// Rotate
	
	void SetEulerAngles(cvec3 eulerAngles);                         // Set Euler angles (in degrees)
	vec3 GetEulerAngles() const;									// Get the current rotation of the camera in Euler angles (degrees)

	void AddPitch(float fPitch, Space space = Space::Local);        // Add this pitch (rotation about the X-axis) in degrees to the current camera's pitch 
	void AddYaw(float fYaw, Space space = Space::Local);            // Add this yaw (rotation about the Y-axis) in degrees to the current camera's yaw
	void AddRoll(float fRoll, Space space = Space::Local);          // Add this roll (rotation about the Z-axis) in degrees to the current camera's roll

	void AddRotation(const glm::quat& rot);                         // Add rotation to the current rotation (always in local space)

	void SetRotate(float pitch, float yaw, float roll);
	void SetRotate(cvec3 rotate);
	void SetRotate(const glm::quat& rot);
	glm::quat GetRotation() const;

	bool IsDirty() const;
	
	// Matrices

	void SetViewMatrix(cmat4 viewMatrix);
	mat4 GetViewMatrix() const;

	void SetProjectionMatrix(cmat4 projectionMatrix);
	mat4 GetProjectionMatrix() const;

	mat4 GetViewProjectionInverseMatrix() const; // Get the view projection inverse matrix (useful for picking)

protected:
	virtual void UpdateViewMatrix();
	virtual void UpdateViewProjectionInverse();

	Viewport    m_Viewport;
	Frustum     m_Frustum;

	// Projection parameters
	float       m_fVFOV;
	float       m_fAspect;
	float       m_fNear;
	float       m_fFar;

	// World space parameters
	vec3        m_Translate;
	glm::quat   m_Rotate;

	mat4        m_ViewMatrix;
	mat4        m_ProjectionMatrix;
	mat4	    m_ViewProjectionInverse;

	bool        m_bViewDirty;
	bool		m_bViewProjectionInverseDirty;
};