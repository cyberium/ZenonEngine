#include "stdafx.h"

// General
#include "MaterialWrapper.h"

MaterialWrapper::MaterialWrapper(std::shared_ptr<IMaterial> _materal)
	: m_Material(_materal)
{
}

MaterialWrapper::~MaterialWrapper()
{
}

void MaterialWrapper::SetShader(Shader::ShaderType type, std::shared_ptr<Shader> pShader)
{
	m_Material->SetShader(type, pShader);
}

std::shared_ptr<Shader> MaterialWrapper::GetShader(Shader::ShaderType type) const
{
	return m_Material->GetShader(type);
}

const ShaderMap & MaterialWrapper::GetShaders() const
{
	return m_Material->GetShaders();
}

std::shared_ptr<Texture> MaterialWrapper::GetTexture(uint8 ID) const
{
	return m_Material->GetTexture(ID);
}

void MaterialWrapper::SetTexture(uint8 type, std::shared_ptr<Texture> texture)
{
	m_Material->SetTexture(type, texture);
}

std::shared_ptr<SamplerState> MaterialWrapper::GetSampler(uint8 ID) const
{
    return m_Material->GetSampler(ID);
}

void MaterialWrapper::SetSampler(uint8 ID, std::shared_ptr<SamplerState> samplerState)
{
    m_Material->SetSampler(ID, samplerState);
}

void MaterialWrapper::Bind(const ShaderMap& shaders) const
{
	m_Material->Bind(shaders);
}

void MaterialWrapper::Unbind(const ShaderMap& shaders) const
{
	m_Material->Unbind(shaders);
}

void MaterialWrapper::SetWrapper(std::weak_ptr<IMaterial> _wrapper)
{
	m_Material->SetWrapper(weak_from_this());
}

void MaterialWrapper::UpdateConstantBuffer() const
{
	m_Material->UpdateConstantBuffer();
}

void MaterialWrapper::UpdateConstantBuffer(const void* _data, size_t size) const
{
	m_Material->UpdateConstantBuffer(_data, size);
}

void MaterialWrapper::MarkConstantBufferDirty()
{
	m_Material->MarkConstantBufferDirty();
}
