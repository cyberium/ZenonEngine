#pragma once

class OW_ENGINE_API CCamera 
	: public ICamera
	, public ICameraMovement
	, public Object
{
public:
	CCamera();
	CCamera(float left, float right, float top, float bottom);
    CCamera(ProjectionHand ProjectionHand, float fovy, float aspect, float zNear, float zFar);
	virtual ~CCamera();


	//
	// IznCamera
	//
	const Frustum*                                  GetFrustum() const;
	vec3                                            GetTranslation() const;
	float                                           GetYaw() const;
	float                                           GetPitch() const;
	glm::vec3                                       GetDirection() const;
	mat4                                            GetViewMatrix() const override;
	mat4                                            GetProjectionMatrix() const override;
	mat4                                            GetViewProjectionInverseMatrix() const override;
	void                                            SetProjection(ProjectionHand ProjectionHand, float fovy, float aspect, float zNear, float zFar) override;
	void                                            SetOrthographic(float left, float right, float top, float bottom) override;

	//
	// ICameraMovement
	//

	// Translate
	void                                            TranslateX(float x, ICameraMovement::Space space = ICameraMovement::Space::Local) override;
	void                                            TranslateY(float y, ICameraMovement::Space space = ICameraMovement::Space::Local) override;
	void                                            TranslateZ(float z, ICameraMovement::Space space = ICameraMovement::Space::Local) override;
	void                                            DoMoveFront(float Value = 1.0f) override;
	void                                            DoMoveBack(float Value = 1.0f) override;
	void                                            DoMoveLeft(float Value = 1.0f) override;
	void                                            DoMoveRight(float Value = 1.0f) override;
	void                                            SetTranslate(vec3 Translate) override;
	void                                            AddTranslate(vec3 Translate) override;
	   
	// Rotate
    void                                            SetYaw(float Yaw) override;
    void                                            AddYaw(float Yaw) override;
    void                                            SetPitch(float Pitch) override;
    void                                            AddPitch(float Pitch) override;
	



protected:
    virtual void                                    UpdateDirections();

	virtual void                                    UpdateViewMatrix();
	virtual void                                    UpdateViewProjectionInverse();

protected:
	Frustum                                         m_Frustum;

	// Projection parameters
	float                                           m_VFOV;
	float                                           m_Aspect;
	float                                           m_Near;
	float                                           m_Far;

	// World space parameters
	vec3                                            m_Translate;

    vec3                                            m_Front;
    vec3                                            m_Right;
    vec3                                            m_Up;

    float                                           m_Yaw_X;
    float                                           m_Pitch_Y;

	mat4                                            m_ViewMatrix;
	mat4                                            m_ProjectionMatrix;
	mat4	                                        m_ViewProjectionInverse;

	bool                                            m_ViewDirty;
	bool		                                    m_ViewProjectionInverseDirty;
};