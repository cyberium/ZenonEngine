#pragma once

#include "common.h"

class Shader;
class ConstantBuffer;
class Texture;
class SamplerState;
class StructuredBuffer;

class OW_ENGINE_API ShaderParameter : public Object
{
    typedef Object base;
public:
	enum class Type
	{
		Invalid,    // Invalid parameter. Doesn't store a type.
		Texture,    // Texture.
		Sampler,    // Texture sampler.
		Buffer,     // Buffers, ConstantBuffers, StructuredBuffers.
		RWTexture,  // Texture that can be written to in a shader (using Store operations).
		RWBuffer,   // Read/write structured buffers.
	};

public:
	ShaderParameter();
	ShaderParameter(const std::string& name, UINT slotID, std::shared_ptr<Shader> shader, Type parameterType);
    virtual ~ShaderParameter();

	template <typename T>
	void Set(const T* value);

	template <typename T>
	const T* Get() const;

	// Get the type of the stored parameter.
	virtual Type GetType() const;

	// Bind the shader parameter to a specific slot for the given shader type.
	virtual void Bind();
	virtual void UnBind();

	// Test to see if this is a valid shader parameter.
	virtual bool IsValid() const;

protected:
	virtual void SetConstantBuffer(const ConstantBuffer* constantBuffer);
	virtual void SetTexture(const Texture* texture);
	virtual void SetSampler(const SamplerState* sampler);
	virtual void SetStructuredBuffer(const StructuredBuffer* rwBuffer);

private:
	std::string             m_Name;
	UINT                    m_uiSlotID;
	const Shader*           m_Shader;
	Type                    m_ParameterType;

	const ConstantBuffer*   m_pConstantBuffer;
	const Texture*          m_pTexture;
	const SamplerState*     m_pSamplerState;
	const StructuredBuffer* m_pStructuredBuffer;
};

// Template definitions
#include "ShaderParameter.inl"