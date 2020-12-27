#pragma once

// FORWARD BEGIN
ZN_INTERFACE IManager;
ZN_INTERFACE IFile;
// FORWARD END


ZN_INTERFACE ZN_API IImage
	: public std::enable_shared_from_this<IImage>
{
	virtual ~IImage() {}

	virtual uint32 GetWidth() const = 0;
	virtual uint32 GetHeight() const = 0;
	virtual uint32 GetBitsPerPixel() const = 0;
	virtual uint32 GetStride() const = 0;
	virtual bool IsTransperent() const = 0;

	virtual const uint8* GetData() const = 0;
	virtual uint8* GetDataEx() = 0;

	virtual const uint8* GetLine(uint32 Line) const = 0;
	virtual uint8* GetLineEx(uint32 Line) = 0;

	virtual size_t GetDataSize() const = 0;
	virtual void Resize(uint32 NewWidth, uint32 NewHeight) = 0;
	virtual std::shared_ptr<IImage> ConvertAnyTo32Bit() = 0;
	virtual std::shared_ptr<IByteBuffer> SaveToMemory() = 0;
};


ZN_INTERFACE ZN_API IImageLoader
{
	virtual ~IImageLoader() {}

	virtual bool IsFilenameSupported(const std::string& Filename) const = 0;
	virtual bool IsFileSupported(std::shared_ptr<IFile> File) const = 0;
	virtual std::shared_ptr<IImage> CreateEmptyImage(uint32 Width, uint32 Height, uint32 BitsPerPixel) const = 0;
	virtual std::shared_ptr<IImage> CreateImage(std::shared_ptr<IFile> File) const = 0;
};


ZN_INTERFACE ZN_API IImagesFactory 
	: public IManager
{
	ZN_OBJECTCLASS(cImagesFactory)

	virtual ~IImagesFactory() {}

	virtual void AddImageLoader(const std::shared_ptr<IImageLoader>& ImageLoader) = 0;
	virtual void RemoveImageLoader(const std::shared_ptr<IImageLoader>& ImageLoader) = 0;
	virtual std::shared_ptr<IImage> CreateEmptyImage(const std::string& FileName, uint32 Width, uint32 Height, uint32 BitsPerPixel) = 0;
	virtual std::shared_ptr<IImage> CreateImage(const std::string& FileName) = 0;
	virtual std::shared_ptr<IImage> CreateImage(const std::shared_ptr<IFile>& File) = 0;
};
