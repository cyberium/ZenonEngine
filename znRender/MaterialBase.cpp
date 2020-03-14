#include "stdafx.h"

// General
#include "MaterialBase.h"

MaterialBase::MaterialBase(IRenderDevice& RenderDevice, size_t Size)
	: m_Name("MaterialBase")
	, m_RenderDevice(RenderDevice)
	, m_Dirty(true)
	, m_pConstantBuffer(nullptr)
{
	if (Size > 0)
	{
		m_pConstantBuffer = m_RenderDevice.GetObjectsFactory().CreateConstantBuffer(nullptr, Size);
	}
}

MaterialBase::~MaterialBase()
{
}



//
// IMaterial
//
void MaterialBase::SetName(const std::string & Name)
{
	m_Name = Name;
}

std::string MaterialBase::GetName() const
{
	return m_Name;
}

void MaterialBase::SetShader(EShaderType type, const std::shared_ptr<IShader> pShader)
{
	m_Shaders[type] = pShader;
}

const std::shared_ptr<IShader>& MaterialBase::GetShader(EShaderType type) const
{
	const auto& iter = m_Shaders.find(type);
	if (iter == m_Shaders.end())
		throw CException(L"Shader not found.");

	return iter->second;
}

const ShaderMap& MaterialBase::GetShaders() const
{
	return m_Shaders;
}

void MaterialBase::SetTexture(uint8 ID, const std::shared_ptr<ITexture> texture)
{
	m_Textures[ID] = texture;
	m_Dirty = true;
}

const std::shared_ptr<ITexture>& MaterialBase::GetTexture(uint8 ID) const
{
	const auto& iter = m_Textures.find(ID);
	if (iter == m_Textures.end())
		throw CException(L"Texture not found.");

	return iter->second;
}

void MaterialBase::SetSampler(uint8 ID, const std::shared_ptr<ISamplerState> samplerState)
{
	m_Samplers[ID] = samplerState;
	m_Dirty = true;
}

const std::shared_ptr<ISamplerState>& MaterialBase::GetSampler(uint8 ID) const
{
    const auto& iter = m_Samplers.find(ID);
    if (iter == m_Samplers.end())
		throw CException(L"Sampler not found.");

    return iter->second;
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
