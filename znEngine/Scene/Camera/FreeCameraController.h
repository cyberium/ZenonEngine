#pragma once

#include "CameraControllerBase.h"

class ZN_API CFreeCameraController 
	: public CCameraControllerBase
{
public:
	                                                CFreeCameraController();
	virtual                                         ~CFreeCameraController();

	// Engine event
	void                                            OnUpdate(UpdateEventArgs& e) override;

    // Keyboard events
	bool                                            OnKeyPressed(KeyEventArgs& e) override;
	void                                            OnKeyReleased(KeyEventArgs& e) override;

    // Mouse events
	void                                            OnMouseMoved(MouseMotionEventArgs& e) override;

private:
	glm::vec3 m_PreviousPoint;
	float Forward, Back, Left, Right, Up, Down;

    float Yaw;
    float Pitch;

	bool TranslateFaster;
	bool RotateFaster;
};