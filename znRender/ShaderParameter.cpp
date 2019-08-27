#include "stdafx.h"

// Include
#include "Shader.h"
#include "ConstantBuffer.h"
#include "Texture.h"
#include "SamplerState.h"
#include "StructuredBuffer.h"

// General
#include "ShaderParameter.h"

ShaderParameter::ShaderParameter()
	: m_Name("<emprty>")
	, m_uiSlotID(UINT_MAX)
	, m_Shader(nullptr)
	, m_ParameterType(Type::Invalid)

	, m_pConstantBuffer(nullptr)
	, m_pTexture(nullptr)
	, m_pSamplerState(nullptr)
	, m_pStructuredBuffer(nullptr)
{}

ShaderParameter::ShaderParameter(const std::string& name, UINT slotID, std::shared_ptr<Shader> shader, Type parameterType)
	: m_Name(name)
	, m_uiSlotID(slotID)
	, m_Shader(shader.get())
	, m_ParameterType(parameterType)

	, m_pConstantBuffer(nullptr)
	, m_pTexture(nullptr)
	, m_pSamplerState(nullptr)
	, m_pStructuredBuffer(nullptr)
{}

ShaderParameter::~ShaderParameter()
{
}

//--

template<> void ShaderParameter::Set<ConstantBuffer>(const ConstantBuffer* value)
{
	SetConstantBuffer(value);
}
template<> void ShaderParameter::Set<Texture>(const Texture* value)
{
	SetTexture(value);
}
template<> void ShaderParameter::Set<SamplerState>(const SamplerState* value)
{
	SetSampler(value);
}
template<> void ShaderParameter::Set<StructuredBuffer>(const StructuredBuffer* value)
{
	SetStructuredBuffer(value);
}

//--

template<> const ConstantBuffer* ShaderParameter::Get<ConstantBuffer>() const
{
	return m_pConstantBuffer;
}
template<> const Texture* ShaderParameter::Get<Texture>() const
{
	return m_pTexture;
}
template<> const SamplerState* ShaderParameter::Get<SamplerState>() const
{
	return m_pSamplerState;
}
template<> const StructuredBuffer* ShaderParameter::Get<StructuredBuffer>() const
{
	return m_pStructuredBuffer;
}

//--

void ShaderParameter::SetConstantBuffer(const ConstantBuffer* buffer)
{
	m_pConstantBuffer = buffer;
}

void ShaderParameter::SetTexture(const Texture* texture)
{
	m_pTexture = texture;
}

void ShaderParameter::SetSampler(const SamplerState* sampler)
{
	m_pSamplerState = sampler;
}

void ShaderParameter::SetStructuredBuffer(const StructuredBuffer* rwBuffer)
{
	m_pStructuredBuffer = rwBuffer;
}

//--

ShaderParameter::Type ShaderParameter::GetType() const
{
	return m_ParameterType;
}

void ShaderParameter::Bind()
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

void ShaderParameter::UnBind()
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

bool ShaderParameter::IsValid() const
{
	return m_ParameterType != ShaderParameter::Type::Invalid;
}