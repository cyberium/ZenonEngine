#pragma once

#include "Shader.h"
#include "ShaderParameter.h"

class IBuffer : public Object
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

	virtual bool Bind(uint32 id, std::weak_ptr<Shader> shader, ShaderParameter::Type parameterType) = 0;
	virtual void UnBind(uint32 id, std::weak_ptr<Shader> shader, ShaderParameter::Type parameterType) = 0;

	virtual void Copy(std::shared_ptr<IBuffer> other) = 0;

	virtual BufferType GetType() const = 0;
	virtual uint32 GetElementCount() const = 0;
	virtual uint32 GetElementStride() const = 0;
};
