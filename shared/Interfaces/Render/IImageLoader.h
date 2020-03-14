#pragma once

// FORWARD BEGIN
ZN_INTERFACE IFile;
ZN_INTERFACE IManager;
// FORWARD END

ZN_INTERFACE ZN_API IImage
{
	virtual ~IImage() {}

	virtual uint32 GetWidth() const = 0;
	virtual uint32 GetHeight() const = 0;
	virtual uint32 GetBitsPerPixel() const = 0;
	virtual uint32 GetStride() const = 0;
	virtual bool IsTransperent() const = 0;
	virtual const uint8* GetData() const = 0;
	virtual void Resize(uint32 NewWidth, uint32 NewHeight) = 0;
};

ZN_INTERFACE ZN_API IImageLoader
{
	virtual ~IImageLoader() {}

	virtual bool IsFileSupported(std::shared_ptr<IFile> File) const = 0;
	virtual std::shared_ptr<IImage> CreateImage(std::shared_ptr<IFile> File) const = 0;
};

ZN_INTERFACE ZN_API 
	__declspec(uuid("6D65A347-1C79-472E-BFAB-0C1C04597FAE"))
	IImagesFactory : public IManager
{
	virtual ~IImagesFactory() {}

	virtual void AddImageLoader(const std::shared_ptr<IImageLoader>& ImageLoader) = 0;
	virtual void RemoveImageLoader(const std::shared_ptr<IImageLoader>& ImageLoader) = 0;
	virtual std::shared_ptr<IImage> CreateImage(const std::string& FileName) = 0;
	virtual std::shared_ptr<IImage> CreateImage(const std::shared_ptr<IFile>& File) = 0;
};
