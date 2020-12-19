#pragma once

class CznTexturesFactory
	: public IznTexturesFactory
{
public:
	CznTexturesFactory(IBaseManager& BaseManager);
	virtual ~CznTexturesFactory();

	// IznTexturesFactory
	virtual std::shared_ptr<ITexture> LoadTexture2D(const std::string& TextureFileName) override;
	virtual std::shared_ptr<ITexture> LoadTexture2D(const std::shared_ptr<IFile>& TextureFile) override;

	virtual std::shared_ptr<ITexture> LoadTextureCube(const std::string& CubeTextureFilename) override;

	virtual std::shared_ptr<IFile> SaveTexture(const std::shared_ptr<ITexture>& Texture, const std::string& FileName) const override;
	
	std::shared_ptr<ITexture> GetDefaultTexture() override;

protected:
	std::shared_ptr<ITexture> TryGetTextureFromCache(const std::string& TextureFileName);

private:
	std::unordered_map<std::string, std::weak_ptr<ITexture>> m_TexturesByName;
	mutable std::mutex m_LockMutex;
	std::shared_ptr<ITexture> m_DefaultTexture;

private:
	IBaseManager& m_BaseManager;
};
