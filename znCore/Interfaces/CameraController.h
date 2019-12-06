#pragma once

// Forward BEGIN
ZN_INTERFACE ICamera;
ZN_INTERFACE ICameraMovement;

class KeyEventArgs;
class MouseButtonEventArgs;
class MouseMotionEventArgs;
class MouseWheelEventArgs;
class ResizeEventArgs;
class UpdateEventArgs;
// Forward END

ZN_INTERFACE OW_ENGINE_API ICameraController
{
	virtual ~ICameraController() {}

	virtual std::shared_ptr<ICamera> GetCamera() const = 0;
	virtual std::shared_ptr<ICameraMovement> GetCameraMovement() const = 0;

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
