#pragma once

// Forward BEGIN
ZN_INTERFACE ICameraComponent3D;

class Ray;
class Viewport;

class KeyEventArgs;
class MouseButtonEventArgs;
class MouseMotionEventArgs;
class MouseWheelEventArgs;
class ResizeEventArgs;
class UpdateEventArgs;
// Forward END

ZN_INTERFACE ZN_API ICameraController
{
	virtual ~ICameraController() {}

	virtual void SetCamera(ICameraComponent3D* Camera) = 0;

	virtual ICameraComponent3D* GetCamera() const = 0;
	virtual Ray ScreenPointToRay(const Viewport& Viewport, glm::vec2 screenPoint) const = 0;

	// Input events
	virtual void OnKeyPressed(KeyEventArgs& e) = 0;
	virtual void OnKeyReleased(KeyEventArgs& e) = 0;
	virtual void OnMouseButtonPressed(MouseButtonEventArgs& e) = 0;
	virtual void OnMouseButtonReleased(MouseButtonEventArgs& e) = 0;
	virtual void OnMouseMoved(MouseMotionEventArgs& e) = 0;
	virtual void OnMouseWheel(MouseWheelEventArgs& e) = 0;

	// Window events
	virtual void OnResize(ResizeEventArgs& e) = 0;

	// Update events
	virtual void OnUpdate(UpdateEventArgs& e) = 0;
};
