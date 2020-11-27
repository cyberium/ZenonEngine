#pragma once

#include <functional>

// FORWARD BEGIN
ZN_INTERFACE IPipelineState;
ZN_INTERFACE ICameraComponent3D;

ZN_INTERFACE ISceneNode;
ZN_INTERFACE IUIControl;
// FORWARD END


class ZN_API EventArgs
{
public:
	EventArgs()	{}
};


class ZN_API WindowCloseEventArgs : public EventArgs
{
public:
	WindowCloseEventArgs()
		: ConfirmClose(true)
	{}

	// The user can cancel a window closing operating by registering for the 
	// Window::Close event on the window and setting the ConfirmClose to false if
	// the window should be kept open (for example, there are unsaved changes 
	// made and closing the window would cause those changes to be lost).
	// By default, the window will be destoryed if the Window::Close even is not handled.
	bool ConfirmClose;
};

enum class ZN_API KeyState
{
	Released = 0,
	Pressed = 1
};

class ZN_API KeyEventArgs : public EventArgs
{
public:
	KeyEventArgs(KeyCode key, uint32 c, KeyState state, bool control, bool shift, bool alt)
		: Key(key)
		, Char(c)
		, State(state)
		, Control(control)
		, Shift(shift)
		, Alt(alt)
	{}

	KeyCode         Key;    // The Key Code that was pressed or released.
	uint32          Char;   // The 32-bit character code that was pressed. This value will be 0 if it is a non-printable character.
	KeyState        State;  // Was the key pressed or released?
	bool            Control;// Is the Control modifier pressed
	bool            Shift;  // Is the Shift modifier pressed
	bool            Alt;    // Is the Alt modifier pressed
};


class ZN_API MouseMotionEventArgs : public EventArgs
{
public:
	MouseMotionEventArgs(bool leftButton, bool middleButton, bool rightButton, bool control, bool shift, int x, int y)
		: LeftButton(leftButton)
		, MiddleButton(middleButton)
		, RightButton(rightButton)
		, Control(control)
		, Shift(shift)
		, X(x)
		, Y(y)
	{}

	bool LeftButton;    // Is the left mouse button down?
	bool MiddleButton;  // Is the middle mouse button down?
	bool RightButton;   // Is the right mouse button down?
	bool Control;       // Is the CTRL key down?
	bool Shift;         // Is the Shift key down?

	int X;              // The X-position of the cursor relative to the upper-left corner of the client area.
	int Y;              // The Y-position of the cursor relative to the upper-left corner of the client area.
	int RelX;			// How far the mouse moved since the last event.
	int RelY;			// How far the mouse moved since the last event.

	glm::vec2 GetPoint() const { return glm::vec2(X, Y); }
};

enum class ZN_API MouseButton
{
	None = 0,
	Left = 1,
	Right = 2,
	Middel = 3
};
enum class ZN_API ButtonState
{
	Released = 0,
	Pressed = 1
};

class ZN_API MouseButtonEventArgs : public EventArgs
{
public:
	MouseButtonEventArgs(MouseButton buttonID, ButtonState state, bool leftButton, bool middleButton, bool rightButton, bool control, bool shift, bool alt, int x, int y)
		: Button(buttonID)
		, State(state)
		, LeftButton(leftButton)
		, MiddleButton(middleButton)
		, RightButton(rightButton)
		, Control(control)
		, Shift(shift)
		, Alt(alt)
		, X(x)
		, Y(y)
	{}

	MouseButton Button; // The mouse button that was pressed or released.
	ButtonState State;  // Was the button pressed or released?
	bool LeftButton;    // Is the left mouse button down?
	bool MiddleButton;  // Is the middle mouse button down?
	bool RightButton;   // Is the right mouse button down?
	bool Control;       // Is the CTRL key down?
	bool Shift;         // Is the Shift key down?
	bool Alt;           // Is the Alt key down?

	int X;              // The X-position of the cursor relative to the upper-left corner of the client area.
	int Y;              // The Y-position of the cursor relative to the upper-left corner of the client area.

	glm::vec2 GetPoint() const { return glm::vec2(X, Y); }
};


