#include "stdafx.h"

// General
#include "ModelsLoader_znxmdl.h"

// Additional
#include "ModelsLoaderHelper.h"
#include "Scene/Components/Animations/Animation.h"
#include "Scene/Components/ModelComponent/Skeleton.h"

#include "Materials/MaterialModel.h"

namespace
{
	const char * cLoaderSupportedExtension = "znxmdl";
}

CModelsLoader_znxmdl::CModelsLoader_znxmdl(IBaseManager& BaseManager)
	: m_BaseManager(BaseManager)
{
}

CModelsLoader_znxmdl::~CModelsLoader_znxmdl()
{
}



//
// IznModelsLoader
//
std::string CModelsLoader_znxmdl::GetName() const
{
	return "ModelsLoader_znxmdl";
}

bool CModelsLoader_znxmdl::IsSupportedFormat(const std::string & ModelFileName) const
{
	std::string lowerFileName = Utils::ToLower(ModelFileName);
	std::string extension = lowerFileName;
	if (ModelFileName.length() > strlen(cLoaderSupportedExtension))
		extension = extension.substr(extension.length() - strlen(cLoaderSupportedExtension));
	return extension == cLoaderSupportedExtension;
}

bool CModelsLoader_znxmdl::IsSupportedFormat(const std::shared_ptr<IFile>& ModelFile) const
{
	return ModelFile->Extension() == cLoaderSupportedExtension;
}

std::shared_ptr<IModel> CModelsLoader_znxmdl::LoadModel(const std::string & ModelFileName, const IznLoaderParams * LoaderParams) const
{
	auto modelFile = m_BaseManager.GetManager<IFilesManager>()->Open(ModelFileName);
	if (modelFile == nullptr)
		throw CException("Model file '%s' not found.", ModelFileName.c_str());
	return LoadModel(modelFile, LoaderParams);
}

std::shared_ptr<IModel> CModelsLoader_znxmdl::LoadModel(const std::shared_ptr<IFile>& ModelFile, const IznLoaderParams * LoaderParams) const
{
	IRenderDevice& renderDevice = m_BaseManager.GetApplication().GetRenderDevice();

	CXMLManager xml(m_BaseManager);
	auto rootReader = xml.CreateReaderFromFile(ModelFile);

	auto modelReader = rootReader->GetChild("Model");

	auto model = renderDevice.GetObjectsFactory().CreateModel();
	auto modelInternal = std::dynamic_pointer_cast<IModelInternal>(model);
	modelInternal->SetFileName(ModelFile->Path_Name());

	//
	// Common data
	BoundingBox bounds;
	bounds.Load(modelReader);
	modelInternal->SetBounds(bounds);


	//
	// Load materials
	std::vector<std::shared_ptr<IMaterial>> materials;
	if (modelReader->IsChildExists("Materials"))
	{
		for (const auto& materialsReaderXML : modelReader->GetChild("Materials")->GetChilds())
		{
			auto material = m_BaseManager.GetManager<IMaterialsFactory>()->CreateMaterial("MaterialModel");
			std::dynamic_pointer_cast<IObjectLoadSave>(material)->Load(materialsReaderXML);
			m_BaseManager.GetManager<IMaterialsFactory>()->AddMaterial(material);
			materials.push_back(material);

			if (material->GetName() == "water")
				std::dynamic_pointer_cast<MaterialModel>(material)->SetEnviorementMappingEnable(true);

			if (material->GetName() == "crystal")
				std::dynamic_pointer_cast<MaterialModel>(material)->SetEmissiveColor(std::dynamic_pointer_cast<MaterialModel>(material)->GetDiffuseColor());
		}
	}


	//
	// Load geometries
	std::vector<std::shared_ptr<IGeometry>> geometries;
	if (modelReader->IsChildExists("Geometries"))
	{
		for (const auto& geometryReaderXML : modelReader->GetChild("Geometries")->GetChilds())
		{
			auto geometry = renderDevice.GetObjectsFactory().CreateGeometry();
			std::dynamic_pointer_cast<IObjectLoadSave>(geometry)->Load(geometryReaderXML);
			geometries.push_back(geometry);
		}
	}


	//
	// Load connections
	if (modelReader->IsChildExists("Connections"))
	{
		for (const auto& connectionReaderXML : modelReader->GetChild("Connections")->GetChilds())
		{
			size_t materialIndex = connectionReaderXML->GetUIntAttribute("Material");
			size_t geometryIndex = connectionReaderXML->GetUIntAttribute("Geometry");

			SGeometryDrawArgs drawArgs;
			ModelsLoaderHelper::LoadGeometryDrawArgs(drawArgs, connectionReaderXML);

			auto materialObject = materials[materialIndex];
			auto geometryObject = geometries[geometryIndex];

			model->AddConnection(materialObject, geometryObject, drawArgs);
		}
	}


	//
	// Animations
	if (modelReader->IsChildExists("Animations"))
	{
		for (const auto& animationReaderXML : modelReader->GetChild("Animations")->GetChilds())
		{
			std::shared_ptr<IAnimation> anim = MakeShared(CAnimation, animationReaderXML);
			model->AddAnimation(anim->GetName(), anim);
		}
	}


	//
	// Skeleton
	if (modelReader->IsChildExists("Skeleton"))
	{
		auto skeleton = MakeShared(CSkeleton);
		std::dynamic_pointer_cast<IObjectLoadSave>(skeleton)->Load(modelReader->GetChild("Skeleton"));
		modelInternal->SetSkeleton(skeleton);
	}


	return model;
}

