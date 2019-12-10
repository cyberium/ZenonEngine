#include "stdafx.h"

// General
#include "FreeCameraController.h"

CFreeCameraController::CFreeCameraController()
	: Forward(0.0f)
	, Back(0.0f)
	, Left(0.0f)
	, Right(0.0f)
	, Up(0.0f)
	, Down(0.0f)
	, Pitch(0.0f)
	, Yaw(0.0f)
	, m_PreviousPoint(vec3(0.0f))
	, TranslateFaster(false)
	, RotateFaster(false)
{
	m_Camera = std::make_shared<CCamera>();
}

CFreeCameraController::~CFreeCameraController()
{
}



//
// Events
//
void CFreeCameraController::OnUpdate(UpdateEventArgs& e)
{
	float moveMultiplier = (TranslateFaster) ? 100 : 50;

	GetCameraMovement()->DoMoveFront(Forward);
	GetCameraMovement()->DoMoveBack(Back);
	GetCameraMovement()->DoMoveLeft(Left);
	GetCameraMovement()->DoMoveRight(Right);
}

void CFreeCameraController::OnKeyPressed(KeyEventArgs& e)
{
	switch (e.Key)
	{
	case KeyCode::W:
	{
		Forward = 10.0f;
	}
	break;
	case KeyCode::A:
	{
		Left = 1.0f;
	}
	break;
	case KeyCode::S:
	{
		Back = 1.0f;
	}
	break;
	case KeyCode::D:
	{
		Right = 1.0f;
	}
	break;
	case KeyCode::Q:
	{
		Down = 1.0f;
	}
	break;
	case KeyCode::E:
	{
		Up = 1.0f;
	}
	break;
	case KeyCode::ShiftKey:
	{
		TranslateFaster = true;
		RotateFaster = true;
	}
	break;
	}

}

void CFreeCameraController::OnKeyReleased(KeyEventArgs& e)
{
	switch (e.Key)
	{
	case KeyCode::W:
	{
		Forward = 0.0f;
	}
	break;
	case KeyCode::A:
	{
		Left = 0.0f;
	}
	break;
	case KeyCode::S:
	{
		Back = 0.0f;
	}
	break;
	case KeyCode::D:
	{
		Right = 0.0f;
	}
	break;
	case KeyCode::Q:
	{
		Down = 0.0f;
	}
	break;
	case KeyCode::E:
	{
		Up = 0.0f;
	}
	break;
	case KeyCode::ShiftKey:
	{
		TranslateFaster = false;
		RotateFaster = false;
	}
	break;
	}
}

void CFreeCameraController::OnMouseMoved(MouseMotionEventArgs& e)
{
	if (e.LeftButton)
	{
        glm::vec2 newPoint = m_PreviousMousePosition - e.GetPoint();

        newPoint *= 0.33f;

        GetCameraMovement()->AddYaw(-newPoint.x);
		GetCameraMovement()->AddPitch(newPoint.y);

        m_PreviousMousePosition = e.GetPoint();
	}
}
