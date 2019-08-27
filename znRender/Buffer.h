#pragma once

#include "common.h"

#include "Shader.h"
#include "ShaderParameter.h"

class OW_ENGINE_API IBuffer : public Object
{
public:
	enum BufferType
	{
		Unknown = 0,
		VertexBuffer,
		IndexBuffer,
		StructuredBuffer,
		ConstantBuffer
	};

	virtual bool Bind(uint32 id, const Shader* shader, ShaderParameter::Type parameterType) const = 0;
	virtual void UnBind(uint32 id, const Shader* shader, ShaderParameter::Type parameterType) const = 0;

	virtual void Copy(std::shared_ptr<IBuffer> other) = 0;

	virtual BufferType GetType() const = 0;
	virtual uint32 GetElementCount() const = 0;
	virtual uint32 GetElementStride() const = 0;
    virtual uint32 GetElementOffset() const = 0;
};
