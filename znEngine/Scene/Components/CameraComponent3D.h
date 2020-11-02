#pragma once

class __declspec(UUID_CameraComponent) ZN_API CCameraComponent3D
	: public ICameraComponent3D
	, public CComponentBase
{
public:
	CCameraComponent3D(const ISceneNode& OwnerNode);
    virtual ~CCameraComponent3D();

	// ICameraComponent3D
	void DoMoveFront(float Value = 1.0f);
	void DoMoveBack(float Value = 1.0f);
	void DoMoveLeft(float Value = 1.0f);
	void DoMoveRight(float Value = 1.0f);

	virtual void SetTranslation(glm::vec3 Translation) override;
	virtual glm::vec3 GetTranslation() const override;
	virtual void SetDirection(glm::vec3 Direction) override;
	virtual glm::vec3 GetDirection() const override;

	void SetYaw(float Yaw) override;
	void AddYaw(float Yaw) override;
	float GetYaw() const override;

	void SetPitch(float Pitch) override;
	void AddPitch(float Pitch) override;
	float GetPitch() const override;

	void SetPerspectiveProjection(EPerspectiveProjectionHand PerspectiveProjectionHand, float fovy, float aspect, float zNear, float zFar) override;
	void SetOrthographicProjection(float left, float right, float top, float bottom, float zNear, float zFar) override;

	const glm::mat4& GetViewMatrix() const override;
	const glm::mat4& GetInverseViewMatrix() const override;

	const glm::mat4& GetProjectionMatrix() const override;
	const glm::mat4& GetInverseProjectionMatrix() const override;

	const glm::mat4& GetProjectionViewMatrix() const override;
	const glm::mat4& GetInverseProjectionViewMatrix() const override;

	const Frustum& GetFrustum() const;
	const glm::vec3& GetCameraUpDirection() const override;

    // ISceneNodeComponent
	virtual void OnMessage(const ISceneNodeComponent* Component, ComponentMessageType Message) override;

protected:
	glm::vec3 EulerAnglesToDirectionVector(float Yaw, float Pitch);
	void DirectionVectorToEulerAngles(const glm::vec3& Direction);

	void UpdateView();
	void UpdateProjectionView();

private: // Properties engine
	std::shared_ptr<CPropertyWrapped<float>>        m_Yaw_XProperty;
	std::shared_ptr<CPropertyWrapped<float>>        m_Pitch_YProperty;


private:
	glm::vec3                                       m_RightDirection;
	glm::vec3                                       m_UpDirection;

	float                                           m_Yaw_X;
	float                                           m_Pitch_Y;

	glm::mat4                                       m_View;
	glm::mat4                                       m_Inverse_View;
	bool                                            m_View_Dirty;

	glm::mat4                                       m_Projection;
	glm::mat4                                       m_Inverse_Projection;

	glm::mat4                                       m_ProjectionView;
	glm::mat4                                       m_Inverse_ProjectionView;
	bool                                            m_ProjectionView_Dirty;

	Frustum                                         m_Frustum;


	// Perspective projection
	float                                           m_Perspective_FOV;
	float                                           m_Perspective_Aspect;

	// Orthographic projection
	float                                           m_Orthographic_Left;
	float                                           m_Orthographic_Right;
	float                                           m_Orthographic_Top;
	float                                           m_Orthographic_Bottom;

	// Perspective & Orthographic
	float                                           m_Near;
	float                                           m_Far;

private: // Consts
	const float                                     cPitchBottomBorder = -80.0f;
	const float                                     cPitchUpperBorder = 80.0f;
};