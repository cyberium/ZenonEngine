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



//
// IMaterial
//
void MaterialProxie::SetName(const std::string & Name)
{
	m_Material->SetName(Name);
}

std::string MaterialProxie::GetName() const
{
	return m_Material->GetName();
}

void MaterialProxie::SetShader(EShaderType type, const std::shared_ptr<IShader> pShader)
{
	m_Material->SetShader(type, pShader);
}

const IShader& MaterialProxie::GetShader(EShaderType type) const
{
	return m_Material->GetShader(type);
}

const ShaderMap & MaterialProxie::GetShaders() const
{
	return m_Material->GetShaders();
}

void MaterialProxie::SetTexture(uint8 type, const std::shared_ptr<ITexture> texture)
{
	m_Material->SetTexture(type, texture);
}

const ITexture& MaterialProxie::GetTexture(uint8 ID) const
{
	return m_Material->GetTexture(ID);
}

void MaterialProxie::SetSampler(uint8 ID, const std::shared_ptr<ISamplerState> samplerState)
{
	m_Material->SetSampler(ID, samplerState);
}

const ISamplerState& MaterialProxie::GetSampler(uint8 ID) const
{
    return m_Material->GetSampler(ID);
}

void MaterialProxie::Bind(const ShaderMap& shaders) const
{
	m_Material->Bind(shaders);
}

void MaterialProxie::Unbind(const ShaderMap& shaders) const
{
	m_Material->Unbind(shaders);
}

void MaterialProxie::SetWrapper(IMaterial* /*_wrapper*/)
{
	m_Material->SetWrapper(this);
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
