#pragma once

class CznModelsManager
	: public IznModelsManager
{
public:
	CznModelsManager(IBaseManager& BaseManager);
	virtual ~CznModelsManager();

	// IznModelsManager
	void AddModelsLoader(const std::shared_ptr<IznModelsLoader>& ModelsLaoder) override;
	void RemoveModelsLoader(const std::shared_ptr<IznModelsLoader>& ModelsLaoder) override;
	std::shared_ptr<IModel> LoadModel(const std::string& ModelFileName, const std::string& TexturesPath) override;
	std::shared_ptr<IFile> SaveModel(const std::shared_ptr<IModel>& Model, const std::string& FileName) override;

private:
	std::unordered_map<std::string, std::weak_ptr<IModel>> m_ModelsByName;
	std::vector<std::shared_ptr<IznModelsLoader>> m_ModelsLoaders;
	IBaseManager& m_BaseManager;
};
