#pragma once

#if 0

ZN_INTERFACE ZN_API ICamera
{
	enum class ZN_API EPerspectiveProjectionHand
	{
		Left,
		Right
	};

	virtual ~ICamera() {}

	virtual glm::vec3                               GetTranslation() const = 0;
	virtual glm::vec3                               GetDirection() const = 0;

	virtual float                                   GetYaw() const = 0;
	virtual float                                   GetPitch() const = 0;
	
	virtual const Frustum*                          GetFrustum() const = 0;

    virtual glm::mat4                               GetViewMatrix() const = 0;
    virtual glm::mat4                               GetProjectionMatrix() const = 0;
    virtual glm::mat4                               GetViewProjectionInverseMatrix() const = 0;

	virtual void                                    SetPerspectiveProjection(float fovy, float aspect, float zNear, float zFar) = 0;
	virtual void                                    SetOrthographicProjection(float left, float right, float top, float bottom) = 0;
};

ZN_INTERFACE ZN_API ICameraMovement
{
	enum class ZN_API Space
	{
		Local,
		World
	};

	virtual ~ICameraMovement() {}

	virtual void                                            TranslateX(float x, Space space = Space::Local) = 0;
	virtual void                                            TranslateY(float y, Space space = Space::Local) = 0;
	virtual void                                            TranslateZ(float z, Space space = Space::Local) = 0;

	virtual void                                            SetTranslate(vec3 Translate) = 0;
	

	// Rotate
	virtual void                                            SetYaw(float Yaw) = 0;
	virtual void                                            AddYaw(float Yaw) = 0;
	virtual void                                            SetPitch(float Pitch) = 0;
	virtual void                                            AddPitch(float Pitch) = 0;
};

#endif
