#include "stdafx.h"

// General
#include "CameraControllerBase.h"

CCameraControllerBase::CCameraControllerBase()
	: m_Camera(nullptr)
	, m_IsMousePressed(false)
	, m_PreviousMousePosition(glm::vec2(0.0f))
{
}

CCameraControllerBase::~CCameraControllerBase()
{
	printf("");
}



//
// ICameraController
//
void CCameraControllerBase::SetCamera(const std::shared_ptr<ICameraComponent3D>& Camera)
{
	m_Camera = Camera;
}

std::shared_ptr<ICameraComponent3D> CCameraControllerBase::GetCamera() const
{
	return m_Camera;
}

Ray CCameraControllerBase::ScreenToRay(const Viewport& Viewport, const glm::vec2& screenPoint) const
{
	glm::vec4 viewport = glm::vec4(Viewport.GetX(), Viewport.GetY(), Viewport.GetWidth(), Viewport.GetHeight());
	glm::vec3 nearSource = glm::vec3((float)screenPoint.x, (float)Viewport.GetHeight() - (float)screenPoint.y, 0.0f);
	glm::vec3 farSource = glm::vec3((float)screenPoint.x, (float)Viewport.GetHeight() - (float)screenPoint.y, 1.0f);
	glm::vec3 nearPoint = glm::unProject(nearSource, GetCamera()->GetViewMatrix(), GetCamera()->GetProjectionMatrix(), viewport);
	glm::vec3 farPoint = glm::unProject(farSource, GetCamera()->GetViewMatrix(), GetCamera()->GetProjectionMatrix(), viewport);

	glm::vec3 direction = farPoint - nearPoint;
	direction = glm::normalize(direction);

	return Ray(nearPoint, direction);
}

glm::vec3 CCameraControllerBase::ScreenToWorld(const Viewport& Viewport, const glm::vec2& screenPoint) const
{
	Ray ray = ScreenToRay(Viewport, screenPoint);

	glm::vec3 n = glm::vec3(0.f, 1.f, 0.f);
	Plane p = Plane(n, 0.f);

	// Calculate distance of intersection point from r.origin.
	float denominator = glm::dot(p.normal, ray.GetDirection());
	float numerator = glm::dot(p.normal, ray.GetOrigin()) + p.dist;
	float t = -(numerator / denominator);

	return ray.GetOrigin() + ray.GetDirection() * t;
}

glm::vec3 CCameraControllerBase::ScreenToPlane(const Viewport & Viewport, const glm::vec2 & screenPoint, const Plane& Plane) const
{
	return RayToPlane(ScreenToRay(Viewport, screenPoint), Plane);
}

glm::vec3 CCameraControllerBase::RayToWorld(const Ray& Ray) const
{
	glm::vec3 n = glm::vec3(0.f, 1.f, 0.f);
	Plane p = Plane(n, 0.f);

	// Calculate distance of intersection point from r.origin.
	float denominator = glm::dot(p.normal, Ray.GetDirection());
	float numerator = glm::dot(p.normal, Ray.GetOrigin()) + p.dist;
	float t = -(numerator / denominator);

	return Ray.GetOrigin() + Ray.GetDirection() * t;
}

glm::vec3 CCameraControllerBase::RayToPlane(const Ray & Ray, const Plane & Plane) const
{
	float denominator = glm::dot(Plane.normal, Ray.GetDirection());
	if (glm::abs(denominator) > 0.0001f)
	{
		float t = glm::dot(Plane.GetCenter() - Ray.GetOrigin(), Plane.normal) / denominator;
		if (t >= 0)
			return Ray.GetOrigin() + Ray.GetDirection() * t;
	}

	return glm::vec3(Math::MinFloat);
}


//
// Events
//
void CCameraControllerBase::OnUpdate(UpdateEventArgs& e)
{
	// TODO: Refactor me!!!
	GetCamera()->GetViewMatrix();
}



//
// Window events
//
void CCameraControllerBase::OnResize(ResizeEventArgs& e)
{
	m_Camera->SetPerspectiveProjection(ICameraComponent3D::EPerspectiveProjectionHand::Right, 75.0f, static_cast<float>(e.Width) / static_cast<float>(e.Height), 1.0f, 10000.0f);
}



//
// Keyboard event
//
bool CCameraControllerBase::OnKeyPressed(KeyEventArgs& e)
{
	return false;
}

void CCameraControllerBase::OnKeyReleased(KeyEventArgs& e)
{
}



//
// Mouse event
//
bool CCameraControllerBase::OnMouseButtonPressed(MouseButtonEventArgs& e)
{
	if (e.Button != MouseButton::Right)
		return false;

	m_PreviousMousePosition = glm::vec2(e.X, e.Y);
	m_IsMousePressed = true;
	return true;
}

void CCameraControllerBase::OnMouseButtonReleased(MouseButtonEventArgs& e)
{
	m_PreviousMousePosition = glm::vec2(0.0f, 0.0f);
	m_IsMousePressed = false;
}

void CCameraControllerBase::OnMouseMoved(MouseMotionEventArgs& e)
{
}

bool CCameraControllerBase::OnMouseWheel(MouseWheelEventArgs& e)
{
	return false;
}
