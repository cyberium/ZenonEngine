#pragma once

#include "SceneNodeBase.h"
#include "Properties.h"


class ZN_API CSceneNodeCamera
	: public SceneNodeBase
	, public ICamera
	, public ICameraMovement
{
public:
	CSceneNodeCamera();
	virtual ~CSceneNodeCamera();


	//
	// ICamera
	//
	glm::vec3 GetTranslation() const override;
	glm::vec3 GetDirection() const override;

	float GetYaw() const override;
	float GetPitch() const override;

	const Frustum* GetFrustum() const override;

	glm::mat4 GetViewMatrix() const override;
	glm::mat4 GetProjectionMatrix() const override;
	glm::mat4 GetViewProjectionInverseMatrix() const override;

	void SetPerspectiveProjection(EPerspectiveProjectionHand PerspectiveProjectionHand, float fovy, float aspect, float zNear, float zFar) override;
	void SetOrthographicProjection(float left, float right, float top, float bottom) override;



	//
	// ICameraMovement
	//

	// Translate
	void TranslateX(float x, ICameraMovement::Space space = ICameraMovement::Space::Local) override;
	void TranslateY(float y, ICameraMovement::Space space = ICameraMovement::Space::Local) override;
	void TranslateZ(float z, ICameraMovement::Space space = ICameraMovement::Space::Local) override;
	void DoMoveFront(float Value = 1.0f) override;
	void DoMoveBack(float Value = 1.0f) override;
	void DoMoveLeft(float Value = 1.0f) override;
	void DoMoveRight(float Value = 1.0f) override;
	void SetTranslate(vec3 Translate) override;
	void AddTranslate(vec3 Translate) override;

	// Rotate
	void SetYaw(float Yaw) override;
	void AddYaw(float Yaw) override;
	void SetPitch(float Pitch) override;
	void AddPitch(float Pitch) override;

protected:
	void UpdateLocalTransform() override;
	void UpdateWorldTransform() override;

protected:
	void EulerAnglesToDirectionVector();
	void DirectionVectorToEulerAngles();

	void UpdateViewMatrix();
	void UpdateViewProjectionInverse();

private:
	std::shared_ptr<CPropertyWrapped<glm::vec3>>    m_TranslateProperty;
	std::shared_ptr<CPropertyWrapped<float>>        m_Yaw_XProperty;
	std::shared_ptr<CPropertyWrapped<float>>        m_Pitch_YProperty;

	vec3                                            m_Translate;
	vec3                                            m_Direction;

	vec3                                            m_RightDirection;
	vec3                                            m_UpDirection;

	float                                           m_Yaw_X;
	float                                           m_Pitch_Y;

	Frustum                                         m_Frustum;

	mat4                                            m_ViewMatrix;
	mat4                                            m_ProjectionMatrix;
	mat4	                                        m_ViewProjectionInverse;

	bool                                            m_ViewDirty;
	bool		                                    m_ViewProjectionInverseDirty;

	// Perspective projection
	float                                           m_VFOV;
	float                                           m_Aspect;
	float                                           m_Near;
	float                                           m_Far;

	// Orthographic projection
	float                                           m_Left;
	float                                           m_Right;
	float                                           m_Top;
	float                                           m_Bottom;
};