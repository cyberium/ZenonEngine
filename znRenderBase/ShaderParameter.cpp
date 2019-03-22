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
	: m_uiSlotID(UINT_MAX)
	, m_ParameterType(Type::Invalid)
{}

ShaderParameter::ShaderParameter(cstring name, UINT slotID, std::shared_ptr<Shader> shader, Type parameterType)
	: m_Name(name)
	, m_uiSlotID(slotID)
	, m_Shader(shader)
	, m_ParameterType(parameterType)
{}

//--

template<> void ShaderParameter::Set<ConstantBuffer>(std::shared_ptr<ConstantBuffer> value)
{
	SetConstantBuffer(value);
}
template<> void ShaderParameter::Set<Texture>(std::shared_ptr<Texture> value)
{
	SetTexture(value);
}
template<> void ShaderParameter::Set<SamplerState>(std::shared_ptr<SamplerState> value)
{
	SetSampler(value);
}
template<> void ShaderParameter::Set<StructuredBuffer>(std::shared_ptr<StructuredBuffer> value)
{
	SetStructuredBuffer(value);
}

//--

template<> std::shared_ptr<ConstantBuffer> ShaderParameter::Get<ConstantBuffer>() const
{
	return m_pConstantBuffer.lock();
}
template<> std::shared_ptr<Texture> ShaderParameter::Get<Texture>() const
{
	return m_pTexture.lock();
}
template<> std::shared_ptr<SamplerState> ShaderParameter::Get<SamplerState>() const
{
	return m_pSamplerState.lock();
}
template<> std::shared_ptr <StructuredBuffer> ShaderParameter::Get<StructuredBuffer>() const
{
	return m_pStructuredBuffer.lock();
}

//--

void ShaderParameter::SetConstantBuffer(std::shared_ptr<ConstantBuffer> buffer)
{
	m_pConstantBuffer = buffer;
}

void ShaderParameter::SetTexture(std::shared_ptr<Texture> texture)
{
	m_pTexture = texture;
}

void ShaderParameter::SetSampler(std::shared_ptr<SamplerState> sampler)
{
	m_pSamplerState = sampler;
}

void ShaderParameter::SetStructuredBuffer(std::shared_ptr<StructuredBuffer> rwBuffer)
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
	if (std::shared_ptr<ConstantBuffer> constantBuffer = m_pConstantBuffer.lock())
	{
		constantBuffer->Bind(m_uiSlotID, m_Shader, m_ParameterType);
	}
	if (std::shared_ptr<Texture> texture = m_pTexture.lock())
	{
		texture->Bind(m_uiSlotID, m_Shader, m_ParameterType);
	}
	if (std::shared_ptr<SamplerState> samplerState = m_pSamplerState.lock())
	{
		samplerState->Bind(m_uiSlotID, m_Shader, m_ParameterType);
	}
	if (std::shared_ptr<StructuredBuffer> buffer = m_pStructuredBuffer.lock())
	{
		buffer->Bind(m_uiSlotID, m_Shader, m_ParameterType);
	}
}

void ShaderParameter::UnBind()
{
	if (std::shared_ptr<ConstantBuffer> constantBuffer = m_pConstantBuffer.lock())
	{
		constantBuffer->UnBind(m_uiSlotID, m_Shader, m_ParameterType);
	}
	if (std::shared_ptr<Texture> texture = m_pTexture.lock())
	{
		texture->UnBind(m_uiSlotID, m_Shader, m_ParameterType);
	}
	if (std::shared_ptr<SamplerState> samplerState = m_pSamplerState.lock())
	{
		samplerState->UnBind(m_uiSlotID, m_Shader, m_ParameterType);
	}
	if (std::shared_ptr<StructuredBuffer> buffer = m_pStructuredBuffer.lock())
	{
		buffer->UnBind(m_uiSlotID, m_Shader, m_ParameterType);
	}
}

bool ShaderParameter::IsValid() const
{
	return m_ParameterType != ShaderParameter::Type::Invalid;
}