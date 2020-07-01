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
	glm::vec3                                       RayToPlane(const Ray& Ray, const Plane& Plane) const override;

    // Engine events
    virtual void                                    OnUpdate(UpdateEventArgs& e) override;

    // Window events
    virtual void                                    OnResize(ResizeEventArgs& e) override;

	// Keyboard event
	virtual void                                    OnKeyPressed(KeyEventArgs& e) override;
	virtual void                                    OnKeyReleased(KeyEventArgs& e) override;

    // Mouse event
	virtual void                                    OnMouseButtonPressed(MouseButtonEventArgs& e) override;
	virtual void                                    OnMouseButtonReleased(MouseButtonEventArgs& e) override;
	virtual void                                    OnMouseMoved(MouseMotionEventArgs& e) override;
	virtual void                                    OnMouseWheel(MouseWheelEventArgs& e) override;
	

protected:
	std::shared_ptr<ICameraComponent3D>				m_Camera;
	
	glm::vec2                                       m_PreviousMousePosition;
};