#pragma once

#include "CameraControllerBase.h"

class CRTSCameraController : public CCameraControllerBase
{
public:
	CRTSCameraController();
	~CRTSCameraController();

	// RTS camera
	void SetBounds(BoundingBox Bounds);
	void SetCameraPosition(glm::vec2 Position);
	void SetCameraDistance(float Distance);

	// Events
	void OnUpdate(UpdateEventArgs& e) override;
	void OnMouseMoved(MouseMotionEventArgs& e) override;
	void OnMouseWheel(MouseWheelEventArgs& e) override;

private:
	void AddX(float Value);
	void AddY(float Value);
	void AddZ(float Value);

private:
	BoundingBox              m_Bounds;

	// Multipliers
	float                    m_MouseMoveMultiplier;
	float                    m_MouseWheelMultiplier;
};