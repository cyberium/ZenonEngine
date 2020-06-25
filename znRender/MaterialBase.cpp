#include "stdafx.h"

// General
#include "MaterialBase.h"

MaterialBase::MaterialBase(IRenderDevice& RenderDevice)
	: m_BufferSize(0)
	, m_RenderDevice(RenderDevice)
	, m_Dirty(true)
	, m_MaterialData(nullptr)
{
	SetClassName("MaterialBase");
}

MaterialBase::~MaterialBase()
{
	if (m_MaterialData)
	{
		_aligned_free(m_MaterialData);
		m_MaterialData = nullptr;
	}
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
// ILoadableFromFile
//
void MaterialBase::Load(const std::shared_ptr<IByteBuffer>& ByteBuffer)
{
	//ByteBuffer->readString(&m_Name);

	ByteBuffer->read(&m_BufferSize);
	if (m_BufferSize > 0)
	{
		InitializeMaterialData(m_BufferSize);
		ByteBuffer->readBytes(m_MaterialData, m_BufferSize);
	}

	size_t texturesCount;
	ByteBuffer->read(&texturesCount);
	for (size_t i = 0; i < texturesCount; i++)
	{
		uint8 textureIndex;
		ByteBuffer->read(&textureIndex);

		std::string textureFileName;
		ByteBuffer->readString(&textureFileName);

		auto texture = m_RenderDevice.GetObjectsFactory().LoadTexture2D(textureFileName);
		if (texture == nullptr)
		{
			Log::Error("MaterialBase: Unable load texture: '%s' with index '%d' for material '%s'.", textureFileName.c_str(), textureIndex, GetName().c_str());
			texture = m_RenderDevice.GetDefaultTexture();
		}

		if (m_Textures.find(textureIndex) != m_Textures.end())
		{
			Log::Error("MaterialBase: Texture with index '%d' already set.", textureIndex);
		}

		m_Textures.insert(std::make_pair(textureIndex, texture));
	}
}

void MaterialBase::Save(const std::shared_ptr<IByteBuffer>& ByteBuffer)
{
	// Name
	//ByteBuffer->writeString(m_Name);

	// Material data
	ByteBuffer->write(&m_BufferSize);
	if (m_BufferSize > 0)
		ByteBuffer->writeBytes(m_MaterialData, m_BufferSize);

	// Textures
	size_t texturesCount = m_Textures.size();
	ByteBuffer->write(&texturesCount);
	for (const auto& it : m_Textures)
	{
		if (it.second == nullptr)
			throw CException("NullPtr texture for m_Texture[%d].", it.first);

		if (auto fileNameOwner = std::dynamic_pointer_cast<IFileNameOwner>(it.second))
		{
			const auto fileName = fileNameOwner->GetFileName();
			if (fileName.empty())
				throw CException("Empty filename for m_Texture[%d].", it.first);

			// Тупая проверка. Но для меня сейчас это надежда найти всякую хуйню с менеджером тексутр.
			_ASSERT(m_RenderDevice.GetObjectsFactory().LoadTexture2D(fileName) == it.second);

			ByteBuffer->write(&it.first);
			ByteBuffer->writeString(fileNameOwner->GetFileName());
		}
		else
			throw CException("Texture [%d] don't support 'IFileNameOwner'.");
	}
}



//
// IMaterialDataOwner
//
void MaterialBase::InitializeMaterialData(size_t BufferSize)
{
	_ASSERT(BufferSize > 0);

	if (BufferSize == m_BufferSize)
		return;

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

void MaterialBase::FinalizeMaterialData()
{
	if (m_MaterialData != nullptr)
	{
		_aligned_free(m_MaterialData);
		m_MaterialData = nullptr;

		m_ConstantBuffer.reset();

		//throw CznRenderException("MaterialBase: Material data already initialized for '%s'.", m_Name.c_str());
	}
}
