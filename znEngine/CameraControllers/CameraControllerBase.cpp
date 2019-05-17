#include "stdafx.h"

// General
#include "CameraControllerBase.h"




CCameraControllerBase::CCameraControllerBase()
	: m_Camera(nullptr)
	, m_CameraPosition(vec3(0.0f))
	, m_PreviousMousePosition(vec2(0.0f))
{
}

//
// ICameraController
//
std::shared_ptr<Camera> CCameraControllerBase::GetCamera() const
{
	return m_Camera;
}



//
// Events
//
void CCameraControllerBase::OnUpdate(UpdateEventArgs& e)
{}

void CCameraControllerBase::OnKeyPressed(KeyEventArgs& e)
{}

void CCameraControllerBase::OnKeyReleased(KeyEventArgs& e)
{}

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
	clipPoint.x = (screenPoint.x - m_Camera->GetViewport().X) / m_Camera->GetViewport().Width;
	clipPoint.y = 1.0f - (screenPoint.y - m_Camera->GetViewport().Y) / m_Camera->GetViewport().Height;
	clipPoint = clipPoint * 2.0f - 1.0f;

	vec3 p0 = m_Camera->GetTranslation();
	vec4 worldSpace = clipToWorld * clipPoint;
	vec3 p1 = vec3(worldSpace / worldSpace.w); // glm::unProject( vec3( screenPoint, 0), m_ViewMatrix, m_ProjectionMatrix, vec4( m_Viewport.X, m_Viewport.Y, m_Viewport.Width, m_Viewport.Height ) );

	return Ray(p0, glm::normalize(p1 - p0));
}