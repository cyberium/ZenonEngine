#include "stdafx.h"

// General
#include "ImagesFactory.h"

CImagesFactory::CImagesFactory(const IBaseManager& BaseManager)
	: m_BaseManager(BaseManager)
{
}

CImagesFactory::~CImagesFactory()
{
}



//
// IImagesFactory
//
void CImagesFactory::AddImageLoader(const std::shared_ptr<IImageLoader>& ImageLoader)
{
	_ASSERT(ImageLoader != nullptr);
	m_ImageLoaders.push_back(ImageLoader);
}

void CImagesFactory::RemoveImageLoader(const std::shared_ptr<IImageLoader>& ImageLoader)
{
	const auto& it = std::find(m_ImageLoaders.begin(), m_ImageLoaders.end(), ImageLoader);
	if (it == m_ImageLoaders.end())
		return;

	m_ImageLoaders.erase(it);
}

std::shared_ptr<IImage> CImagesFactory::CreateEmptyImage(const std::string & FileName, uint32 Width, uint32 Height, uint32 BitsPerPixel)
{
	for (const auto& loader : m_ImageLoaders)
	{
		if (loader->IsFilenameSupported(FileName))
		{
			return loader->CreateEmptyImage(Width, Height, BitsPerPixel);
		}
	}

	Log::Error("CImagesFactory: Image '%s' not supported.", FileName.c_str());
	return nullptr;
}

std::shared_ptr<IImage> CImagesFactory::CreateImage(const std::string& FileName)
{
	return CreateImage(m_BaseManager.GetManager<IFilesManager>()->Open(FileName));
}

std::shared_ptr<IImage> CImagesFactory::CreateImage(const std::shared_ptr<IFile>& File)
{
	if (File == nullptr)
		throw CException("ImagesFactory: Can't load null file.");

	const auto& iter = m_ImagesByName.find(File->Path_Name());
	if (iter != m_ImagesByName.end())
	{
		if (auto image = iter->second.lock())
		{
			return image;
		}
		else
		{
			m_ImagesByName.erase(iter);
		}
	}

	for (const auto& loader : m_ImageLoaders)
	{
		if (loader->IsFileSupported(File))
		{
			auto image = loader->CreateImage(File);
			m_ImagesByName[File->Path_Name()] = image;
			return image;
		}
	}

	Log::Error("CImagesFactory: Image '%s' not supported.", File->Name().c_str());
	return nullptr;
}
