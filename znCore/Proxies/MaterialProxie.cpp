#include "stdafx.h"

// General
#include "MaterialProxie.h"

MaterialProxie::MaterialProxie(std::shared_ptr<IMaterial> _materal)
	: m_Material(_materal)
{
	_ASSERT(m_Material != nullptr);

	SetType(0);
	SetName("MaterialProxie");
}

MaterialProxie::~MaterialProxie()
{}



//
// IMaterial
//
void MaterialProxie::SetType(MaterialType Type)
{
	m_Material->SetType(Type);
}

MaterialType MaterialProxie::GetType() const
{
	return m_Material->GetType();
}

bool MaterialProxie::Is(MaterialType MaterialType) const
{
	return m_Material->Is(MaterialType);
}

void MaterialProxie::SetName(const std::string & Name)
{
	m_Material->SetName(Name);
}

std::string MaterialProxie::GetName() const
{
	return m_Material->GetName();
}

void MaterialProxie::SetTexture(uint8 type, const std::shared_ptr<ITexture> texture)
{
	m_Material->SetTexture(type, texture);
}

const std::shared_ptr<ITexture>& MaterialProxie::GetTexture(uint8 ID) const
{
	return m_Material->GetTexture(ID);
}

void MaterialProxie::SetSampler(uint8 ID, const std::shared_ptr<ISamplerState> samplerState)
{
	m_Material->SetSampler(ID, samplerState);
}

const std::shared_ptr<ISamplerState>& MaterialProxie::GetSampler(uint8 ID) const
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



//
// ILoadableFromFile
//
void MaterialProxie::Load(const std::shared_ptr<IByteBuffer>& ByteBuffer)
{
	if (const auto& loadableFromFile = std::dynamic_pointer_cast<ILoadableFromFile>(m_Material))
	{
		loadableFromFile->Load(ByteBuffer);
	}
	else
		_ASSERT(false);
}

void MaterialProxie::Save(const std::shared_ptr<IByteBuffer>& ByteBuffer)
{
	if (const auto& loadableFromFile = std::dynamic_pointer_cast<ILoadableFromFile>(m_Material))
	{
		loadableFromFile->Save(ByteBuffer);
	}
	else
		_ASSERT(false);
}



//
// IMaterialDataOwner
//
void MaterialProxie::InitializeMaterialData(size_t BufferSize)
{
	return std::dynamic_pointer_cast<IMaterialDataOwner>(m_Material)->InitializeMaterialData(BufferSize);
}

const void * MaterialProxie::GetMaterialData() const
{
	return std::dynamic_pointer_cast<const IMaterialDataOwner>(m_Material)->GetMaterialData();
}

void * MaterialProxie::GetMaterialDataEx()
{
	return std::dynamic_pointer_cast<IMaterialDataOwner>(m_Material)->GetMaterialDataEx();
}

void MaterialProxie::MarkMaterialDataDirty()
{
	std::dynamic_pointer_cast<IMaterialDataOwner>(m_Material)->MarkMaterialDataDirty();
}