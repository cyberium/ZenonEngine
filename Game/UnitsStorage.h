#pragma once

class CUnitsStorage
{
public:
	CUnitsStorage(const IBaseManager& BaseManager);


	void RaceStorage(const std::string& FileName);
	std::shared_ptr<IModel> CreateUnitModel(const std::shared_ptr<IXMLReader>& PrefabReader, const std::string& RootFolder, const std::string& TexturesFolder, const std::vector<std::pair<std::string, std::string>>& TexturesOverride);
	
	
	std::shared_ptr<IModel> GetModel(size_t Index) const;
	const std::vector<std::shared_ptr<IModel>>& GetModels() const;

private:
	std::vector<std::shared_ptr<IModel>> m_Models;
	const IBaseManager& m_BaseManager;
};