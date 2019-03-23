#include "stdafx.h"

// General
#include "ConstantBuffer.h"

IBuffer::BufferType ConstantBuffer::GetType() const
{
	return IBuffer::ConstantBuffer;
}

uint32 ConstantBuffer::GetElementCount() const
{
	return 1;
}

uint32 ConstantBuffer::GetElementStride() const
{
	return 0;
}