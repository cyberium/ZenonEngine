#pragma once

#include <functional>

#include "Object.h"
#include "KeyCodes.h"

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

	void operator()(typename ArgumentType& argument)
	{
		std::for_each(
			m_Functions.begin(),
			m_Functions.end(),
			[&argument](const FunctionDecl& _decl) 
			{ 
				(*_decl)(argument); 
			}
		);
	}

private:
	mutable FunctionsSet m_Functions;
};



class EventArgs
{
public:
	EventArgs(const Object& caller)
		: Caller(caller)
	{}

	// The object that invoked the event
	const Object& Caller;
};
typedef Delegate<EventArgs> Event;
typedef Delegate<EventArgs>::FunctionDecl EventConnection;

class WindowCloseEventArgs : EventArgs
{
	typedef EventArgs base;
public:
	WindowCloseEventArgs(const Object& caller)
		: base(caller)
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
typedef Delegate<WindowCloseEventArgs>::FunctionDecl WindowCloseConnection;

class KeyEventArgs : public EventArgs
{
	typedef EventArgs base;
public:
	enum KeyState
	{
		Released = 0,
		Pressed = 1
	};

	KeyEventArgs(const Object& caller, KeyCode key, uint32 c, KeyState state, bool control, bool shift, bool alt)
		: base(caller)
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
typedef Delegate<KeyEventArgs>::FunctionDecl KeyboardConnection;


class MouseMotionEventArgs : public EventArgs
{
	typedef EventArgs base;
public:
	MouseMotionEventArgs(const Object& caller, bool leftButton, bool middleButton, bool rightButton, bool control, bool shift, int x, int y)
		: base(caller)
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
typedef Delegate<MouseMotionEventArgs>::FunctionDecl MouseMotionConnection;


class MouseButtonEventArgs : public EventArgs
{
public:
	enum MouseButton
	{
		None = 0,
		Left = 1,
		Right = 2,
		Middel = 3
	};
	enum ButtonState
	{
		Released = 0,
		Pressed = 1
	};

	typedef EventArgs base;
	MouseButtonEventArgs(const Object& caller, MouseButton buttonID, ButtonState state, bool leftButton, bool middleButton, bool rightButton, bool control, bool shift, int x, int y)
		: base(caller)
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
typedef Delegate<MouseButtonEventArgs>::FunctionDecl MouseButtonConnection;


class MouseWheelEventArgs : public EventArgs
{
public:
	typedef EventArgs base;
	MouseWheelEventArgs(const Object& caller, float wheelDelta, bool leftButton, bool middleButton, bool rightButton, bool control, bool shift, int x, int y)
		: base(caller)
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
typedef Delegate<MouseWheelEventArgs>::FunctionDecl MouseWheelConnection;


class ResizeEventArgs : public EventArgs
{
public:
	typedef EventArgs base;
	ResizeEventArgs(const Object& caller, int width, int height)
		: base(caller)
		, Width(width)
		, Height(height)
	{}

	int Width;
	int Height;
};
typedef Delegate<ResizeEventArgs> ResizeEvent;



class UserEventArgs : public EventArgs
{
	typedef EventArgs base;
public:
	UserEventArgs(const Object& caller, int code, void* data1, void* data2)
		: base(caller)
		, Code(code)
		, Data1(data1)
		, Data2(data2)
	{}

	int     Code;
	void*   Data1;
	void*   Data2;
};
typedef Delegate<UserEventArgs> UserEvent;
typedef Delegate<UserEventArgs>::FunctionDecl UserConnection;
