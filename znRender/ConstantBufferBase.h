#pragma once

class OW_ENGINE_API ConstantBufferBase : public IConstantBuffer
{
public:
	virtual BufferType GetType() const
	{
		return IBuffer::BufferType::ConstantBuffer;
	}
	virtual uint32 GetElementCount() const
	{
		return 1;
	}
	virtual uint32 GetElementStride() const
	{
		return 0;
	}
    virtual uint32 GetElementOffset() const
	{
		return 0;
	}
};
