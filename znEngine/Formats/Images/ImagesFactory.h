#pragma once

class ZN_API CImagesFactory
	: public IImagesFactory
{
public:
	CImagesFactory(const IBaseManager& BaseManager);
	virtual ~CImagesFactory();

	// IImagesFactory
	void AddImageLoader(std::shared_ptr<IImageLoader> ImageLoader);
	void RemoveImageLoader(std::shared_ptr<IImageLoader> ImageLoader);
	std::shared_ptr<IImage> CreateImage(std::shared_ptr<IFile> File) const;

private:
	std::vector<std::shared_ptr<IImageLoader>> m_ImageLoaders;

private:
	const IBaseManager& m_BaseManager;
};