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

	destCast->m_Models = m_Models;
}

void CModelsComponent3D::Load(const std::shared_ptr<IXMLReader>& Reader)
{
	CComponentBase::Load(Reader);

	auto modelsReader = Reader->GetChild("Models");
	if (modelsReader)
	{
		for (const auto& modelReader : modelsReader->GetChilds())
		{
			auto model = GetBaseManager().GetApplication().GetRenderDevice().GetObjectsFactory().CreateModel();
			if (auto loadable = std::dynamic_pointer_cast<IObjectLoadSave>(model))
				loadable->Load(modelReader);
			AddModel(model);
		}
	}
}

void CModelsComponent3D::Save(const std::shared_ptr<IXMLWriter>& Writer) const
{
	CComponentBase::Save(Writer);

	auto models = GetModels();
	if (false == models.empty())
	{
		CXMLManager xml;
		auto modelsWriter = xml.CreateWriter("Models");
		for (const auto& model : GetModels())
		{
			if (auto loadable = std::dynamic_pointer_cast<IObjectLoadSave>(model))
				loadable->Save(modelsWriter);
		}
		Writer->AddChild(modelsWriter);
	}
}



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

const ModelsList& CModelsComponent3D::GetModels() const
{
	return m_Models;
}



//
// ISceneNodeComponent
//
void CModelsComponent3D::Accept(IVisitor* visitor)
{
	const auto& models = GetModels();
	std::for_each(models.begin(), models.end(), [visitor](const auto& Model) {
		if (Model)
			Model->Accept(visitor);
	});
}
