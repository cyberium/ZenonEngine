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
	, RollCW(0.0f)
	, RollCCW(0.0f)
	, Pitch(0.0f)
	, Yaw(0.0f)
	, m_PreviousPoint(vec3(0.0f))
	, TranslateFaster(false)
	, RotateFaster(false)
{
	m_Camera = std::make_shared<Camera>();
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
	//float rotateMultiplier = (RotateFaster) ? 600 : 300;

	m_Camera->TranslateX((Right - Left) * e.ElapsedTime * moveMultiplier);
	m_Camera->TranslateY((Up - Down) * e.ElapsedTime * moveMultiplier);
	m_Camera->TranslateZ((Back - Forward) * e.ElapsedTime * moveMultiplier);

	//m_Camera->AddPitch(Pitch * 60.0f * e.ElapsedTime * rotateMultiplier, Camera::Space::Local);
	//m_Camera->AddYaw(Yaw * 60.0f * e.ElapsedTime * rotateMultiplier, Camera::Space::World);
}

void CFreeCameraController::OnKeyPressed(KeyEventArgs& e)
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

void CFreeCameraController::OnMouseMoved(MouseMotionEventArgs& e)
{
	if (e.LeftButton)
	{
		glm::vec3 currentPoint = ProjectOntoUnitSphere(glm::ivec2(e.X, e.Y));

		m_Camera->AddRotation(glm::quat(currentPoint, m_PreviousPoint));



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

/*void CFreeCameraController::OnMouseWheel(MouseWheelEventArgs& e)
{
	//float fPivot = m_Camera->GetPivotDistance();
	//fPivot -= e.WheelDelta * (TranslateFaster ? 1.0f : 0.1f);
	//m_Camera->SetPivotDistance(fPivot);
}*/

vec3 CFreeCameraController::ProjectOntoUnitSphere(glm::ivec2 screenPos)
{
	const Viewport& viewport = m_Camera->GetViewport();

	// Map the screen coordinates so that (0, 0) is the center of the viewport.
	screenPos -= glm::vec2(viewport.Width, viewport.Height) * 0.5f;

	float x, y, z;
	// The radius of the unit sphere is 1/2 of the shortest dimension of the viewport.
	float radius = glm::min(viewport.Width, viewport.Height) * 0.5f;

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
		//if (m_PivotDistance <= 0.0f)
		{
			z = -z;
		}
	}

	return glm::vec3(x, y, z);
}
