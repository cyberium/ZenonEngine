#pragma once

#include "CameraControllerBase.h"

class CArcBallCameraController : public CCameraControllerBase
{
public:
	CArcBallCameraController();
	~CArcBallCameraController();

	// Pivot
	void  SetPivotDistance(float pivotDistance);
	float GetPivotDistance() const;
	vec3  GetPivotPoint() const;

	// Input
	void OnUpdate(UpdateEventArgs& e);
	void OnKeyPressed(KeyEventArgs& e);
	void OnKeyReleased(KeyEventArgs& e);
	void OnMouseButtonPressed(MouseButtonEventArgs& e);
	void OnMouseButtonReleased(MouseButtonEventArgs& e);
	void OnMouseMoved(MouseMotionEventArgs& e);

protected:
	vec3 ProjectOntoUnitSphere(glm::ivec2 screenPos);

private:
	// Translation movement
	float Forward, Back, Left, Right, Up, Down;

	// Rotation movement
	float RollCW, RollCCW;
	float Pitch, Yaw;

	// Move in/out from pivot point.
	float PivotTranslate;

	// Used for arcball camera
	glm::vec3   m_PreviousPoint;
	float       m_PivotDistance;

	// Do you want to go faster?
	bool TranslateFaster;
	bool RotateFaster;
};