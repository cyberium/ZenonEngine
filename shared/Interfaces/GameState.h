#pragma once

// FORWARD BEGIN
ZN_INTERFACE IScene;
ZN_INTERFACE IManager;
ZN_INTERFACE IRenderWindow;
ZN_INTERFACE IWindowEvents;
// FORWARD END


ZN_INTERFACE ZN_API IGameState
{
	virtual ~IGameState() = 0 {};

	virtual bool Init() = 0;
	virtual void Destroy() = 0;
	virtual bool Set() = 0;
	virtual void Unset() = 0;

	virtual void SetInited(bool _value) = 0;
	virtual bool IsInited() const = 0;
	virtual void SetCurrent(bool _value) = 0;
	virtual bool IsCurrent() const = 0;

	// Keyboard events
	virtual void OnKeyPressed(KeyEventArgs& e) = 0;
	virtual void OnKeyReleased(KeyEventArgs& e) = 0;
	virtual void OnKeyboardFocus(EventArgs& e) = 0;
	virtual void OnKeyboardBlur(EventArgs& e) = 0;

	// Mouse events
	virtual void OnMouseMoved(MouseMotionEventArgs& e) = 0;
	virtual void OnMouseButtonPressed(MouseButtonEventArgs& e) = 0;
	virtual void OnMouseButtonReleased(MouseButtonEventArgs& e) = 0;
	virtual void OnMouseWheel(MouseWheelEventArgs& e) = 0;
	virtual void OnMouseLeave(EventArgs& e) = 0;
	virtual void OnMouseFocus(EventArgs& e) = 0;
	virtual void OnMouseBlur(EventArgs& e) = 0;

	virtual std::shared_ptr<IScene> GetScene() const = 0;
};

typedef uint32 ZN_API GameStatePriority;

struct ZN_API SGameStateDef
{
	std::string       Name;
	GameStatePriority Priority;
};



//
// For plugins
//
ZN_INTERFACE ZN_API IGameStateCreator
{
	virtual ~IGameStateCreator() {}

	virtual size_t GetGameStatesCount() const = 0;
	virtual std::string GetGameStateName(size_t Index) const = 0;
	virtual GameStatePriority GetGameStatePriority(size_t Index) const = 0;
	virtual std::shared_ptr<IGameState> CreateGameState(size_t Index, IRenderWindow* RenderWindow, IWindowEvents* WindowEvents) const = 0;
};



ZN_INTERFACE ZN_API
__declspec(uuid("BF3B1BDC-9019-4082-9042-BE3B033CC199"))
IGameStatesFactory
	: public IManager
{
	virtual ~IGameStatesFactory() {}

	virtual void AddGameStateCreator(std::shared_ptr<IGameStateCreator> Creator) = 0;
	virtual void RemoveGameStateCreator(std::shared_ptr<IGameStateCreator> Creator) = 0;

	virtual std::shared_ptr<IGameState> CreateGameStateWithHighestPriority(IRenderWindow* RenderWindow, IWindowEvents* WindowEvents) const = 0;
	virtual std::shared_ptr<IGameState> CreateGameState(std::string GameStateName, IRenderWindow* RenderWindow, IWindowEvents* WindowEvents) const = 0;
};