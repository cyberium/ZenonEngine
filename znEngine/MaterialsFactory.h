#pragma once

class CMaterialsFactory
	: public IMaterialsFactory
{
public:
	CMaterialsFactory(IBaseManager& BaseManager);
	virtual ~CMaterialsFactory();

	// IMaterialsFactory
	void AddMaterialsCreator(std::shared_ptr<IMaterialsCreator> Creator) ;
	void RemoveMaterialsCreator(std::shared_ptr<IMaterialsCreator> Creator);
	std::shared_ptr<IMaterial> CreateMaterial(MaterialType MaterialType) const;

private:
	IBaseManager& m_BaseManager;
};