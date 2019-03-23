#include "stdafx.h"

// Include
#include "ConstantBuffer.h"
#include "RenderDevice.h"

// General
#include "MaterialImpl.h"

MaterialImpl::MaterialImpl(IRenderDevice* renderDevice)
	: m_RenderDevice(renderDevice)
	, m_Dirty(false)
{}

MaterialImpl::~MaterialImpl()
{
	if (m_pConstantBuffer)
	{
		m_RenderDevice->DestroyConstantBuffer(m_pConstantBuffer);
		m_pConstantBuffer.reset();
	}
}


void MaterialImpl::SetShader(Shader::ShaderType type, std::shared_ptr<Shader> pShader)
{
	m_Shaders[type] = pShader;
}

std::shared_ptr<Shader> MaterialImpl::GetShader(Shader::ShaderType type) const
{
	ShaderMap::const_iterator iter = m_Shaders.find(type);
	if (iter != m_Shaders.end())
	{
		return iter->second;
	}

	return nullptr;
}

const MaterialImpl::ShaderMap& MaterialImpl::GetShaders() const
{
	return m_Shaders;
}


std::shared_ptr<Texture> MaterialImpl::GetTexture(uint8 type) const
{
	TextureMap::const_iterator itr = m_Textures.find(type);
	if (itr != m_Textures.end())
	{
		return itr->second;
	}

	return nullptr;
}

void MaterialImpl::SetTexture(uint8 type, std::shared_ptr<Texture> texture)
{
	m_Textures[type] = texture;
	m_Dirty = true;
}


void MaterialImpl::Bind() const
{
	if (m_Dirty)
	{
		std::shared_ptr<Material> wrapper = m_Wrapper.lock();
		if (wrapper)
			wrapper->UpdateConstantBuffer();
		m_Dirty = false;
	}

	for (auto shader : m_Shaders)
	{
		std::shared_ptr<Shader> pShader = shader.second;
		if (pShader)
		{
			pShader->Bind();

			for (auto texture : m_Textures)
			{
				std::shared_ptr<Texture> pTexture = texture.second;
				//if (pTexture != nullptr)
				pTexture->Bind((uint32_t)texture.first, pShader, ShaderParameter::Type::Texture);
			}

			ShaderParameter& materialParameter = pShader->GetShaderParameterByName("Material");
			if (materialParameter.IsValid() && m_pConstantBuffer != nullptr)
			{
				materialParameter.Set<ConstantBuffer>(m_pConstantBuffer);
				materialParameter.Bind();
			}
		}
	}
}

void MaterialImpl::Unbind() const
{
	for (auto shader : m_Shaders)
	{
		std::shared_ptr<Shader> pShader = shader.second;
		if (pShader)
		{
			for (auto texture : m_Textures)
			{
				std::shared_ptr<Texture> pTexture = texture.second;
				//if (pTexture != nullptr)
				pTexture->UnBind((uint32_t)texture.first, pShader, ShaderParameter::Type::Texture);
			}

			ShaderParameter& materialParameter = pShader->GetShaderParameterByName("Material");
			if (materialParameter.IsValid() && m_pConstantBuffer != nullptr)
			{
				materialParameter.UnBind();
			}

			pShader->UnBind();
		}
	}
}

//--

void MaterialImpl::SetWrapper(std::weak_ptr<Material> _wrapper)
{
	m_Wrapper = _wrapper;
}

void MaterialImpl::CreateConstantBuffer(const void* data, size_t size)
{
	m_pConstantBuffer = m_RenderDevice->CreateConstantBuffer(data, size);
}

void MaterialImpl::UpdateConstantBuffer() const
{
	// It's empty
}

void MaterialImpl::UpdateConstantBuffer(const void* _data, size_t size) const
{
	if (m_pConstantBuffer)
	{
		m_pConstantBuffer->Set(_data, size);
	}
}

void MaterialImpl::MarkConstantBufferDirty()
{
	m_Dirty = true;
}
