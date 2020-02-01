#include "stdafx.h"

// General
#include "CameraControllerBase.h"

CCameraControllerBase::CCameraControllerBase()
	: m_Camera(nullptr)
	, m_PreviousMousePosition(vec2(0.0f))
{
}

CCameraControllerBase::~CCameraControllerBase()
{
	printf("");
}



//
// ICameraController
//
void CCameraControllerBase::SetCamera(ICameraComponent3D* Camera)
{
	m_Camera = Camera;
}

ICameraComponent3D* CCameraControllerBase::GetCamera() const
{
	return m_Camera;
}

Ray CCameraControllerBase::ScreenPointToRay(const Viewport& Viewport, glm::vec2 screenPoint) const
{
	glm::vec4 clipPoint = glm::vec4(screenPoint, 1, 1);
	clipPoint.x = (screenPoint.x - Viewport.GetX()) / Viewport.GetWidth();
	clipPoint.y = 1.0f - (screenPoint.y - Viewport.GetY()) / Viewport.GetHeight();
	clipPoint = clipPoint * 2.0f - 1.0f;

	glm::vec3 p0 = m_Camera->GetTranslation();
	glm::vec4 worldSpace = m_Camera->GetInverseProjectionMatrix() * clipPoint;
	glm::vec3 p1 = glm::vec3(worldSpace / worldSpace.w);

	return Ray(p0, glm::normalize(p1 - p0));
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
	m_Camera->SetPerspectiveProjection(ICameraComponent3D::EPerspectiveProjectionHand::Right, 45.0f, static_cast<float>(e.Width) / static_cast<float>(e.Height), 0.5f, 10000.0f);
}



//
// Keyboard event
//
void CCameraControllerBase::OnKeyPressed(KeyEventArgs& e)
{
}

void CCameraControllerBase::OnKeyReleased(KeyEventArgs& e)
{
}



//
// Mouse event
//
void CCameraControllerBase::OnMouseButtonPressed(MouseButtonEventArgs& e)
{
	m_PreviousMousePosition = glm::vec2(e.X, e.Y);
}

void CCameraControllerBase::OnMouseButtonReleased(MouseButtonEventArgs& e)
{
	m_PreviousMousePosition = glm::vec2(0.0f, 0.0f);
}

void CCameraControllerBase::OnMouseMoved(MouseMotionEventArgs& e)
{
}

void CCameraControllerBase::OnMouseWheel(MouseWheelEventArgs& e)
{
}
