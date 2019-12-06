#pragma once

// FORWARD BEGIN
ZN_INTERFACE IConstantBuffer;
ZN_INTERFACE ITexture;
ZN_INTERFACE ISamplerState;
ZN_INTERFACE IStructuredBuffer;
// FORWARD END

ZN_INTERFACE IShaderParameter
{
	enum class OW_ENGINE_API Type
	{
		Invalid,    // Invalid parameter. Doesn't store a type.
		Texture,    // Texture.
		Sampler,    // Texture sampler.
		Buffer,     // Buffers, ConstantBuffers, StructuredBuffers.
		RWTexture,  // Texture that can be written to in a shader (using Store operations).
		RWBuffer,   // Read/write structured buffers.
	};

	virtual ~IShaderParameter() {}

	// Get the type of the stored parameter.
	virtual Type GetType() const = 0;

	// Test to see if this is a valid shader parameter.
	virtual bool IsValid() const = 0;

	virtual void SetConstantBuffer(const IConstantBuffer* constantBuffer) = 0;
	virtual const IConstantBuffer* GetConstantBuffer() const = 0;
	virtual void SetTexture(const ITexture* texture) = 0;
	virtual const ITexture* GetTexture() const = 0;
	virtual void SetSampler(const ISamplerState* sampler) = 0;
	virtual const ISamplerState* GetSampler() const = 0;
	virtual void SetStructuredBuffer(const IStructuredBuffer* rwBuffer) = 0;
	virtual const IStructuredBuffer* GetStructuredBuffer() const = 0;

	// Bind the shader parameter to a specific slot for the given shader type
	virtual void Bind() = 0;
	virtual void Unbind() = 0;

	// Templates

	template <typename T>
	void Set(const T* value);

	template <typename T>
	const T* Get() const;
};



//
// Template specizalizations
//
template<> 
inline void IShaderParameter::Set<IConstantBuffer>(const IConstantBuffer* value)
{
	SetConstantBuffer(value);
}
template<> 
inline void IShaderParameter::Set<ITexture>(const ITexture* value)
{
	SetTexture(value);
}
template<> 
inline void IShaderParameter::Set<ISamplerState>(const ISamplerState* value)
{
	SetSampler(value);
}
template<> 
inline void IShaderParameter::Set<IStructuredBuffer>(const IStructuredBuffer* value)
{
	SetStructuredBuffer(value);
}

template<>
inline const IConstantBuffer* IShaderParameter::Get<IConstantBuffer>() const
{
	return GetConstantBuffer();
}
template<>
inline const ITexture* IShaderParameter::Get<ITexture>() const
{
	return GetTexture();
}
template<>
inline const ISamplerState* IShaderParameter::Get<ISamplerState>() const
{
	return GetSampler();
}
template<>
inline const IStructuredBuffer* IShaderParameter::Get<IStructuredBuffer>() const
{
	return GetStructuredBuffer();
}
