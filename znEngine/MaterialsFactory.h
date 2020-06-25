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

private:
	IBaseManager& m_BaseManager;
};