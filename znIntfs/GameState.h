#pragma once

// FORWARD BEGIN
class Scene;
// FORWARD END

struct OW_ENGINE_API IGameState
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

	virtual std::shared_ptr<Scene> GetScene() const = 0;
};