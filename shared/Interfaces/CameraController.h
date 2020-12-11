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

	virtual void SetCamera(const std::shared_ptr<ICameraComponent3D>& Camera) = 0;

	virtual std::shared_ptr<ICameraComponent3D> GetCamera() const = 0;
	virtual Ray ScreenToRay(const Viewport& Viewport, const glm::vec2& screenPoint) const = 0;
	
	// To world position
	virtual glm::vec3 ScreenToWorld(const Viewport& Viewport, const glm::vec2& screenPoint) const = 0;
	virtual glm::vec3 ScreenToPlane(const Viewport& Viewport, const glm::vec2& screenPoint, const Plane& Plane) const = 0;
	virtual glm::vec3 RayToWorld(const Ray& Ray) const = 0;
	virtual glm::vec3 RayToPlane(const Ray& Ray, const Plane& Plane) const = 0;

	// To screen position
	virtual glm::vec2 WorldToScreen(const Viewport& Viewport, const glm::vec3& WorldPoint) const = 0;

	// Input events
	virtual bool OnKeyPressed(KeyEventArgs& e) = 0;
	virtual void OnKeyReleased(KeyEventArgs& e) = 0;
	virtual bool OnMouseButtonPressed(MouseButtonEventArgs& e) = 0;
	virtual void OnMouseButtonReleased(MouseButtonEventArgs& e) = 0;
	virtual void OnMouseMoved(MouseMotionEventArgs& e) = 0;
	virtual bool OnMouseWheel(MouseWheelEventArgs& e) = 0;

	// Window events
	virtual void OnResize(ResizeEventArgs& e) = 0;

	// Update events
	virtual void OnUpdate(UpdateEventArgs& e) = 0;
};
