#include "stdafx.h"

// General
#include "MaterialWrapper.h"

MaterialWrapper::MaterialWrapper(std::shared_ptr<Material> _materal)
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

const Material::ShaderMap & MaterialWrapper::GetShaders() const
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

void MaterialWrapper::Bind() const
{
	m_Material->Bind();
}

void MaterialWrapper::Unbind() const
{
	m_Material->Unbind();
}

void MaterialWrapper::SetWrapper(std::weak_ptr<Material> _wrapper)
{
	m_Material->SetWrapper(weak_from_this());
}

void MaterialWrapper::CreateConstantBuffer(const void* data, size_t size)
{
	m_Material->CreateConstantBuffer(data, size);
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
