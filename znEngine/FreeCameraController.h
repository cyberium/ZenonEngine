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

protected:
	vec3 ProjectOntoUnitSphere(glm::ivec2 screenPos);

private:
	vec3 m_PreviousPoint;
	float Forward, Back, Left, Right, Up, Down;

	float RollCW, RollCCW;
	float Pitch, Yaw;

	bool TranslateFaster;
	bool RotateFaster;
};