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


std::shared_ptr<Texture> MaterialImpl::GetTexture(uint8 ID) const
{
	TextureMap::const_iterator itr = m_Textures.find(ID);
	if (itr != m_Textures.end())
	{
		return itr->second;
	}

	return std::shared_ptr<Texture>();
}

void MaterialImpl::SetTexture(uint8 ID, std::shared_ptr<Texture> texture)
{
	m_Textures[ID] = texture;
	m_Dirty = true;
}

std::shared_ptr<SamplerState> MaterialImpl::GetSampler(uint8 ID) const
{
    SamplersMap::const_iterator itr = m_Samplers.find(ID);
    if (itr != m_Samplers.end())
    {
        return itr->second;
    }

    return std::shared_ptr<SamplerState>();
}

void MaterialImpl::SetSampler(uint8 ID, std::shared_ptr<SamplerState> samplerState)
{
    m_Samplers[ID] = samplerState;
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

			for (auto textureIt : m_Textures)
			{
				std::shared_ptr<Texture> texture = textureIt.second;
				//if (pTexture != nullptr)
                texture->Bind((uint32_t)textureIt.first, pShader, ShaderParameter::Type::Texture);
			}

            for (auto samplerStateIt : m_Samplers)
            {
                std::shared_ptr<SamplerState> samplerState = samplerStateIt.second;
                samplerState->Bind((uint32_t)samplerStateIt.first, pShader, ShaderParameter::Type::Sampler);
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
			for (auto textureIt : m_Textures)
			{
				std::shared_ptr<Texture> texture = textureIt.second;
				texture->UnBind((uint32_t)textureIt.first, pShader, ShaderParameter::Type::Texture);
			}

            for (auto samplerStateIt : m_Samplers)
            {
                std::shared_ptr<SamplerState> samplerState = samplerStateIt.second;
                samplerState->UnBind((uint32_t)samplerStateIt.first, pShader, ShaderParameter::Type::Sampler);
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
