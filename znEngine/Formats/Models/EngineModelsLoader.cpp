#include "stdafx.h"

// General
#include "EngineModelsLoader.h"

// Additional
#include "Scene/Components/ModelComponent/Animation.h"
#include "Scene/Components/ModelComponent/SkeletonBone.h"

namespace
{
	const char * cLoaderSupportedExtension = "znmdl";
}

CznEngineModelsLoader::CznEngineModelsLoader(IBaseManager& BaseManager)
	: m_BaseManager(BaseManager)
{
}

CznEngineModelsLoader::~CznEngineModelsLoader()
{
}



//
// IznModelsLoader
//
std::string CznEngineModelsLoader::GetName() const
{
	return "EngineModelsLoader";
}

bool CznEngineModelsLoader::IsSupportedFormat(const std::string & ModelFileName) const
{
	std::string lowerFileName = Utils::ToLower(ModelFileName);
	std::string extension = lowerFileName;
	if (ModelFileName.length() > strlen(cLoaderSupportedExtension))
		extension = extension.substr(extension.length() - strlen(cLoaderSupportedExtension));
	return extension == cLoaderSupportedExtension;
}

bool CznEngineModelsLoader::IsSupportedFormat(const std::shared_ptr<IFile>& ModelFile) const
{
	return ModelFile->Extension() == cLoaderSupportedExtension;
}



namespace
{
	struct SConnectionIndexed
	{
		size_t MaterialIndex;
		size_t GeometryIndex;
		SGeometryDrawArgs GeometryDrawArgs;
	};

	size_t FindOrAddMaterialIndex(std::vector<std::shared_ptr<IMaterial>>& Materials, std::shared_ptr<IMaterial> Material)
	{
		const auto& matIter = std::find(Materials.begin(), Materials.end(), Material);
		if (matIter != Materials.end())
			return std::distance(Materials.begin(), matIter);
		Materials.push_back(Material);
		return Materials.size() - 1;
	}

	size_t FindOrAddGeometryIndex(std::vector<std::shared_ptr<IGeometry>>& Geometries, std::shared_ptr<IGeometry> Geometry)
	{
		const auto& geomIter = std::find(Geometries.begin(), Geometries.end(), Geometry);
		if (geomIter != Geometries.end())
			return std::distance(Geometries.begin(), geomIter);
		Geometries.push_back(Geometry);
		return Geometries.size() - 1;
	}
}

std::shared_ptr<IModel> CznEngineModelsLoader::LoadModel(const std::string& ModelFileName, const IznLoaderParams* LoaderParams) const
{
	auto modelFile = m_BaseManager.GetManager<IFilesManager>()->Open(ModelFileName);
	if (modelFile == nullptr)
		throw CException("Model file '%s' not found.", ModelFileName.c_str());
	return LoadModel(modelFile, LoaderParams);
}

std::shared_ptr<IModel> CznEngineModelsLoader::LoadModel(const std::shared_ptr<IFile>& ModelFile, const IznLoaderParams* LoaderParams) const
{
	IRenderDevice& renderDevice = m_BaseManager.GetApplication().GetRenderDevice();
	
	ModelFile->seek(0);

	auto model = renderDevice.GetObjectsFactory().CreateModel();
	model->SetFileName(ModelFile->Path_Name());

	//
	// Common data
	BoundingBox bounds;
	bounds.Load(ModelFile);
	model->SetBounds(bounds);


	//
	// Load materials
	std::vector<std::shared_ptr<IMaterial>> materials;
	size_t materialsCount;
	ModelFile->read(&materialsCount);
	materials.reserve(materialsCount);
	for (size_t i = 0; i < materialsCount; i++)
	{
		auto material = m_BaseManager.GetManager<IMaterialsFactory>()->CreateMaterial("MaterialModel");
		if (auto materialAsLoadableFromFile = std::dynamic_pointer_cast<IObjectLoadSave>(material))
		{
			materialAsLoadableFromFile->Load(ModelFile);
		}
		else
			throw CException("Material '%s' is not supports 'IObjectLoadSave'.", material->GetName().c_str());

		materials.push_back(material);
	}


	//
	// Load geometries
	std::vector<std::shared_ptr<IGeometry>> geometries;
	size_t geometriesCount;
	ModelFile->read(&geometriesCount);
	geometries.reserve(geometriesCount);
	for (size_t i = 0; i < geometriesCount; i++)
	{
		auto geometry = renderDevice.GetObjectsFactory().CreateGeometry();
		if (auto geometryAsLoadableFromFile = std::dynamic_pointer_cast<IObjectLoadSave>(geometry))
		{
			geometryAsLoadableFromFile->Load(ModelFile);
		}
		else
			throw CException("Geometry '%s' is not supports 'IObjectLoadSave'.", geometry->GetName().c_str());

		geometries.push_back(geometry);
	}


	//
	// Load connections
	{
		size_t indexedConnectionsCount;
		ModelFile->read(&indexedConnectionsCount);
		for (size_t i = 0; i < indexedConnectionsCount; i++)
		{
			size_t materialIndex;
			ModelFile->read(&materialIndex);

			size_t geometryIndex;
			ModelFile->read(&geometryIndex);

			SGeometryDrawArgs drawArgs;
			ModelFile->read(&drawArgs);

			auto materialObject = materials[materialIndex];
			auto geometryObject = geometries[geometryIndex];

			model->AddConnection(materialObject, geometryObject, drawArgs);
		}
	}


	//
	// Load animations
	{
		std::vector<std::shared_ptr<IAnimation>> animations;
		size_t animationsCount;
		if (ModelFile->read(&animationsCount))
		{
			animations.reserve(animationsCount);
			for (size_t i = 0; i < animationsCount; i++)
			{
				std::shared_ptr<IAnimation> anim = MakeShared(CAnimation, ModelFile);
				model->AddAnimation(anim->GetName(), anim);
			}
		}
	}


	//
	// Load bones
	{
		std::vector<std::shared_ptr<IAnimation>> bones;
		size_t bonesCount;
		if (ModelFile->read(&bonesCount))
		{
			bones.reserve(bonesCount);
			for (size_t i = 0; i < bonesCount; i++)
			{
				std::shared_ptr<ISkeletonBone> bone = MakeShared(CSkeletonBone, ModelFile);
				model->AddBone(bone);
			}
		}
	}

	// Fix skeleton matrix
	glm::mat4 fixSkeletonMatrix;
	if (ModelFile->read(&fixSkeletonMatrix))
		model->SetFixSkeleton(fixSkeletonMatrix);

	return model;
}

