#include "stdafx.h"

// General
#include "ShaderParameterBase.h"

ShaderParameterBase::ShaderParameterBase()
	: m_Name("<emprty>")
	, m_uiSlotID(UINT_MAX)
	, m_Shader(nullptr)
	, m_ParameterType(EType::Invalid)

	, m_pConstantBuffer(nullptr)
	, m_pTexture(nullptr)
	, m_pSamplerState(nullptr)
	, m_pStructuredBuffer(nullptr)
{}

ShaderParameterBase::ShaderParameterBase(const std::string& name, UINT slotID, IShader* shader, EType parameterType)
	: m_Name(name)
	, m_uiSlotID(slotID)
	, m_Shader(shader)
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
ShaderParameterBase::EType ShaderParameterBase::GetType() const
{
	return m_ParameterType;
}

void ShaderParameterBase::SetSource(std::shared_ptr<IShaderParameterSource> ShaderParameterSource)
{
	if (std::shared_ptr<IConstantBuffer> value = std::dynamic_pointer_cast<IConstantBuffer>(ShaderParameterSource))
	{
		m_pConstantBuffer = value;
		return;
	}

	if (std::shared_ptr<ITexture> value = std::dynamic_pointer_cast<ITexture>(ShaderParameterSource))
	{
		m_pTexture = value;
		return;
	}

	if (std::shared_ptr<ISamplerState> value = std::dynamic_pointer_cast<ISamplerState>(ShaderParameterSource))
	{
		m_pSamplerState = value;
		return;
	}

	if (std::shared_ptr<IStructuredBuffer> value = std::dynamic_pointer_cast<IStructuredBuffer>(ShaderParameterSource))
	{
		m_pStructuredBuffer = value;
		return;
	}
}

std::shared_ptr<IShaderParameterSource> ShaderParameterBase::GetSource() const
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

	throw CException("This shader parameter doesn't contains any assigments.");
}

void ShaderParameterBase::SetConstantBuffer(std::shared_ptr<IConstantBuffer> buffer)
{
	m_pConstantBuffer = buffer;
}

const std::shared_ptr<IConstantBuffer>& ShaderParameterBase::GetConstantBuffer() const
{
	return m_pConstantBuffer;
}

void ShaderParameterBase::SetTexture(std::shared_ptr<ITexture> texture)
{
	m_pTexture = texture;
}

const std::shared_ptr<ITexture>& ShaderParameterBase::GetTexture() const
{
	return m_pTexture;
}

void ShaderParameterBase::SetSampler(std::shared_ptr<ISamplerState> sampler)
{
	m_pSamplerState = sampler;
}

const std::shared_ptr<ISamplerState>& ShaderParameterBase::GetSampler() const
{
	return m_pSamplerState;
}

void ShaderParameterBase::SetStructuredBuffer(std::shared_ptr<IStructuredBuffer> rwBuffer)
{
	m_pStructuredBuffer = rwBuffer;
}

const std::shared_ptr<IStructuredBuffer>& ShaderParameterBase::GetStructuredBuffer() const
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
