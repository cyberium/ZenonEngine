#pragma once

class ZN_API CImagesFactory
	: public IImagesFactory
{
public:
	CImagesFactory(const IBaseManager& BaseManager);
	virtual ~CImagesFactory();

	// IImagesFactory
	void AddImageLoader(const std::shared_ptr<IImageLoader>& ImageLoader) override;
	void RemoveImageLoader(const std::shared_ptr<IImageLoader>& ImageLoader) override;
	std::shared_ptr<IImage> CreateImage(const std::string& FileName) override;
	std::shared_ptr<IImage> CreateImage(const std::shared_ptr<IFile>& File) override;

private:
	std::vector<std::shared_ptr<IImageLoader>> m_ImageLoaders;
	std::unordered_map<std::string, std::weak_ptr<IImage>> m_ImagesByName;

private:
	const IBaseManager& m_BaseManager;
};