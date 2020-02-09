#include "stdafx.h"

// General
#include "ImagesFactory.h"

CImagesFactory::CImagesFactory(const IBaseManager* BaseManager)
	: m_BaseManager(BaseManager)
{
}

CImagesFactory::~CImagesFactory()
{
}



//
// IImagesFactory
//
void CImagesFactory::AddImageLoader(std::shared_ptr<IImageLoader> ImageLoader)
{
	_ASSERT(ImageLoader != nullptr);
	m_ImageLoaders.push_back(ImageLoader);
}

void CImagesFactory::RemoveImageLoader(std::shared_ptr<IImageLoader> ImageLoader)
{
	auto it = std::find(m_ImageLoaders.begin(), m_ImageLoaders.end(), ImageLoader);
	if (it == m_ImageLoaders.end())
		return;

	m_ImageLoaders.erase(it);
}

std::shared_ptr<IImage> CImagesFactory::CreateImage(std::shared_ptr<IFile> File) const
{
	if (File == nullptr)
	{
		Log::Error("CImagesFactory: Can't load null file.");
		return nullptr;
	}

	for (const auto& loader : m_ImageLoaders)
	{
		if (loader->IsFileSupported(File))
		{
			return loader->CreateImage(File);
		}
	}

	Log::Error("CImagesFactory: Image '%s' not supported.", File->Name().c_str());
	return nullptr;
}
