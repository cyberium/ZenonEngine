#pragma once

// FORWARD BEGIN
struct ISocket;
// FORWARD END

struct ISendable
{
	virtual ~ISendable() = 0 {};

	virtual void Send(ISocket* _socket) = 0;
};

struct IAcceptable
{
	virtual ~IAcceptable() = 0 {};

};

struct ISocket
{
	virtual ~ISocket() = 0 {};

	virtual void SendData(const IByteBuffer& _bb) = 0;
	virtual void SendData(const uint8* _data, uint32 _count) = 0;
};

