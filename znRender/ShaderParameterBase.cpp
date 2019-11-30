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

// IShaderParameter
ShaderParameterBase::Type ShaderParameterBase::GetType() const
{
	return m_ParameterType;
}

bool ShaderParameterBase::IsValid() const
{
	return m_ParameterType != ShaderParameterBase::Type::Invalid;
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
	}
}
