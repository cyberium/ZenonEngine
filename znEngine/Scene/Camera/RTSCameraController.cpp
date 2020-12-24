#include "stdafx.h"

// General
#include "RTSCameraController.h"

CRTSCameraController::CRTSCameraController()
{
	SetBounds(BoundingBox(glm::vec3(-5.0f * 64.0f), glm::vec3(15.0f * 64.0f)));
}

CRTSCameraController::~CRTSCameraController()
{
}

void CRTSCameraController::SetCamera(const std::shared_ptr<ICameraComponent3D>& Camera)
{
	Camera->SetYaw(0.0f);
	Camera->SetPitch(-75.0f);

	m_MouseMoveMultiplier = 0.2f;
	m_MouseWheelMultiplier = 8.7f;

	__super::SetCamera(Camera);
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
	glm::vec3 BoundsPosition = m_Bounds.getCenter();
	BoundsPosition.y = Distance;

	if (m_Bounds.isPointInside(BoundsPosition))
	{
		m_CameraPosition.y = Distance;
	}
}



//
// Engine events
//
void CRTSCameraController::OnUpdate(UpdateEventArgs& e)
{
	
}



//
// Mouse events
//
void CRTSCameraController::OnMouseMoved(MouseMotionEventArgs& e)
{
	if (false == m_IsMousePressed)
		return;

	if (false == e.RightButton)
		return;

	glm::vec2 currPoint = glm::vec2(e.X, e.Y);
	glm::vec2 diff = currPoint - m_PreviousMousePosition;
	diff *= m_MouseMoveMultiplier;

	AddX(diff.y);
	AddZ(-diff.x);

	m_PreviousMousePosition = currPoint;

	//GetCamera()->SetTranslation(m_CameraPosition);
}

bool CRTSCameraController::OnMouseWheel(MouseWheelEventArgs& e)
{
	if (false == m_IsMousePressed)
		return false;

	float value = e.WheelDelta / 100.0f;
	value *= m_MouseWheelMultiplier;
	AddY(value);
	//GetCamera()->SetTranslation(m_CameraPosition);

	return true;
}



//
// Positions verifiers
//
void CRTSCameraController::AddX(float Value)
{
	glm::vec3 CameraPosition = m_CameraPosition;
	CameraPosition.x += Value;

	if (m_Bounds.isPointInside(CameraPosition))
	{
		m_CameraPosition = CameraPosition;
	}
}

void CRTSCameraController::AddY(float Value)
{
	glm::vec3 CameraPosition = m_CameraPosition;
	CameraPosition.y += Value;

	if (m_Bounds.isPointInside(CameraPosition))
	{
		m_CameraPosition = CameraPosition;
	}
}

void CRTSCameraController::AddZ(float Value)
{
	glm::vec3 CameraPosition = m_CameraPosition;
	CameraPosition.z += Value;

	if (m_Bounds.isPointInside(CameraPosition))
	{
		m_CameraPosition = CameraPosition;
	}
}
