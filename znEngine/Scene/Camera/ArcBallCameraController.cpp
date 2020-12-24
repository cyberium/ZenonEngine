#include "stdafx.h"

// General
#include "ArcBallCameraController.h"

CArcBallCameraController::CArcBallCameraController()
	: Forward(0.0f)
	, Back(0.0f)
	, Left(0.0f)
	, Right(0.0f)
	, Up(0.0f)
	, Down(0.0f)
	, RollCW(0.0f)
	, RollCCW(0.0f)
	, Pitch(0.0f)
	, Yaw(0.0f)
	, PivotTranslate(0.0f)
	, TranslateFaster(false)
	, RotateFaster(false)
{
}

CArcBallCameraController::~CArcBallCameraController()
{
}


//
// Pivot
//
void CArcBallCameraController::SetPivotDistance(float pivotDistance)
{
	m_PivotDistance = std::max(pivotDistance, 0.0f);
}

float CArcBallCameraController::GetPivotDistance() const
{
	return m_PivotDistance;
}

glm::vec3 CArcBallCameraController::GetPivotPoint() const
{
	return m_Camera->GetPosition();
}



//
// Engine events
//
void CArcBallCameraController::OnUpdate(UpdateEventArgs& e)
{
	float moveMultiplier = (TranslateFaster) ? 100.0 : 50.0;
	//float rotateMultiplier = (RotateFaster) ? 600 : 300;

#if 0
	GetCamera()->TranslateX((Right - Left) * e.DeltaTime * moveMultiplier);
	GetCamera()->TranslateY((Up - Down) * e.DeltaTime * moveMultiplier);
	GetCamera()->TranslateZ((Back - Forward) * e.DeltaTime * moveMultiplier);
#endif

	//m_Camera->AddPitch(Pitch * 60.0f * e.DeltaTime * rotateMultiplier, Camera::Space::Local);
	//m_Camera->AddYaw(Yaw * 60.0f * e.DeltaTime * rotateMultiplier, Camera::Space::World);
}



//
// Keyboards events
//
bool CArcBallCameraController::OnKeyPressed(KeyEventArgs& e)
{
	switch (e.Key)
	{
		case KeyCode::W:
		{
			Forward = 1.0f;
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
		default:
			return false;
	}

	return true;
}

void CArcBallCameraController::OnKeyReleased(KeyEventArgs& e)
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
	case KeyCode::R:
	{
		RollCW = 0.0f;
	}
	break;
	case KeyCode::F:
	{
		RollCCW = 0.0f;
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

bool CArcBallCameraController::OnMouseButtonPressed(MouseButtonEventArgs& e)
{
	//m_PreviousPoint = glm::ivec2(e.X, e.Y);
	m_PreviousPoint = ProjectOntoUnitSphere(glm::ivec2(e.X, e.Y));

	m_PreviousMousePosition = glm::vec2(e.X, e.Y);

	return true;
}

void CArcBallCameraController::OnMouseButtonReleased(MouseButtonEventArgs& e)
{
	glm::vec2 currentMousePosition = glm::vec2(e.X, e.Y);
	float offset = glm::distance(m_PreviousMousePosition, currentMousePosition);
}

void CArcBallCameraController::OnMouseMoved(MouseMotionEventArgs& e)
{
	if (e.LeftButton)
	{
		glm::vec3 currentPoint = ProjectOntoUnitSphere(glm::ivec2(e.X, e.Y));

		//m_Camera->AddRotation(glm::quat(currentPoint, m_PreviousPoint));



		//// Compute the axis of rotation.
		/*glm::vec3 axis = glm::cross( currentPoint, m_PreviousPoint );

		float length_sqr = glm::length2( axis );

		//// If the rotation axis is too short, don't rotate.
		if ( length_sqr > 0.0f )
		{
			// Normalize the axis of rotation
			//axis *= glm::inversesqrt( length_sqr );

			// The dot product between the two vectors gives the angle of rotation.
			float dotProduct = glm::dot( m_PreviousPoint, currentPoint );
			if ( dotProduct <= 1 && dotProduct >= -1 )
			{
				// Add the resulting rotation to our current rotation
				glm::quat deltaRotate = glm::angleAxis( glm::acos( dotProduct ), glm::normalize(axis) );
				AddRotation( deltaRotate );
			}
		}*/

		m_PreviousPoint = currentPoint;
	}
}

glm::vec3 CArcBallCameraController::ProjectOntoUnitSphere(glm::ivec2 screenPos)
{
	//TODO: const Viewport * viewport = m_Camera->GetViewport();

	// Map the screen coordinates so that (0, 0) is the center of the viewport.
	// TODO: screenPos -= glm::vec2(viewport->GetWidth(), viewport->GetHeight()) * 0.5f;

	float x = 0.0f, y = 0.0f, z = 0.0f;
	// The radius of the unit sphere is 1/2 of the shortest dimension of the viewport.
	/*float radius = glm::min(viewport->GetWidth(), viewport->GetHeight()) * 0.5f;

	// Now normalize the screen coordinates into the range [-1 .. 1].
	x = screenPos.x / radius;
	// The y-coordinate has to be inverted so that +1 is the top of the unit sphere
	// and -1 is the bottom of the unit sphere.
	y = -(screenPos.y / radius);

	float length_sqr = (x * x) + (y * y);

	// If the screen point is mapped outside of the unit sphere
	if (length_sqr > 1.0f)
	{
		float invLength = glm::inversesqrt(length_sqr);

		// Return the normalized point that is closest to the outside of the unit sphere.
		x *= invLength;
		y *= invLength;
		z = 0.0f;
	}
	else
	{
		// The point is on the inside of the unit sphere.
		z = glm::sqrt(1.0f - length_sqr);
		// If we are "inside" the unit sphere, then 
		// invert the z component.
		// In a right-handed coordinate system, the "+z" axis of the unit
		// sphere points towards the viewer. If we are in the Unit sphere, we
		// want to project the point to the inside of the sphere and in this case
		// the z-axis we want to project on points away from the viewer (-z).
		if (m_PivotDistance <= 0.0f)
		{
			z = -z;
		}
	}*/

	return glm::vec3(x, y, z);
}
