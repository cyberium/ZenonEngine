#include "stdafx.h"

// General
#include "ModelsComponent3D.h"

CModelsComponent3D::CModelsComponent3D(const ISceneNode3D& OwnerNode)
    : CComponentBase(OwnerNode)
{
	GetProperties()->SetName("ModelsComponent");
	GetProperties()->SetDescription("Contains data about visual model");
}

CModelsComponent3D::~CModelsComponent3D()
{}

void CModelsComponent3D::Copy(std::shared_ptr<ISceneNodeComponent> Destination) const
{
	auto destCast = std::static_pointer_cast<CModelsComponent3D>(Destination);

	destCast->m_Model = m_Model;
}

void CModelsComponent3D::Load(const std::shared_ptr<IXMLReader>& Reader)
{
	CComponentBase::Load(Reader);

	auto model = GetBaseManager().GetApplication().GetRenderDevice().GetObjectsFactory().CreateModel();
	if (auto loadable = std::dynamic_pointer_cast<IObjectLoadSave>(model))
		loadable->Load(Reader);
	SetModel(model);
}

void CModelsComponent3D::Save(const std::shared_ptr<IXMLWriter>& Writer) const
{
	CComponentBase::Save(Writer);

	auto model = GetModel();
	if (model)
	{
		if (auto loadable = std::dynamic_pointer_cast<IObjectLoadSave>(model))
			loadable->Save(Writer);
	}
}



//
// IModelsComponent3D
//
void CModelsComponent3D::SetModel(const std::shared_ptr<IModel>& Model)
{
	_ASSERT(m_Model == nullptr);
	m_Model = Model;
}

void CModelsComponent3D::ResetModel()
{
	m_Model.reset();
}

std::shared_ptr<IModel> CModelsComponent3D::GetModel() const
{
	return m_Model;
}



//
// ISceneNodeComponent
//
void CModelsComponent3D::Accept(IVisitor* visitor)
{
	if (GetModel())
		GetModel()->Accept(visitor);
}
