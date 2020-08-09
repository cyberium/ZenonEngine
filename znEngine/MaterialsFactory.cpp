#include "stdafx.h"

// General
#include "MaterialsFactory.h"

// Additional
#include "Materials/MaterialModel.h"

CMaterialsFactory::CMaterialsFactory(IBaseManager & BaseManager)
	: m_BaseManager(BaseManager)
{
}

CMaterialsFactory::~CMaterialsFactory()
{
}

void CMaterialsFactory::AddMaterialsCreator(std::shared_ptr<IMaterialsCreator> Creator)
{
}

void CMaterialsFactory::RemoveMaterialsCreator(std::shared_ptr<IMaterialsCreator> Creator)
{
}

std::shared_ptr<IMaterial> CMaterialsFactory::CreateMaterial(std::string ObjectType) const
{
	if (ObjectType == "MaterialModel")
	{
		return MakeShared(MaterialModel, m_BaseManager);
	}

	return std::shared_ptr<IMaterial>();
}
