#pragma once

#include <functional>

// FORWARD BEGIN
ZN_INTERFACE IPipelineState;
ZN_INTERFACE ICameraComponent3D;
ZN_INTERFACE IScene;
ZN_INTERFACE ISceneNode;
// FORWARD END

template<class ArgumentType>
class Delegate
{
public:
	typedef std::function<void(ArgumentType&)> FunctionType;
	typedef std::shared_ptr<FunctionType> FunctionDecl;
	typedef std::set<FunctionDecl> FunctionsSet;

    template<class _Fx,  class... _Types>
    FunctionDecl connect(_Fx&& _Func, _Types&&... _Args)
    {
        // https://stackoverflow.com/questions/20588191/error-with-variadiac-template-parameter-pack-must-be-expanded
        FunctionDecl ret = std::make_shared<FunctionType>(std::bind(_Func, std::forward<_Types>(_Args)...));
        m_Functions.insert(ret);
        return ret;
    }

	FunctionDecl connect(typename const FunctionType& function)
	{
		FunctionDecl ret = std::make_shared<FunctionType>(function);
		m_Functions.insert(ret);
		return ret;
	}

	void disconnect(typename const FunctionDecl& function)
	{
		_ASSERT(function);

		FunctionsSet::const_iterator cit = m_Functions.find(function);
		if (cit != m_Functions.end())
		{
			m_Functions.erase(cit);
		}
	}

	bool operator()(typename ArgumentType& argument)
	{
		for (const auto& it : m_Functions)
		{
			(*it)(argument);
		}


		/*std::for_each(
			m_Functions.begin(),
			m_Functions.end(),
			[&argument](const FunctionDecl& _decl) 
			{ 
				(*_decl)(argument); 
			}
		);*/

		return !m_Functions.empty();
	}

private:
	mutable FunctionsSet m_Functions;
};


class ZN_API EventArgs
{
public:
	EventArgs(const Object* caller)
		: Caller(caller)
	{}

	// The object that invoked the event
	const Object* Caller;
};
typedef Delegate<EventArgs> Event;


class ZN_API WindowCloseEventArgs : public EventArgs
{
public:
	WindowCloseEventArgs(const Object* caller)
		: EventArgs(caller)
		, ConfirmClose(true)
	{}

	// The user can cancel a window closing operating by registering for the 
	// Window::Close event on the window and setting the ConfirmClose to false if
	// the window should be kept open (for example, there are unsaved changes 
	// made and closing the window would cause those changes to be lost).
	// By default, the window will be destoryed if the Window::Close even is not handled.
	bool ConfirmClose;
};
typedef Delegate<WindowCloseEventArgs> WindowCloseEvent;


class ZN_API KeyEventArgs : public EventArgs
{
public:
	enum KeyState
	{
		Released = 0,
		Pressed = 1
	};

	KeyEventArgs(const Object* caller, KeyCode key, uint32 c, KeyState state, bool control, bool shift, bool alt)
		: EventArgs(caller)
		, Key(key)
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
typedef Delegate<KeyEventArgs> KeyboardEvent;


class ZN_API MouseMotionEventArgs : public EventArgs
{
public:
	MouseMotionEventArgs(const Object* caller, bool leftButton, bool middleButton, bool rightButton, bool control, bool shift, int x, int y)
		: EventArgs(caller)
		, LeftButton(leftButton)
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
typedef Delegate<MouseMotionEventArgs> MouseMotionEvent;


class ZN_API MouseButtonEventArgs : public EventArgs
{
public:
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

	MouseButtonEventArgs(const Object* caller, MouseButton buttonID, ButtonState state, bool leftButton, bool middleButton, bool rightButton, bool control, bool shift, int x, int y)
		: EventArgs(caller)
		, Button(buttonID)
		, State(state)
		, LeftButton(leftButton)
		, MiddleButton(middleButton)
		, RightButton(rightButton)
		, Control(control)
		, Shift(shift)
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

	int X;              // The X-position of the cursor relative to the upper-left corner of the client area.
	int Y;              // The Y-position of the cursor relative to the upper-left corner of the client area.

