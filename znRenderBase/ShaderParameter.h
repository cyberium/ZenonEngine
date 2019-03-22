#pragma once

class Shader;
class ConstantBuffer;
class Texture;
class SamplerState;
class StructuredBuffer;

class ShaderParameter : public Object
{
public:
	typedef Object base;

	enum class Type
	{
		Invalid,    // Invalid parameter. Doesn't store a type.
		Texture,    // Texture.
		Sampler,    // Texture sampler.
		Buffer,     // Buffers, ConstantBuffers, StructuredBuffers.
		RWTexture,  // Texture that can be written to in a shader (using Store operations).
		RWBuffer,   // Read/write structured buffers.
	};

	ShaderParameter();
	ShaderParameter(cstring name, UINT slotID, std::shared_ptr<Shader> shader, Type parameterType);

	template <typename T>
	void Set(std::shared_ptr<T> value);

	template <typename T>
	std::shared_ptr<T> Get() const;

	// Get the type of the stored parameter.
	virtual Type GetType() const;

	// Bind the shader parameter to a specific slot for the given shader type.
	virtual void Bind();
	virtual void UnBind();

	// Test to see if this is a valid shader parameter.
	virtual bool IsValid() const;

protected:
	virtual void SetConstantBuffer(std::shared_ptr<ConstantBuffer> constantBuffer);
	virtual void SetTexture(std::shared_ptr<Texture> texture);
	virtual void SetSampler(std::shared_ptr<SamplerState> sampler);
	virtual void SetStructuredBuffer(std::shared_ptr<StructuredBuffer> rwBuffer);

private:
	std::string                     m_Name;
	UINT                            m_uiSlotID;
	std::weak_ptr<Shader>           m_Shader;
	Type                            m_ParameterType;

	std::weak_ptr<ConstantBuffer>   m_pConstantBuffer;
	std::weak_ptr<Texture>          m_pTexture;
	std::weak_ptr<SamplerState>     m_pSamplerState;
	std::weak_ptr<StructuredBuffer> m_pStructuredBuffer;
};

// Template definitions
#include "ShaderParameter.inl"