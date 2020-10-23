#include "stdafx.h"

// General
#include "ModelsComponent3D.h"

CModelsComponent3D::CModelsComponent3D(const ISceneNode3D& OwnerNode)
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
	auto destCast = std::static_pointer_cast<CModelsComponent3D>(Destination);
	destCast->m_Model = m_Model;
	destCast->m_IsCastShadows = m_IsCastShadows;
}

void CModelsComponent3D::Load(const std::shared_ptr<IXMLReader>& Reader)
{
	CComponentBase::Load(Reader);

	auto fileNameReader = Reader->GetChild("FileName");
	if (fileNameReader == nullptr)
	{
		Log::Error("ModelsComponent3D: Reader '%s' don't contains 'FileName' child.", Reader->GetName().c_str());
		return;
	}

	std::string fileName = fileNameReader->GetValue();
	if (false == GetBaseManager().GetManager<IFilesManager>()->IsFileExists(fileName))
	{
		Log::Error("ModelsComponent3D: File '%s' don't exists.", fileName.c_str());
		return;
	}

	try
	{
		auto model = GetBaseManager().GetManager<IznModelsManager>()->LoadModel(fileName);
		SetModel(model);
	}
	catch (const CException& e)
	{
		Log::Error("ModelsComponent3D: Error while loading model '%s'. Message: '%s'", fileName.c_str(), e.MessageCStr());
	}
}

void CModelsComponent3D::Save(const std::shared_ptr<IXMLWriter>& Writer) const
{
	CComponentBase::Save(Writer);

	auto model = GetModel();
	if (model == nullptr)
	{
		Log::Error("ModelsComponent3D: No model presented.");
		return;
	}

	CXMLManager xml;
	auto fileNameWriter = xml.CreateWriter("FileName");

	auto fileName = model->GetFileName();
	if (fileName.empty())
	{
		Log::Error("ModelsComponent3D: FileName is empty.");
		return;
	}

	fileNameWriter->SetValue(model->GetFileName());
	Writer->AddChild(fileNameWriter);
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
