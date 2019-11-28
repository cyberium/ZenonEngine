#pragma once

// FORWARD BEGIN
struct TcpSocket;
// FORWARD END

struct OW_ENGINE_API ISendable
{
	virtual ~ISendable() = 0 {};

	virtual void Send(TcpSocket* _socket) = 0;
};