std::shared_ptr<IFile> CModelsLoader_znxmdl::SaveModel(const std::shared_ptr<IModel>& Model, const std::string & FileName) const
{
	std::vector<std::shared_ptr<IMaterial>> materials;
	std::vector<std::shared_ptr<IGeometry>> geometries;
	std::vector<ModelsLoaderHelper::SConnectionIndexed> indexedConnections;

	const auto& modelConnections = Model->GetConnections();
	for (const auto& it : modelConnections)
	{
		size_t materialIndex = ModelsLoaderHelper::FindOrAddMaterialIndex(materials, it.Material);
		size_t geometryIndex = ModelsLoaderHelper::FindOrAddGeometryIndex(geometries, it.Geometry);
		indexedConnections.push_back(ModelsLoaderHelper::SConnectionIndexed{ materialIndex, geometryIndex, it.GeometryDrawArgs });
	}

	CXMLManager xml(m_BaseManager);
	auto xmlDocument = xml.CreateDocument();
	auto modelWriter = xmlDocument->CreateChild("Model");

	//
	// Common data
	BoundingBox modelBBox = Model->GetBounds();
	modelBBox.Save(modelWriter);


	//
	// Save materials
	{
		auto materialsWriterXML = modelWriter->CreateChild("Materials");
		for (const auto& m : materials)
		{
			auto materialWriterXML = materialsWriterXML->CreateChild("MaterialModel");
			std::dynamic_pointer_cast<IObjectLoadSave>(m)->Save(materialWriterXML);
		}
	}


	//
	// Save geometries
	{
		auto geometriesWriterXML = modelWriter->CreateChild("Geometries");
		for (const auto& g : geometries)
		{
			auto geometryWriterXML = geometriesWriterXML->CreateChild("Geometry");
			std::dynamic_pointer_cast<IObjectLoadSave>(g)->Save(geometryWriterXML);
		}
	}


	//
	// Save connections
	{
		auto connectionsWriterXML = modelWriter->CreateChild("Connections");
		//_ASSERT(false == indexedConnections.empty());
		for (const auto& c : indexedConnections)
		{
			auto connectionWriterXML = connectionsWriterXML->CreateChild("Connection");

			connectionWriterXML->SetUIntAttribute(c.MaterialIndex, "Material");
			connectionWriterXML->SetUIntAttribute(c.GeometryIndex, "Geometry");
			ModelsLoaderHelper::SaveGeometryDrawArgs(c.GeometryDrawArgs, connectionWriterXML);
		}
	}


	//
	// Save animations
	const auto& animations = Model->GetAnimations();
	if (false == animations.empty())
	{
		auto animationsWriterXML = modelWriter->CreateChild("Animations");
		for (const auto& a : animations)
		{
			auto animationWriterXML = animationsWriterXML->CreateChild("Animation");
			std::dynamic_pointer_cast<IObjectLoadSave>(a.second)->Save(animationWriterXML);
		}
	}


	//
	// Save bones
	if (auto skeleton = Model->GetSkeleton())
	{
		auto bonesWriterXML = modelWriter->CreateChild("Skeleton");
		std::dynamic_pointer_cast<IObjectLoadSave>(skeleton)->Save(bonesWriterXML);
	}


	auto file = m_BaseManager.GetManager<IFilesManager>()->Create(FileName);
	xml.SaveWriterToFile(xmlDocument, file);
	return file;
}
