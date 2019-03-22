#pragma once

class CCameraControllerBase : public ICameraController
{
public:
	CCameraControllerBase();

	// ICameraController
	std::shared_ptr<Camera> GetCamera() const override;

	// Input events
	virtual void OnKeyPressed(KeyEventArgs& e) override;
	virtual void OnKeyReleased(KeyEventArgs& e) override;
	virtual void OnMouseButtonPressed(MouseButtonEventArgs& e) override;
	virtual void OnMouseButtonReleased(MouseButtonEventArgs& e) override;
	virtual void OnMouseMoved(MouseMotionEventArgs& e) override;
	virtual void OnMouseWheel(MouseWheelEventArgs& e) override;
	
	// Update events
	virtual void OnUpdate(UpdateEventArgs& e) override;

	// Helpers
	Ray	ScreenPointToRay(vec2 screenPoint) const;

protected:
	std::shared_ptr<Camera>  m_Camera;
	glm::vec3                m_CameraPosition;

	// Events variables
	vec2                     m_PreviousMousePosition;


};