class ZN_API MouseWheelEventArgs : public EventArgs
{
public:
	MouseWheelEventArgs(float wheelDelta, bool leftButton, bool middleButton, bool rightButton, bool control, bool shift, int x, int y)
		: WheelDelta(wheelDelta)
		, LeftButton(leftButton)
		, MiddleButton(middleButton)
		, RightButton(rightButton)
		, Control(control)
		, Shift(shift)
		, X(x)
		, Y(y)
	{}

	float WheelDelta;   // How much the mouse wheel has moved. A positive value indicates that the wheel was moved to the right. A negative value indicates the wheel was moved to the left.
	bool LeftButton;    // Is the left mouse button down?
	bool MiddleButton;  // Is the middle mouse button down?
	bool RightButton;   // Is the right mouse button down?
	bool Control;       // Is the CTRL key down?
	bool Shift;         // Is the Shift key down?

	int X;              // The X-position of the cursor relative to the upper-left corner of the client area.
	int Y;              // The Y-position of the cursor relative to the upper-left corner of the client area.

	glm::vec2 GetPoint() const { return glm::vec2(X, Y); }

};


class ZN_API ResizeEventArgs : public EventArgs
{
public:
	ResizeEventArgs(int width, int height)
		: Width(width)
		, Height(height)
	{}

	int Width;
	int Height;

	glm::ivec2 GetSize() const { return glm::ivec2(Width, Height); }
};


class ZN_API UpdateEventArgs 
	: public EventArgs
{
public:
	UpdateEventArgs
	(
		double DeltaTime,
		double TotalTime,
		uint64_t FrameCounter
	)
		: DeltaTime(DeltaTime)
		, TotalTime(TotalTime)
		, FrameCounter(FrameCounter)
		, Camera(Camera)
		, CameraForCulling(CameraForCulling)
	{
		DeltaTimeMultiplier = DeltaTime / 16.6666666667;
		if (DeltaTime == TotalTime)
			DeltaTimeMultiplier = 1.0; // First frame
	}

	double                                          DeltaTime;
	double                                          DeltaTimeMultiplier;
	double                                          TotalTime;
	int64_t                                         FrameCounter;

	// Engine specific. TODO: Replace me!
	const ICameraComponent3D*                       Camera;
	const ICameraComponent3D*                       CameraForCulling;
};


class ZN_API RenderEventArgs 
	: public UpdateEventArgs
{
public:
	RenderEventArgs
	(
		const UpdateEventArgs& UpdateEventArgs,
		const IPipelineState* PipelineState
	)
		: UpdateEventArgs(UpdateEventArgs)
		, PipelineState(PipelineState)
	{}

	// Engine specific. TODO: Replace me!
	const IPipelineState*                           PipelineState;
};


class ZN_API UserEventArgs : public EventArgs
{
public:
	UserEventArgs(int code, void* data1, void* data2)
		: Code(code)
		, Data1(data1)
		, Data2(data2)
	{}

	int     Code;
	void*   Data1;
	void*   Data2;
};


// FORWARD BEGIN
ZN_INTERFACE IScene;
// FORWARD END

class ZN_API SceneEventArgs : public EventArgs
{
public:
	SceneEventArgs(const IScene* Scene)
		: Scene(Scene)
	{}

	const IScene* Scene;
};


enum class ZN_API ESceneChangeType
{
	NodeAddedToParent = 0,
	NodeRemovedFromParent
};

class ZN_API SceneChangeEventArgs : public SceneEventArgs
{
public:
	SceneChangeEventArgs(const IScene* Scene, ESceneChangeType SceneChangeType, const std::shared_ptr<ISceneNode>& OwnerNode, const std::shared_ptr<ISceneNode>& ChildNode)
		: SceneEventArgs(Scene)
		, SceneChangeType(SceneChangeType)
		, OwnerNode(OwnerNode)
		, ChildNode(ChildNode)
	{}

	const ESceneChangeType SceneChangeType;
	std::shared_ptr<ISceneNode> OwnerNode;
	std::shared_ptr<ISceneNode> ChildNode;
};
