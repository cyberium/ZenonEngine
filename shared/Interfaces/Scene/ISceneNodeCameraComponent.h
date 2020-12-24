#pragma once

// FORWARD BEGIN
ZN_INTERFACE ISceneNodeComponent;
// FORWARD END


ZN_INTERFACE ZN_API ICameraComponent3D
{
	ZN_OBJECTCLASS(cSceneNodeCameraComponent);

	virtual ~ICameraComponent3D() {}

	virtual void DoMoveFront(float Value = 1.0f) = 0;
	virtual void DoMoveBack(float Value = 1.0f) = 0;
	virtual void DoMoveLeft(float Value = 1.0f) = 0;
	virtual void DoMoveRight(float Value = 1.0f) = 0;

	virtual void SetPosition(glm::vec3 Position) = 0;
	virtual glm::vec3 GetPosition() const = 0;
	virtual void SetDirection(glm::vec3 Direction) = 0;
	virtual glm::vec3 GetDirection() const = 0;

	virtual void SetYaw(float Yaw) = 0;
	virtual void AddYaw(float Yaw) = 0;
	virtual float GetYaw() const = 0;

	virtual void SetPitch(float Pitch) = 0;
	virtual void AddPitch(float Pitch) = 0;
	virtual float GetPitch() const = 0;

	virtual void SetPerspectiveProjection(float fovy, float aspect, float zNear, float zFar) = 0;
	virtual void SetOrthographicProjection(float aspect, float left, float right, float top, float bottom, float zNear, float zFar) = 0;

	virtual const glm::mat4& GetViewMatrix() const = 0;
	virtual const glm::mat4& GetInverseViewMatrix() const = 0;

	virtual const glm::mat4& GetProjectionMatrix() const = 0;
	virtual const glm::mat4& GetInverseProjectionMatrix() const = 0;

	virtual const Frustum& GetFrustum() const = 0;
	virtual const glm::vec3& GetCameraUpDirection() const = 0;
};
