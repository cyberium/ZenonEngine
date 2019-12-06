#pragma once

ZN_INTERFACE OW_ENGINE_API ICamera
{
	enum class OW_ENGINE_API ProjectionHand
	{
		Left,
		Right
	};

	virtual ~ICamera() {}

	virtual const Frustum*                          GetFrustum() const = 0;

	virtual vec3                                    GetTranslation() const = 0;

	virtual float                                   GetYaw() const = 0;
	virtual float                                   GetPitch() const = 0;
	virtual glm::vec3                               GetDirection() const = 0;

    virtual glm::mat4                               GetViewMatrix() const = 0;
    virtual glm::mat4                               GetProjectionMatrix() const = 0;
    virtual glm::mat4                               GetViewProjectionInverseMatrix() const = 0;

	virtual void                                    SetProjection(ProjectionHand ProjectionHand, float fovy, float aspect, float zNear, float zFar) = 0;
	virtual void                                    SetOrthographic(float left, float right, float top, float bottom) = 0;
};

ZN_INTERFACE OW_ENGINE_API ICameraMovement
{
	enum class OW_ENGINE_API Space
	{
		Local,
		World
	};

	virtual ~ICameraMovement() {}

	virtual void                                            TranslateX(float x, Space space = Space::Local) = 0;
	virtual void                                            TranslateY(float y, Space space = Space::Local) = 0;
	virtual void                                            TranslateZ(float z, Space space = Space::Local) = 0;
	virtual void                                            DoMoveFront(float Value = 1.0f) = 0;
	virtual void                                            DoMoveBack(float Value = 1.0f) = 0;
	virtual void                                            DoMoveLeft(float Value = 1.0f) = 0;
	virtual void                                            DoMoveRight(float Value = 1.0f) = 0;
	virtual void                                            SetTranslate(vec3 Translate) = 0;
	virtual void                                            AddTranslate(vec3 Translate) = 0;

	// Rotate
	virtual void                                            SetYaw(float Yaw) = 0;
	virtual void                                            AddYaw(float Yaw) = 0;
	virtual void                                            SetPitch(float Pitch) = 0;
	virtual void                                            AddPitch(float Pitch) = 0;
};
