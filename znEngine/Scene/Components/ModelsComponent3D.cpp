#include "stdafx.h"

// General
#include "ModelsComponent3D.h"

CModelsComponent3D::CModelsComponent3D(const ISceneNode& OwnerNode)
    : CComponentBase(OwnerNode)
	, m_IsCastShadows(true)
{
	GetProperties()->SetName("ModelsComponent");
	GetProperties()->SetDescription("Contains data about visual model");
}

CModelsComponent3D::~CModelsComponent3D()
{}

void CModelsComponent3D::Copy(std::shared_ptr<ISceneNodeComponent> Destination) const
{
	auto destCast = std::dynamic_pointer_cast<IModelsComponent3D>(Destination);

	destCast->SetModel(m_Model);
	destCast->SetCastShadows(m_IsCastShadows);
}

void CModelsComponent3D::Load(const std::shared_ptr<IXMLReader>& Reader)
{
	CComponentBase::Load(Reader);

	auto fileNameReader = Reader->GetChild("FileName");
	if (fileNameReader == nullptr)
		throw CException("Model XML doesn't contains 'FileName' section. Node '%s'.", GetOwnerNode().GetName().c_str());

	std::string fileName = fileNameReader->GetValue();
	if (false == GetBaseManager().GetManager<IFilesManager>()->IsFileExists(fileName))
		throw CException("Model file '%s' doesn't exists. Node '%s'.", fileName.c_str(), GetOwnerNode().GetName().c_str());

	try
	{
		auto model = GetBaseManager().GetManager<IznModelsFactory>()->LoadModel(fileName);
		SetModel(model);
	}
	catch (const CException& e)
	{
		throw CException("Error occurs while loading '%s' model. Node '%s'. Error '%s'.", fileName.c_str(), GetOwnerNode().GetName().c_str(), e.MessageCStr());
	}
}

void CModelsComponent3D::Save(const std::shared_ptr<IXMLWriter>& Writer) const
{
	CComponentBase::Save(Writer);

	auto model = GetModel();
	if (model == nullptr)
		throw CException("Unable to save nullptr model. Node '%s'", GetOwnerNode().GetName().c_str());

	CXMLManager xml(GetBaseManager());
	auto fileNameWriter = xml.CreateWriter("FileName");

	auto fileName = model->GetFileName();
	if (fileName.empty())
		throw CException("Unable to save model with empty filename. Node '%s'", GetOwnerNode().GetName().c_str());

	if (false == GetBaseManager().GetManager<IFilesManager>()->IsFileExists(fileName))
		throw CException("Model file '%s' doesn't exists. Node '%s'.", fileName.c_str(), GetOwnerNode().GetName().c_str());

	fileNameWriter->SetValue(model->GetFileName());
	Writer->AddChild(fileNameWriter);
}



//
// IModelsComponent3D
//
void CModelsComponent3D::SetModel(const std::shared_ptr<IModel>& Model)
{
	if (Model == nullptr)
		throw CException("Nullptr model can't be assigned to node '%s'.", GetOwnerNode().GetName().c_str());

	if (m_Model != nullptr)
		Log::Warn("Node '%s' already has model '%s'. Do you really want to replace it with '%s'?", m_Model->GetFileName().c_str(), Model->GetFileName().c_str());

	m_Model = Model;
	const auto& modelBounds = GetModel()->GetBounds();
	if (modelBounds.IsInfinite())
		throw CException("Model '%s' with inifinity bounds can't be assigned to node '%s'.", Model->GetFileName().c_str(), GetOwnerNode().GetName().c_str());

	GetComponent<IColliderComponent3D>()->ExtendBounds(modelBounds);
}

void CModelsComponent3D::ResetModel()
{
	m_Model.reset();
}

std::shared_ptr<IModel> CModelsComponent3D::GetModel() const
{
	return m_Model;
}

void CModelsComponent3D::SetCastShadows(bool Value)
{
	m_IsCastShadows = Value;
}

bool CModelsComponent3D::IsCastShadows() const
{
	return m_IsCastShadows;
}



//
// ISceneNodeComponent
//
void CModelsComponent3D::Accept(IVisitor* visitor)
{
	if (GetModel())
		GetModel()->Accept(visitor);
}
