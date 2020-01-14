#include "stdafx.h"

// General
#include "ShaderParameterBase.h"

ShaderParameterBase::ShaderParameterBase()
	: m_Name("<emprty>")
	, m_uiSlotID(UINT_MAX)
	, m_Shader(nullptr)
	, m_ParameterType(Type::Invalid)

	, m_pConstantBuffer(nullptr)
	, m_pTexture(nullptr)
	, m_pSamplerState(nullptr)
	, m_pStructuredBuffer(nullptr)
{}

ShaderParameterBase::ShaderParameterBase(const std::string& name, UINT slotID, std::shared_ptr<IShader> shader, Type parameterType)
	: m_Name(name)
	, m_uiSlotID(slotID)
	, m_Shader(shader.get())
	, m_ParameterType(parameterType)

	, m_pConstantBuffer(nullptr)
	, m_pTexture(nullptr)
	, m_pSamplerState(nullptr)
	, m_pStructuredBuffer(nullptr)
{}

ShaderParameterBase::~ShaderParameterBase()
{}



//
// IShaderParameter
//
ShaderParameterBase::Type ShaderParameterBase::GetType() const
{
	return m_ParameterType;
}

bool ShaderParameterBase::IsValid() const
{
	return m_ParameterType != ShaderParameterBase::Type::Invalid;
}

void ShaderParameterBase::SetSource(const IShaderParameterSource * ShaderParameterSource)
{
	if (const IConstantBuffer* value = dynamic_cast<const IConstantBuffer*>(ShaderParameterSource))
	{
		m_pConstantBuffer = value;
		return;
	}

	if (const ITexture* value = dynamic_cast<const ITexture*>(ShaderParameterSource))
	{
		m_pTexture = value;
		return;
	}

	if (const ISamplerState* value = dynamic_cast<const ISamplerState*>(ShaderParameterSource))
	{
		m_pSamplerState = value;
		return;
	}

	if (const IStructuredBuffer* value = dynamic_cast<const IStructuredBuffer*>(ShaderParameterSource))
	{
		m_pStructuredBuffer = value;
		return;
	}
}

const IShaderParameterSource * ShaderParameterBase::GetSource()
{
	if (m_pConstantBuffer)
	{
		return m_pConstantBuffer;
	}
	if (m_pTexture)
	{
		return m_pTexture;
	}
	if (m_pSamplerState)
	{
		return m_pSamplerState;
	}
	if (m_pStructuredBuffer)
	{
		return m_pStructuredBuffer;
	}

	return nullptr;
}

void ShaderParameterBase::SetConstantBuffer(const IConstantBuffer* buffer)
{
	m_pConstantBuffer = buffer;
}

const IConstantBuffer * ShaderParameterBase::GetConstantBuffer() const
{
	return m_pConstantBuffer;
}

void ShaderParameterBase::SetTexture(const ITexture* texture)
{
	m_pTexture = texture;
}

const ITexture * ShaderParameterBase::GetTexture() const
{
	return m_pTexture;
}

void ShaderParameterBase::SetSampler(const ISamplerState* sampler)
{
	m_pSamplerState = sampler;
}

const ISamplerState * ShaderParameterBase::GetSampler() const
{
	return m_pSamplerState;
}

void ShaderParameterBase::SetStructuredBuffer(const IStructuredBuffer* rwBuffer)
{
	m_pStructuredBuffer = rwBuffer;
}

const IStructuredBuffer * ShaderParameterBase::GetStructuredBuffer() const
{
	return m_pStructuredBuffer;
}

void ShaderParameterBase::Bind()
{
	if (m_pConstantBuffer)
	{
		m_pConstantBuffer->Bind(m_uiSlotID, m_Shader, m_ParameterType);
	}
	if (m_pTexture)
	{
		m_pTexture->Bind(m_uiSlotID, m_Shader, m_ParameterType);
	}
	if (m_pSamplerState)
	{
		m_pSamplerState->Bind(m_uiSlotID, m_Shader, m_ParameterType);
	}
	if (m_pStructuredBuffer)
	{
		m_pStructuredBuffer->Bind(m_uiSlotID, m_Shader, m_ParameterType);
	}
}

void ShaderParameterBase::Unbind()
{
	if (m_pConstantBuffer)
	{
		m_pConstantBuffer->UnBind(m_uiSlotID, m_Shader, m_ParameterType);
		m_pConstantBuffer = nullptr;
	}
	if (m_pTexture)
	{
		m_pTexture->UnBind(m_uiSlotID, m_Shader, m_ParameterType);
	}
	if (m_pSamplerState)
	{
		m_pSamplerState->UnBind(m_uiSlotID, m_Shader, m_ParameterType);
	}
	if (m_pStructuredBuffer)
	{
		m_pStructuredBuffer->UnBind(m_uiSlotID, m_Shader, m_ParameterType);
		m_pStructuredBuffer = nullptr;
	}
}
