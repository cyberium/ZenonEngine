#include "stdafx.h"

// General
#include "MaterialBase.h"

MaterialBase::MaterialBase(IRenderDevice& RenderDevice)
	: m_BufferSize(0)
	, m_RenderDevice(RenderDevice)
	, m_Dirty(true)
	, m_MaterialData(nullptr)
{
}

MaterialBase::~MaterialBase()
{
	FinalizeMaterialData();
}



//
// IMaterial
//
void MaterialBase::SetTexture(uint8 ID, const std::shared_ptr<ITexture> texture)
{
	m_Textures[ID] = texture;
	m_Dirty = true;
}

const std::shared_ptr<ITexture>& MaterialBase::GetTexture(uint8 ID) const
{
	const auto& iter = m_Textures.find(ID);
	if (iter == m_Textures.end())
		throw CznRenderException("MaterialBase: Texture with index '%d' not found in material '%s'.", ID, GetName().c_str());

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
		throw CznRenderException("MaterialBase: Sampler with index '%d' not found in material '%s'.", ID, GetName().c_str());

    return iter->second;
}


void MaterialBase::Bind(const ShaderMap& shaders) const
{
	if (m_Dirty)
	{
		if (m_ConstantBuffer)
			m_ConstantBuffer->Set(m_MaterialData, m_BufferSize);

		m_Dirty = false;
	}
}

void MaterialBase::Unbind(const ShaderMap& shaders) const
{

}



//
// IMaterialDataOwner
//
void MaterialBase::InitializeMaterialData(size_t BufferSize)
{
	_ASSERT(BufferSize > 0);

	if (BufferSize == m_BufferSize)
		return;

	// Delete current material data
	FinalizeMaterialData();

	m_BufferSize = BufferSize;
	if (m_BufferSize > 0)
	{
		m_MaterialData = _aligned_malloc(m_BufferSize, 16);
		m_ConstantBuffer = m_RenderDevice.GetObjectsFactory().CreateConstantBuffer(m_MaterialData, m_BufferSize);
	}
}

const void * MaterialBase::GetMaterialData() const
{
	return m_MaterialData;
}

void * MaterialBase::GetMaterialDataEx()
{
	return m_MaterialData;
}

void MaterialBase::MarkMaterialDataDirty()
{
	m_Dirty = true;
}


//
// IObjectLoadSave
//

void MaterialBase::Load(const std::shared_ptr<IByteBuffer>& ByteBuffer)
{
	ByteBuffer->read(&m_BufferSize);
	if (m_BufferSize > 0)
	{
		InitializeMaterialData(m_BufferSize);
		ByteBuffer->readBytes(m_MaterialData, m_BufferSize);
	}

	size_t texturesCount;
	ByteBuffer->read(&texturesCount);
	//Log::Info("Material: Load '%d' textures.", texturesCount);
	for (size_t i = 0; i < texturesCount; i++)
	{
		uint8 textureID;
		ByteBuffer->read(&textureID);
		std::string textureFileName;
		ByteBuffer->readString(&textureFileName);

		if (m_Textures.find(textureID) != m_Textures.end())
		{
			Log::Error("MaterialBase: Texture with index '%d' already set.", textureID);
			continue;
		}

		auto texture = m_RenderDevice.GetBaseManager().GetManager<IznTexturesFactory>()->LoadTexture2D(textureFileName);
		if (texture == nullptr)
		{
			Log::Error("MaterialBase: Unable load texture: '%s' with index '%d' for material '%s'.", textureFileName.c_str(), textureID, GetName().c_str());
			texture = m_RenderDevice.GetBaseManager().GetManager<IznTexturesFactory>()->GetDefaultTexture();
		}

		SetTexture(textureID, texture);
		//Log::Info("Material: Load '%s' texture '%s'.", GetTextureTypeName(textureIndex).c_str(), textureFileName.c_str());
	}
}

void MaterialBase::Save(const std::shared_ptr<IByteBuffer>& ByteBuffer) const
{
	// Material data
	ByteBuffer->write(&m_BufferSize);
	if (m_BufferSize > 0)
		ByteBuffer->writeBytes(m_MaterialData, m_BufferSize);

	// Textures
	size_t texturesCount = m_Textures.size();
	ByteBuffer->write(&texturesCount);
	//Log::Info("Material: Saving '%d' textures.", texturesCount);

	for (const auto& texture : m_Textures)
	{
		if (texture.second == nullptr)
			throw CException("NullPtr texture for m_Texture[%d].", texture.first);

		const auto fileName = texture.second->GetFilename();
		if (fileName.empty())
			throw CException("Empty filename for m_Texture[%d].", texture.first);

		// Тупая проверка. Но для меня сейчас это надежда найти всякую хуйню с менеджером тексутр.
		_ASSERT(m_RenderDevice.GetBaseManager().GetManager<IznTexturesFactory>()->LoadTexture2D(fileName) == texture.second);

		ByteBuffer->write(&texture.first);
		ByteBuffer->writeString(fileName);
		//Log::Info("Material: Save '%s' texture '%s'.", GetTextureTypeName(texture.first).c_str(), fileName.c_str());
	}
}

void MaterialBase::Load(const std::shared_ptr<IXMLReader>& Reader)
{
	auto texturesReader = Reader->GetChild("Textures");
	for (const auto& texture : texturesReader->GetChilds())
	{
		_ASSERT(texture->GetName() == "Texture");
		uint8 textureID = texture->GetUInt8Attribute("Index");

		if (m_Textures.find(textureID) != m_Textures.end())
		{
			Log::Error("MaterialBase: Texture with ID '%d' already set.", textureID);
			continue;
		}

		std::string textureFileName = texture->GetValue();

		auto texture = m_RenderDevice.GetBaseManager().GetManager<IznTexturesFactory>()->LoadTexture2D(textureFileName);
		if (texture == nullptr)
		{
			Log::Error("MaterialBase: Unable load texture: '%s' with ID '%d' for material '%s'.", textureFileName.c_str(), textureID, GetName().c_str());
			texture = m_RenderDevice.GetBaseManager().GetManager<IznTexturesFactory>()->GetDefaultTexture();
		}

		SetTexture(textureID, texture);
		//Log::Info("Material: Load '%s' texture '%s'.", GetTextureTypeName(textureIndex).c_str(), textureFileName.c_str());
	}
}

void MaterialBase::Save(const std::shared_ptr<IXMLWriter>& Writer) const
{
	auto texturesWriter = Writer->CreateChild("Textures");
	for (const auto& texture : m_Textures)
	{
		auto textureWriter = texturesWriter->CreateChild("Texture");
		textureWriter->SetUInt8Attribute(texture.first, "Index");

		const auto fileName = texture.second->GetFilename();
		if (fileName.empty())
			throw CException("Empty filename for texture with ID '%d'.", texture.first);
		textureWriter->SetValue(fileName);
	}
}




//
// Protected
//
void MaterialBase::FinalizeMaterialData()
{
	if (m_MaterialData != nullptr)
	{
		_aligned_free(m_MaterialData);
		m_MaterialData = nullptr;
	}

	m_ConstantBuffer.reset();
}