std::shared_ptr<IFile> CznEngineModelsLoader::SaveModel(const std::shared_ptr<IModel>& Model, const std::string & FileName) const
{	
	std::vector<std::shared_ptr<IMaterial>> materials;
	std::vector<std::shared_ptr<IGeometry>> geometries;
	std::vector<SConnectionIndexed> indexedConnections;

	const auto& modelConnections = Model->GetConnections();
	for (const auto& it : modelConnections)
	{
		size_t materialIndex = FindOrAddMaterialIndex(materials, it.Material);
		size_t geometryIndex = FindOrAddGeometryIndex(geometries, it.Geometry);
		indexedConnections.push_back(SConnectionIndexed{ materialIndex, geometryIndex, it.GeometryDrawArgs });
	}

	auto file = m_BaseManager.GetManager<IFilesManager>()->Create(FileName);


	//
	// Common data
	BoundingBox modelBBox = Model->GetBounds();
	modelBBox.Save(file);


	//
	// Save materials
	size_t materialsCount = materials.size();
	file->write(&materialsCount);
	for (const auto& m : materials)
	{
		if (auto materialAsLoadableFromFile = std::dynamic_pointer_cast<IObjectLoadSave>(m))
		{
			materialAsLoadableFromFile->Save(file);
		}
		else
			throw CException("Material '%s' is not supports 'IObjectLoadSave'.", m->GetName().c_str());
	}


	//
	// Save geometries
	size_t geometriesCount = geometries.size();
	file->write(&geometriesCount);
	for (const auto& g : geometries)
	{
		if (auto geometryAsLoadableFromFile = std::dynamic_pointer_cast<IObjectLoadSave>(g))
		{
			geometryAsLoadableFromFile->Save(file);
		}
		else
			throw CException("Geometry '%s' is not supports 'IObjectLoadSave'.", g->GetName().c_str());
	}


	//
	// Save connections
	size_t connectionsCount = indexedConnections.size();
	file->write(&connectionsCount);
	for (const auto& c : indexedConnections)
	{
		file->write(&c.MaterialIndex);
		file->write(&c.GeometryIndex);
		file->write(&c.GeometryDrawArgs);
	}


	//
	// Save animations
	const auto& animations = Model->GetAnimations();
	size_t animationsCount = animations.size();
	file->write(&animationsCount);
	for (const auto& a : animations)
	{
		if (auto loadSave = std::dynamic_pointer_cast<IObjectLoadSave>(a.second))
			loadSave->Save(file);
	}

	
	//
	// Save bones
	const auto& bones = Model->GetBones();
	size_t bonesCount = bones.size();
	file->write(&bonesCount);
	for (const auto& b : bones)
	{
		if (auto loadSave = std::dynamic_pointer_cast<IObjectLoadSave>(b))
			loadSave->Save(file);
	}

	// Fix skeleton matrix
	file->write(&Model->GetFixSkeleton());

	return file;
}
