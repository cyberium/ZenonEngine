#pragma once

#include "Buffer.h"

class StructuredBuffer : public IBuffer
{
public:
	virtual bool Bind(uint32 id, std::weak_ptr<Shader> shader, ShaderParameter::Type parameterType) = 0;
	virtual void UnBind(uint32 id, std::weak_ptr<Shader> shader, ShaderParameter::Type parameterType) = 0;

	// Copy the contents of another buffer to this one.
	// Buffers must be the same size.
	virtual void Copy(std::shared_ptr<StructuredBuffer> other) = 0;

	// Set the buffer data.
	template<typename T>
	void Set(const std::vector<T>& value);

	virtual void Clear() = 0;

protected:
	virtual void SetData(void* data, size_t elementSize, size_t offset, size_t numElements) = 0;

};



//
// Template specizalizations
//
template<typename T>
void StructuredBuffer::Set(const std::vector<T>& values)
{
	SetData((void*)values.data(), sizeof(T), 0, values.size());
}
