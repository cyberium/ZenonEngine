#pragma once

class CMaterialsFactory
	: public IMaterialsFactory
{
public:
	CMaterialsFactory(IBaseManager& BaseManager);
	virtual ~CMaterialsFactory();

	// IMaterialsFactory
	void AddMaterialsCreator(std::shared_ptr<IMaterialsCreator> Creator) override;
	void RemoveMaterialsCreator(std::shared_ptr<IMaterialsCreator> Creator) override;
	
	std::shared_ptr<IMaterial> CreateMaterial(std::string ClassName) const override;
	std::shared_ptr<IMaterial> AddMaterial(std::shared_ptr<IMaterial> Material) override;

	void PrintMaterials() const override;

private:
	std::map<std::string, std::shared_ptr<IMaterial>> m_Materials;

private:
	IBaseManager& m_BaseManager;
};