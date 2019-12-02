#pragma once 

ZN_INTERFACE OW_ENGINE_API IBuffer
{
	enum class OW_ENGINE_API BufferType
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

	virtual void Copy(std::shared_ptr<IBuffer> other) = 0;

	virtual BufferType GetType() const = 0;
	virtual uint32 GetElementCount() const = 0;
	virtual uint32 GetElementStride() const = 0;
	virtual uint32 GetElementOffset() const = 0;
};



ZN_INTERFACE OW_ENGINE_API IConstantBuffer : public IBuffer
{
	virtual ~IConstantBuffer() {}

	virtual void Copy(std::shared_ptr<IConstantBuffer> other) = 0;
	virtual void Set(const void* data, size_t size) = 0;

	// Templates

	template <typename T>
	void Set(const T& data);
};



//
// Template specizalizations
//
template<typename T>
void IConstantBuffer::Set(const T& data)
{
	Set(&data, sizeof(T));
}



ZN_INTERFACE OW_ENGINE_API IStructuredBuffer : public IBuffer
{
	virtual ~IStructuredBuffer() {}

	virtual void Copy(std::shared_ptr<IStructuredBuffer> other) = 0;
	virtual void SetData(void* data, size_t elementSize, size_t offset, size_t numElements) = 0;
	virtual void Clear() = 0;

	// Templates

	template<typename T>
	void Set(const std::vector<T>& value);
};



//
// Template specizalizations
//
template<typename T>
void IStructuredBuffer::Set(const std::vector<T>& values)
{
	SetData((void*)values.data(), sizeof(T), 0, values.size());
}

typedef std::vector<std::shared_ptr<IBuffer>> BufferList;
