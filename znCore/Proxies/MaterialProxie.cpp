#include "stdafx.h"

// General
#include "MaterialProxie.h"

MaterialProxie::MaterialProxie(std::shared_ptr<IMaterial> Material)
	: CObjectProxy(Material)
	, m_Material(Material)
{
	_ASSERT(m_Material != nullptr);
}

MaterialProxie::~MaterialProxie()
{}



//
// IMaterial
//

void MaterialProxie::SetTexture(uint8 type, const std::shared_ptr<ITexture> texture)
{
	m_Material->SetTexture(type, texture);
}

std::shared_ptr<ITexture> MaterialProxie::GetTexture(uint8 ID) const
{
	return m_Material->GetTexture(ID);
}

void MaterialProxie::SetSampler(uint8 ID, const std::shared_ptr<ISamplerState> samplerState)
{
	m_Material->SetSampler(ID, samplerState);
}

std::shared_ptr<ISamplerState> MaterialProxie::GetSampler(uint8 ID) const
{
    return m_Material->GetSampler(ID);
}

void MaterialProxie::Bind(const IShader* PixelShader) const
{
	m_Material->Bind(PixelShader);
}

void MaterialProxie::Unbind(const IShader* PixelShader) const
{
	m_Material->Unbind(PixelShader);
}



//
// IObjectLoadSave
//
void MaterialProxie::Load(const std::shared_ptr<IByteBuffer>& ByteBuffer)
{
	if (const auto& loadableFromFile = std::dynamic_pointer_cast<IObjectLoadSave>(m_Material))
	{
		loadableFromFile->Load(ByteBuffer);
	}
	else
		_ASSERT(false);
}

void MaterialProxie::Save(const std::shared_ptr<IByteBuffer>& ByteBuffer) const
{
	if (const auto& loadableFromFile = std::dynamic_pointer_cast<IObjectLoadSave>(m_Material))
	{
		loadableFromFile->Save(ByteBuffer);
	}
	else
		_ASSERT(false);
}

void MaterialProxie::Load(const std::shared_ptr<IXMLReader>& Reader)
{
	_ASSERT(false);
}

void MaterialProxie::Save(const std::shared_ptr<IXMLWriter>& Writer) const
{
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

