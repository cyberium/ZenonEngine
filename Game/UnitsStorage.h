#pragma once

class CUnitsStorage
{
public:
	CUnitsStorage(const IBaseManager& BaseManager);

	std::shared_ptr<IModel> CreateUnitModel(const std::shared_ptr<IXMLReader>& PrefabReader);
	std::shared_ptr<IModel> GetModel(size_t Index) const;

private:
	std::vector<std::shared_ptr<IModel>> m_Models;
	const IBaseManager& m_BaseManager;
};