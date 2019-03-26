#pragma once

#include "CameraControllerBase.h"

class CFreeCameraController : public CCameraControllerBase
{
public:
	CFreeCameraController();
	~CFreeCameraController();

	// Input
	void OnUpdate(UpdateEventArgs& e) override;
	void OnKeyPressed(KeyEventArgs& e) override;
	void OnKeyReleased(KeyEventArgs& e) override;
	void OnMouseMoved(MouseMotionEventArgs& e) override;

private:
	vec3 m_PreviousPoint;
	float Forward, Back, Left, Right, Up, Down;

    float Yaw;
    float Pitch;

	bool TranslateFaster;
	bool RotateFaster;
};