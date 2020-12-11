#pragma once

class ZN_API CCameraControllerBase 
	: public ICameraController
{
public:
	CCameraControllerBase();
    virtual	~CCameraControllerBase();

	// ICameraController
	void											SetCamera(const std::shared_ptr<ICameraComponent3D>& Camera) override;
	std::shared_ptr<ICameraComponent3D>             GetCamera() const override;

	Ray	                                            ScreenToRay(const Viewport& Viewport, const glm::vec2& screenPoint) const override;
	
	glm::vec3	                                    ScreenToWorld(const Viewport& Viewport, const glm::vec2& screenPoint) const override;
	glm::vec3                                       ScreenToPlane(const Viewport& Viewport, const glm::vec2& screenPoint, const Plane& Plane) const override;
	glm::vec3                                       RayToWorld(const Ray& Ray) const override;
	glm::vec3                                       RayToPlane(const Ray& Ray, const Plane& Plane) const override;

	glm::vec2										WorldToScreen(const Viewport& Viewport, const glm::vec3& WorldPoint) const override;

    // Engine events
    virtual void                                    OnUpdate(UpdateEventArgs& e) override;

    // Window events
    virtual void                                    OnResize(ResizeEventArgs& e) override;

	// Keyboard event
	virtual bool                                    OnKeyPressed(KeyEventArgs& e) override;
	virtual void                                    OnKeyReleased(KeyEventArgs& e) override;

    // Mouse event
	virtual bool                                    OnMouseButtonPressed(MouseButtonEventArgs& e) override;
	virtual void                                    OnMouseButtonReleased(MouseButtonEventArgs& e) override;
	virtual void                                    OnMouseMoved(MouseMotionEventArgs& e) override;
	virtual bool                                    OnMouseWheel(MouseWheelEventArgs& e) override;
	

protected:
	std::shared_ptr<ICameraComponent3D>				m_Camera;
	
	bool                                            m_IsMousePressed;
	glm::vec2                                       m_PreviousMousePosition;
};