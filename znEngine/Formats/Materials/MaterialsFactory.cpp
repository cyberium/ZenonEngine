#include "stdafx.h"

// General
#include "MaterialsFactory.h"

// Additional
#include "Materials/MaterialModel.h"
#include "Materials/MaterialDebug.h"
#include "UIControls/Common/UICommonMaterial.h"

CMaterialsFactory::CMaterialsFactory(IBaseManager & BaseManager)
	: m_BaseManager(BaseManager)
{
}

CMaterialsFactory::~CMaterialsFactory()
{
}



//
// IMaterialsFactory
//
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
	else if (ObjectType == "MaterialDebug")
	{
		return MakeShared(MaterialDebug, m_BaseManager.GetApplication().GetRenderDevice());
	}
	else if (ObjectType == "MaterialUIControl")
	{
		return MakeShared(CUICommonMaterial, m_BaseManager.GetApplication().GetRenderDevice());
	}

	throw CException("Unknown material type '%s'.", ObjectType);
}

std::shared_ptr<IMaterial> CMaterialsFactory::AddMaterial(std::shared_ptr<IMaterial> Material)
{
	auto materialName = Material->GetName();
	if (materialName.empty())
		return nullptr;

	const auto& materialIt = m_Materials.find(materialName);
	if (materialIt != m_Materials.end())
		return nullptr; // TODO: Check equality

	m_Materials.insert(std::make_pair(materialName, Material));
	return nullptr;
}

void CMaterialsFactory::PrintMaterials() const
{
	for (const auto& material : m_Materials)
	{
		Log::Print("Material '%s'.", material.second->GetName().c_str());
	}
}
