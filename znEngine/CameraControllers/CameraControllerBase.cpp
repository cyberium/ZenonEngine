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
}



//
// ICameraController
//
std::shared_ptr<ICamera> CCameraControllerBase::GetCamera() const
{
	return m_Camera;
}

std::shared_ptr<ICameraMovement> CCameraControllerBase::GetCameraMovement() const
{
	return std::dynamic_pointer_cast<ICameraMovement>(GetCamera());
}



//
// Events
//
void CCameraControllerBase::OnUpdate(UpdateEventArgs& e)
{
}



//
// Window events
//
void CCameraControllerBase::OnResize(ResizeEventArgs& e)
{
	m_Camera->SetProjection(ICamera::ProjectionHand::Right, 45.0f, static_cast<float>(e.Width) / static_cast<float>(e.Height), 0.5f, 4000.0f);
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



//
// Helpers
//
Ray CCameraControllerBase::ScreenPointToRay(vec2 screenPoint) const
{
	mat4 clipToWorld = m_Camera->GetViewProjectionInverseMatrix();
	vec4 clipPoint = vec4(screenPoint, 1, 1);
	clipPoint.x = (screenPoint.x - /*m_Camera->GetViewport()->GetX()*/ 0); /// m_Camera->GetViewport()->GetWidth();
	clipPoint.y = 1.0f - (screenPoint.y - /*m_Camera->GetViewport()->GetY() */ 0);// / m_Camera->GetViewport()->GetHeight();
	clipPoint = clipPoint * 2.0f - 1.0f;

	vec3 p0 = m_Camera->GetTranslation();
	vec4 worldSpace = clipToWorld * clipPoint;
	vec3 p1 = vec3(worldSpace / worldSpace.w); // glm::unProject( vec3( screenPoint, 0), m_ViewMatrix, m_ProjectionMatrix, vec4( m_Viewport.X, m_Viewport.Y, m_Viewport.Width, m_Viewport.Height ) );

	return Ray(p0, glm::normalize(p1 - p0));
}
