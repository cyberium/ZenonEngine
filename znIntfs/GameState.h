#pragma once

struct IGameState
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
};