#pragma once

// FORWARD BEGIN
ZN_INTERFACE IManager;
ZN_INTERFACE ITexture;
// FORWARD END



ZN_INTERFACE ZN_API	IznTexturesFactory 
	: public IManager
{
	ZN_OBJECTCLASS(cTexturesFactory)

	virtual ~IznTexturesFactory() {};

	virtual std::shared_ptr<ITexture> LoadTexture2D(const std::string& TextureFileName) = 0;
	virtual std::shared_ptr<ITexture> LoadTexture2D(const std::shared_ptr<IFile>& TextureFile) = 0;

	virtual std::shared_ptr<IFile> SaveTexture(const std::shared_ptr<ITexture>& Texture, const std::string& FileName) const = 0;

	virtual std::shared_ptr<ITexture> GetDefaultTexture() = 0;
};
