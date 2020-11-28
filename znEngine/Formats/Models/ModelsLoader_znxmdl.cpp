#include "stdafx.h"

// General
#include "ModelsLoader_znxmdl.h"

// Additional
#include "Scene/Components/ModelComponent/Animation.h"
#include "Scene/Components/ModelComponent/SkeletonBone.h"
#include "ModelsLoaderHelper.h"

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
	model->SetFileName(ModelFile->Path_Name());

	//
	// Common data
	BoundingBox bounds;
	bounds.Load(modelReader);
	model->SetBounds(bounds);


	//
	// Load materials
	std::vector<std::shared_ptr<IMaterial>> materials;
	if (modelReader->IsChildExists("Materials"))
	{
		for (const auto& materialsReaderXML : modelReader->GetChild("Materials")->GetChilds())
		{
			auto material = m_BaseManager.GetManager<IMaterialsFactory>()->CreateMaterial("MaterialModel");
			std::dynamic_pointer_cast<IObjectLoadSave>(material)->Load(materialsReaderXML);
			materials.push_back(material);
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
	// Load skeleton animations
	if (modelReader->IsChildExists("SkeletonAnimations"))
	{
		for (const auto& skeletonAnimationReaderXML : modelReader->GetChild("SkeletonAnimations")->GetChilds())
		{
			SSkeletonAnimation skeletonAnimation;
			skeletonAnimation.RootBoneLocalTransform = Utils::StringToMatrix(skeletonAnimationReaderXML->GetValue());
			std::dynamic_pointer_cast<IModelInternal>(model)->AddSkeletonAnimationInternal(skeletonAnimation);
		}
	}


	//
	// Load animations
	if (modelReader->IsChildExists("Animations"))
	{
		for (const auto& animationReaderXML : modelReader->GetChild("Animations")->GetChilds())
		{
			std::shared_ptr<IAnimation> anim = MakeShared(CAnimation, animationReaderXML);
			model->AddAnimation(anim->GetName(), anim);
		}
	}


	//
	// Load bones
	if (modelReader->IsChildExists("Bones"))
	{
		for (const auto& boneReaderXML : modelReader->GetChild("Bones")->GetChilds())
		{
			std::shared_ptr<ISkeletonBone> bone = MakeShared(CSkeletonBone, boneReaderXML);
			model->AddBone(bone);
		}
	}

	model->SetFixSkeleton(Utils::StringToMatrix(modelReader->GetStrAttribute("FixSkeletonMatrix")));

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
	auto rootWriter = xml.CreateWriter();
	auto modelWriter = rootWriter->CreateChild("Model");

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
		_ASSERT(false == indexedConnections.empty());
		for (const auto& c : indexedConnections)
		{
			auto connectionWriterXML = connectionsWriterXML->CreateChild("Connection");

			connectionWriterXML->SetUIntAttribute(c.MaterialIndex, "Material");
			connectionWriterXML->SetUIntAttribute(c.GeometryIndex, "Geometry");
			ModelsLoaderHelper::SaveGeometryDrawArgs(c.GeometryDrawArgs, connectionWriterXML);
		}
	}


	//
	// Save skeleton animations
	const auto& skeletonAnimations = Model->GetSkeletonAnimations();
	if (false == skeletonAnimations.empty())
	{
		auto skeletonAnimationsWriterXML = modelWriter->CreateChild("SkeletonAnimations");
		for (const auto& sa : skeletonAnimations)
		{
			auto skeletonAnimationWriterXML = skeletonAnimationsWriterXML->CreateChild("SkeletonAnimation");
			skeletonAnimationWriterXML->SetValue(Utils::MatrixToString(sa.RootBoneLocalTransform));
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
	const auto& bones = Model->GetBones();
	if (false == bones.empty())
	{
		auto bonesWriterXML = modelWriter->CreateChild("Bones");
		for (const auto& b : bones)
		{
			auto boneWriterXML = bonesWriterXML->CreateChild("Bone");
			std::dynamic_pointer_cast<IObjectLoadSave>(b)->Save(boneWriterXML);
		}
	}

	// Fix skeleton matrix
	modelWriter->SetStrAttribute(Utils::MatrixToString(Model->GetFixSkeleton()), "FixSkeletonMatrix");

	auto file = m_BaseManager.GetManager<IFilesManager>()->Create(FileName);
	xml.SaveWriterToFile(rootWriter, file);
	return file;
}