	glm::vec2 GetPoint() const { return glm::vec2(X, Y); }
};
typedef Delegate<MouseButtonEventArgs> MouseButtonEvent;


class ZN_API MouseWheelEventArgs : public EventArgs
{
public:
	MouseWheelEventArgs(const Object* caller, float wheelDelta, bool leftButton, bool middleButton, bool rightButton, bool control, bool shift, int x, int y)
		: EventArgs(caller)
		, WheelDelta(wheelDelta)
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
typedef Delegate<MouseWheelEventArgs> MouseWheelEvent;


class ZN_API ResizeEventArgs : public EventArgs
{
public:
	ResizeEventArgs(const Object* caller, int width, int height)
		: EventArgs(caller)
		, Width(width)
		, Height(height)
	{}

	int Width;
	int Height;
};
typedef Delegate<ResizeEventArgs> ResizeEvent;


class ZN_API UpdateEventArgs : public EventArgs
{
public:
	UpdateEventArgs
	(
		const Object* caller,
		float DeltaTime,
		float TotalTime,
		uint64_t FrameCounter
	)
		: EventArgs(caller)
		, ElapsedTime(DeltaTime)
		, TotalTime(TotalTime)
		, FrameCounter(FrameCounter)
	{}

	float                                           ElapsedTime;
	float                                           TotalTime;
	int64_t                                         FrameCounter;
};
typedef Delegate<UpdateEventArgs> UpdateEvent;


class ZN_API RenderEventArgs : public EventArgs
{
public:
	RenderEventArgs
	(
		const Object* Caller,
		float DeltaTime,
		float TotalTime,
		uint64_t FrameCounter,
		const ICameraComponent3D* Camera,
		const IPipelineState* PipelineState,
		const ISceneNode* Node
	)
		: EventArgs(Caller)
		, ElapsedTime(DeltaTime)
		, TotalTime(TotalTime)
		, FrameCounter(FrameCounter)

		, Camera(Camera)
		, PipelineState(PipelineState)
		, Node(Node)
	{}

	float                                           ElapsedTime;
	float                                           TotalTime;
	int64_t                                         FrameCounter;

	const ICameraComponent3D*                       Camera;
	const IPipelineState*                           PipelineState;
	const ISceneNode*                               Node;
};
typedef Delegate<RenderEventArgs> RenderEvent;


class ZN_API UserEventArgs : public EventArgs
{
public:
	UserEventArgs(const Object* caller, int code, void* data1, void* data2)
		: EventArgs(caller)
		, Code(code)
		, Data1(data1)
		, Data2(data2)
	{}

	int     Code;
	void*   Data1;
	void*   Data2;
};
typedef Delegate<UserEventArgs> UserEvent;





class ZN_API SceneEventArgs : public EventArgs
{
public:
	SceneEventArgs(const Object* caller, const std::shared_ptr<IScene>& Scene)
		: EventArgs(caller)
		, Scene(Scene)
	{}

	const std::shared_ptr<IScene>& Scene;
};
typedef Delegate<SceneEventArgs> SceneEvent;



enum class ZN_API ESceneChangeType
{
	NodeAdded = 0,
	NodeRemoved
};

class ZN_API SceneChangeEventArgs : public SceneEventArgs
{
public:
	SceneChangeEventArgs(const Object* caller, const std::shared_ptr<IScene>& Scene, ESceneChangeType SceneChangeType, const std::shared_ptr<ISceneNode>& OwnerNode, const std::shared_ptr<ISceneNode>& ChildNode)
		: SceneEventArgs(caller, Scene)
		, SceneChangeType(SceneChangeType)
		, OwnerNode(OwnerNode)
		, ChildNode(ChildNode)
	{}

	const ESceneChangeType SceneChangeType;
	const std::shared_ptr<ISceneNode>& OwnerNode;
	const std::shared_ptr<ISceneNode>& ChildNode;
};
typedef Delegate<SceneChangeEventArgs> SceneChangeEvent;
