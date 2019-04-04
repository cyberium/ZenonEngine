#pragma once

// FORWARD BEGIN
struct TcpSocket;
// FORWARD END

struct ISendable
{
	virtual ~ISendable() = 0 {};

	virtual void Send(TcpSocket* _socket) = 0;
};
