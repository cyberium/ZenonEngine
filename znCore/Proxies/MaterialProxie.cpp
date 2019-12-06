#include "stdafx.h"

// General
#include "MaterialProxie.h"

MaterialProxie::MaterialProxie(std::shared_ptr<IMaterial> _materal)
	: m_Material(_materal)
{
	_ASSERT(m_Material);
}

MaterialProxie::~MaterialProxie()
{}

void MaterialProxie::SetShader(IShader::ShaderType type, std::shared_ptr<IShader> pShader)
{
	m_Material->SetShader(type, pShader);
}

std::shared_ptr<IShader> MaterialProxie::GetShader(IShader::ShaderType type) const
{
	return m_Material->GetShader(type);
}

const ShaderMap & MaterialProxie::GetShaders() const
{
	return m_Material->GetShaders();
}

std::shared_ptr<ITexture> MaterialProxie::GetTexture(uint8 ID) const
{
	return m_Material->GetTexture(ID);
}

void MaterialProxie::SetTexture(uint8 type, std::shared_ptr<ITexture> texture)
{
	m_Material->SetTexture(type, texture);
}

std::shared_ptr<ISamplerState> MaterialProxie::GetSampler(uint8 ID) const
{
    return m_Material->GetSampler(ID);
}

void MaterialProxie::SetSampler(uint8 ID, std::shared_ptr<ISamplerState> samplerState)
{
    m_Material->SetSampler(ID, samplerState);
}

void MaterialProxie::Bind(const ShaderMap& shaders) const
{
	m_Material->Bind(shaders);
}

void MaterialProxie::Unbind(const ShaderMap& shaders) const
{
	m_Material->Unbind(shaders);
}

void MaterialProxie::SetWrapper(std::weak_ptr<IMaterial> _wrapper)
{
	m_Material->SetWrapper(weak_from_this());
}

void MaterialProxie::UpdateConstantBuffer() const
{
	m_Material->UpdateConstantBuffer();
}

void MaterialProxie::UpdateConstantBuffer(const void* _data, size_t size) const
{
	m_Material->UpdateConstantBuffer(_data, size);
}

void MaterialProxie::MarkConstantBufferDirty()
{
	m_Material->MarkConstantBufferDirty();
}
