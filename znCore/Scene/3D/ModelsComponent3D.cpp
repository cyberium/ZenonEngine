#include "stdafx.h"

// General
#include "ModelsComponent3D.h"

CModelsComponent3D::CModelsComponent3D(const ISceneNode3D& OwnerNode)
    : CComponentBase(OwnerNode)
{}

CModelsComponent3D::~CModelsComponent3D()
{}



//
// IModelsComponent3D
//
void CModelsComponent3D::AddModel(const std::shared_ptr<IModel>& Model)
{
	const auto& iter = std::find(m_Models.begin(), m_Models.end(), Model);
	if (iter == m_Models.end())
		m_Models.push_back(Model);
}

void CModelsComponent3D::RemoveModel(const std::shared_ptr<IModel>& Model)
{
	const auto& iter = std::find(m_Models.begin(), m_Models.end(), Model);
	if (iter != m_Models.end())
	    m_Models.erase(iter);
}

const ModelsList& CModelsComponent3D::GetModels()
{
	return m_Models;
}



//
// ISceneNodeComponent
//
void CModelsComponent3D::Accept(IVisitor* visitor)
{
	const auto& models = GetModels();
	std::for_each(models.begin(), models.end(), [&visitor](const auto& Model) {
		if (Model)
			Model->Accept(visitor);
	});
}
