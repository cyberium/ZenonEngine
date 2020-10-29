#include "stdafx.h"

// General
#include "TexturesFactory.h"

CznTexturesFactory::CznTexturesFactory(IBaseManager& BaseManager)
	: m_BaseManager(BaseManager)
{
}

CznTexturesFactory::~CznTexturesFactory()
{
}



//
// IznTexturesFactory
//
std::shared_ptr<ITexture> CznTexturesFactory::LoadTexture2D(const std::string& TextureFileName) 
{
	if (auto textureFromCache = TryGetTextureFromCache(TextureFileName))
		return textureFromCache;

	auto textureFile = m_BaseManager.GetManager<IFilesManager>()->Open(TextureFileName);
	if (textureFile == nullptr)
		return GetDefaultTexture();

	return LoadTexture2D(textureFile);
}

std::shared_ptr<ITexture> CznTexturesFactory::LoadTexture2D(const std::shared_ptr<IFile>& TextureFile)
{
	if (TextureFile == nullptr)
		throw CException("Can't load nullptr file.");

	if (auto textureFromCache = TryGetTextureFromCache(TextureFile->Path_Name()))
		return textureFromCache;

	std::shared_ptr<IImage> image = m_BaseManager.GetManager<IImagesFactory>()->CreateImage(TextureFile);
	if (image == nullptr)
		return GetDefaultTexture();

	IRenderDevice& renderDevice = m_BaseManager.GetApplication().GetRenderDevice();
	std::shared_ptr<ITexture> texture = renderDevice.GetObjectsFactory().CreateEmptyTexture();
	if (false == texture->LoadTextureFromImage(image))
		return GetDefaultTexture();

	m_TexturesByName[TextureFile->Path_Name()] = texture;
	return texture;
}

std::shared_ptr<IFile> CznTexturesFactory::SaveTexture(const std::shared_ptr<ITexture>& Texture, const std::string & FileName) const
{
	return std::shared_ptr<IFile>();
}

std::shared_ptr<ITexture> CznTexturesFactory::GetDefaultTexture()
{
	if (m_DefaultTexture == nullptr)
		m_DefaultTexture = LoadTexture2D("IDB_DEFAULT_TEXTURE");
	return m_DefaultTexture;
}



//
// Protected
//
std::shared_ptr<ITexture> CznTexturesFactory::TryGetTextureFromCache(const std::string & TextureFileName)
{
	const auto& iter = m_TexturesByName.find(TextureFileName);
	if (iter == m_TexturesByName.end())
		return nullptr;

	if (auto texture = iter->second.lock())
		return texture;
	
	m_TexturesByName.erase(iter);
	return nullptr;
}
