#pragma once

class CznEngineModelsLoader
	: public IznModelsLoader
{
public:
	CznEngineModelsLoader(IBaseManager& BaseManager);
	virtual ~CznEngineModelsLoader();

	// IznModelsLoader
	std::string GetName() const override;
	bool IsSupportedFormat(const std::string& ModelFileName) const override;
	bool IsSupportedFormat(const std::shared_ptr<IFile>& ModelFile) const override;
	std::shared_ptr<IModel> LoadModel(const std::string& ModelFileName, const IznLoaderParams* LoaderParams = nullptr) const override;
	std::shared_ptr<IModel> LoadModel(const std::shared_ptr<IFile>& ModelFile, const IznLoaderParams* LoaderParams) const override;
	std::shared_ptr<IFile> SaveModel(const std::shared_ptr<IModel>& Model, const std::string& FileName) const override;

private:
	IBaseManager& m_BaseManager;
};