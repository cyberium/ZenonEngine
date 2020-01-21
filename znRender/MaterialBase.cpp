#include "stdafx.h"

// General
#include "MaterialBase.h"

MaterialBase::MaterialBase(IRenderDevice* RenderDevice, size_t Size)
	: m_Name("MaterialBase")
	, m_RenderDevice(RenderDevice)
	, m_Dirty(true)
{
	if (Size > 0)
	{
		m_pConstantBuffer = m_RenderDevice->CreateConstantBuffer(nullptr, Size);
	}
}

MaterialBase::~MaterialBase()
{
	if (m_pConstantBuffer)
	{
		m_RenderDevice->DestroyConstantBuffer(m_pConstantBuffer);
	}
}

void MaterialBase::SetName(const std::string & Name)
{
	m_Name = Name;
}

std::string MaterialBase::GetName() const
{
	return m_Name;
}


void MaterialBase::SetShader(EShaderType type, IShader* pShader)
{
	m_Shaders[type] = pShader;
}

IShader* MaterialBase::GetShader(EShaderType type) const
{
	const auto& iter = m_Shaders.find(type);
	if (iter != m_Shaders.end())
	{
		return iter->second;
	}

	return nullptr;
}

const ShaderMap& MaterialBase::GetShaders() const
{
	return m_Shaders;
}


ITexture* MaterialBase::GetTexture(uint8 ID) const
{
	const auto& itr = m_Textures.find(ID);
	if (itr != m_Textures.end())
	{
		return itr->second;
	}

	return nullptr;
}

void MaterialBase::SetTexture(uint8 ID, ITexture* texture)
{
	m_Textures[ID] = texture;
	m_Dirty = true;
}

ISamplerState* MaterialBase::GetSampler(uint8 ID) const
{
    const auto& itr = m_Samplers.find(ID);
    if (itr != m_Samplers.end())
    {
        return itr->second;
    }

    return nullptr;
}

void MaterialBase::SetSampler(uint8 ID, ISamplerState* samplerState)
{
    m_Samplers[ID] = samplerState;
    m_Dirty = true;
}


void MaterialBase::Bind(const ShaderMap& shaders) const
{
	if (m_Dirty)
	{
		if (m_Wrapper)
			m_Wrapper->UpdateConstantBuffer();
		m_Dirty = false;
	}
}

void MaterialBase::Unbind(const ShaderMap& shaders) const
{

}

//--

void MaterialBase::SetWrapper(IMaterial* _wrapper)
{
	m_Wrapper = _wrapper;
}

void MaterialBase::UpdateConstantBuffer() const
{
	// It's empty
}

void MaterialBase::UpdateConstantBuffer(const void* _data, size_t size) const
{
	if (m_pConstantBuffer)
	{
		m_pConstantBuffer->Set(_data, size);
	}
}

void MaterialBase::MarkConstantBufferDirty()
{
	m_Dirty = true;
}
