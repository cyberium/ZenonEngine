#include "stdafx.h"

// General
#include "RTSCameraController.h"

CRTSCameraController::CRTSCameraController()
{
	m_Camera = std::make_shared<Camera>();
	m_Camera->SetRotate(vec3(-90.0f, 0.0f, 0.0f));

	m_MouseMoveMultiplier = 0.3f;
	m_MouseWheelMultiplier = 8.7f;
}

CRTSCameraController::~CRTSCameraController()
{
}



//
// RTS Camera stuff
//
void CRTSCameraController::SetBounds(BoundingBox Bounds)
{
	m_Bounds = Bounds;
	m_CameraPosition = Bounds.getCenter();
}

void CRTSCameraController::SetCameraPosition(glm::vec2 Position)
{
	m_CameraPosition = glm::vec3(Position.x, m_CameraPosition.y, Position.y);
}

void CRTSCameraController::SetCameraDistance(float Distance)
{
	vec3 BoundsPosition = m_Bounds.getCenter();
	BoundsPosition.y = Distance;

	if (m_Bounds.isPointInside(BoundsPosition))
	{
		m_CameraPosition.y = Distance;
	}
}



//
// Events
//
void CRTSCameraController::OnUpdate(UpdateEventArgs& e)
{
	m_Camera->SetTranslate(m_CameraPosition);
}

void CRTSCameraController::OnMouseMoved(MouseMotionEventArgs& e)
{
	if (e.LeftButton)
	{
		glm::vec2 currPoint = glm::vec2(e.X, e.Y);
		glm::vec2 diff = currPoint - m_PreviousMousePosition;
		diff *= m_MouseMoveMultiplier;
		diff = -diff;

		AddX(diff.x);
		AddZ(diff.y);

		m_PreviousMousePosition = currPoint;
	}
}

void CRTSCameraController::OnMouseWheel(MouseWheelEventArgs& e)
{
	float value = e.WheelDelta;
	value *= m_MouseWheelMultiplier;

	AddY(value);
}



//
// Positions verifiers
//
void CRTSCameraController::AddX(float Value)
{
	vec3 CameraPosition = m_CameraPosition;
	CameraPosition.x += Value;

	if (m_Bounds.isPointInside(CameraPosition))
	{
		m_CameraPosition = CameraPosition;
	}
}

void CRTSCameraController::AddY(float Value)
{
	vec3 CameraPosition = m_CameraPosition;
	CameraPosition.y += Value;

	if (m_Bounds.isPointInside(CameraPosition))
	{
		m_CameraPosition = CameraPosition;
	}
}

void CRTSCameraController::AddZ(float Value)
{
	vec3 CameraPosition = m_CameraPosition;
	CameraPosition.z += Value;

	if (m_Bounds.isPointInside(CameraPosition))
	{
		m_CameraPosition = CameraPosition;
	}
}
