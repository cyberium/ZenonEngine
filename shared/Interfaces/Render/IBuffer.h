#pragma once 

// FORWARD BEGIN
ZN_INTERFACE IShaderParameterSource;
// FORWARD END

ZN_INTERFACE ZN_API IBuffer
	: public Object
{
	enum class ZN_API BufferType : uint32
	{
		Unknown = 0,
		VertexBuffer,
		IndexBuffer,
		StructuredBuffer,
		ConstantBuffer
	};

	virtual ~IBuffer() {}

	virtual bool Bind(uint32 id, const IShader* shader, IShaderParameter::Type parameterType) const = 0;
	virtual void UnBind(uint32 id, const IShader* shader, IShaderParameter::Type parameterType) const = 0;

	virtual void Copy(const IBuffer* Other) const = 0;

	virtual BufferType GetBufferType() const = 0;
	virtual uint32 GetElementCount() const = 0;
	virtual uint32 GetElementStride() const = 0;
	virtual uint32 GetElementOffset() const = 0;
};



ZN_INTERFACE ZN_API IConstantBuffer 
	: public IBuffer
	, public IShaderParameterSource
{
	virtual ~IConstantBuffer() {}

	virtual void Copy(const IConstantBuffer* Other) const = 0;
	virtual void Set(const void* data, size_t size) = 0;

	// Templates

	template <typename T>
	inline void Set(const T& data)
	{
		Set(&data, sizeof(T));
	}
};



ZN_INTERFACE ZN_API IStructuredBuffer 
	: public IBuffer
	, public IShaderParameterSource
{
	virtual ~IStructuredBuffer() {}

	virtual void Copy(const IStructuredBuffer* other) = 0;
	virtual void Set(void* data, size_t elementSize, size_t numElements) = 0;
	virtual void Clear() = 0;

	// Templates

	template<typename T>
	inline void Set(const std::vector<T>& values)
	{
		Set((void*)values.data(), sizeof(T), values.size());
	}
};


typedef std::vector<IBuffer*> BufferList;
