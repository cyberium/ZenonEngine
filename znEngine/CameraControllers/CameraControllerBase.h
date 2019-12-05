#pragma once

#include "Camera.h"

class OW_ENGINE_API CCameraControllerBase 
	: public ICameraController
{
public:
	                                                CCameraControllerBase();
    virtual                                         ~CCameraControllerBase();

	// ICameraController
	std::shared_ptr<ICamera>                        GetCamera() const override;
	std::shared_ptr<ICameraMovement>                GetCameraMovement() const override;

    // Engine events
    virtual void                                    OnUpdate(UpdateEventArgs& e) override;

    // Window events
    virtual void                                    OnResize(ResizeEventArgs& e) override;

	// Keyboard event
	virtual void                                    OnKeyPressed(KeyEventArgs& e) override;
	virtual void                                    OnKeyReleased(KeyEventArgs& e) override;

    // Mouse event
	virtual void                                    OnMouseButtonPressed(MouseButtonEventArgs& e) override;
	virtual void                                    OnMouseButtonReleased(MouseButtonEventArgs& e) override;
	virtual void                                    OnMouseMoved(MouseMotionEventArgs& e) override;
	virtual void                                    OnMouseWheel(MouseWheelEventArgs& e) override;
	
	// Helpers
	Ray	                                            ScreenPointToRay(vec2 screenPoint) const;

protected:
	std::shared_ptr<ICamera>                        m_Camera;
	
	vec2                                            m_PreviousMousePosition;
};