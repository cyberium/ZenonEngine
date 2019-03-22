#pragma once

#include "Buffer.h"

class ConstantBuffer : public IBuffer
{
public:
	template <typename T>
	void Set(const T& data);

	// Always returns BufferType::ConstantBuffer
	virtual BufferType GetType() const;
	// Constant buffers only have 1 element.
	virtual uint32 GetElementCount() const;
	// Elements stride
	virtual uint32 GetElementStride() const;

	// Copy the contents of a buffer to this one. Buffers must be the same size.
	virtual void Copy(std::shared_ptr<ConstantBuffer> other) = 0;

	virtual void Set(const void* data, size_t size) = 0;
};



//
// Template specizalizations
//
template<typename T>
void ConstantBuffer::Set(const T& data)
{
	Set(&data, sizeof(T));
}