#pragma once

class CznModelsFactory
	: public IznModelsFactory
{
public:
	CznModelsFactory(IBaseManager& BaseManager);
	virtual ~CznModelsFactory();

	// IznModelsFactory
	void AddModelsLoader(const std::shared_ptr<IznModelsLoader>& ModelsLaoder) override;
	void RemoveModelsLoader(const std::shared_ptr<IznModelsLoader>& ModelsLaoder) override;
	const std::shared_ptr<IznModelsLoader> GetLoaderForModel(const std::string& ModelFileName) override;

	std::shared_ptr<IModel> LoadModel(const std::string& ModelFileName, const std::shared_ptr<IznLoaderParams>& LoaderParams) override;
	std::shared_ptr<IModel> LoadModel(const std::shared_ptr<IFile>& ModelFile, const std::shared_ptr<IznLoaderParams>& LoaderParams) override;
	std::shared_ptr<IFile> SaveModel(const std::shared_ptr<IModel>& Model, const std::string& FileName) override;

private:
	std::vector<std::shared_ptr<IznModelsLoader>> m_ModelsLoaders;
	std::unordered_map<std::string, std::weak_ptr<IModel>> m_ModelsByName;
	IBaseManager& m_BaseManager;
};
