#pragma once

// FORWARD BEGIN
ZN_INTERFACE IConstantBuffer;
ZN_INTERFACE ITexture;
ZN_INTERFACE ISamplerState;
ZN_INTERFACE IStructuredBuffer;
// FORWARD END

ZN_INTERFACE ZN_API __declspec(novtable) IShaderParameterSource
{
	virtual ~IShaderParameterSource() {}
};

ZN_INTERFACE ZN_API __declspec(novtable) IShaderParameter
{
	enum class ZN_API Type
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

	virtual void SetSource(std::shared_ptr<IShaderParameterSource> ShaderParameterSource) = 0;
	virtual std::shared_ptr<IShaderParameterSource> GetSource() const = 0;

	virtual void SetConstantBuffer(std::shared_ptr<IConstantBuffer> constantBuffer) = 0;
	virtual const std::shared_ptr<IConstantBuffer>& GetConstantBuffer() const = 0;

	virtual void SetTexture(std::shared_ptr<ITexture> texture) = 0;
	virtual const std::shared_ptr<ITexture>& GetTexture() const = 0;

	virtual void SetSampler(std::shared_ptr<ISamplerState> sampler) = 0;
	virtual const std::shared_ptr<ISamplerState>& GetSampler() const = 0;

	virtual void SetStructuredBuffer(std::shared_ptr<IStructuredBuffer> rwBuffer) = 0;
	virtual const std::shared_ptr<IStructuredBuffer>& GetStructuredBuffer() const = 0;

	// Bind the shader parameter to a specific slot for the given shader type
	virtual void Bind() = 0;
	virtual void Unbind() = 0;

	// Templates

	template <typename T>
	inline void Set(std::shared_ptr<T> value)
	{
		_ASSERT(false);
	}

	template <typename T>
	inline const std::shared_ptr<T>& Get() const
	{
		_ASSERT(false);
	}
};



//
// Template specizalizations
//
template<> 
inline void IShaderParameter::Set<IConstantBuffer>(std::shared_ptr<IConstantBuffer> value)
{
	SetConstantBuffer(value);
}
template<> 
inline void IShaderParameter::Set<ITexture>(std::shared_ptr<ITexture> value)
{
	SetTexture(value);
}
template<> 
inline void IShaderParameter::Set<ISamplerState>(std::shared_ptr<ISamplerState> value)
{
	SetSampler(value);
}
template<> 
inline void IShaderParameter::Set<IStructuredBuffer>(std::shared_ptr<IStructuredBuffer> value)
{
	SetStructuredBuffer(value);
}

template<>
inline const std::shared_ptr<IConstantBuffer>& IShaderParameter::Get<IConstantBuffer>() const
{
	return GetConstantBuffer();
}
template<>
inline const std::shared_ptr<ITexture>& IShaderParameter::Get<ITexture>() const
{
	return GetTexture();
}
template<>
inline const std::shared_ptr<ISamplerState>& IShaderParameter::Get<ISamplerState>() const
{
	return GetSampler();
}
template<>
inline const std::shared_ptr<IStructuredBuffer>& IShaderParameter::Get<IStructuredBuffer>() const
{
	return GetStructuredBuffer();
}
