#include "stdafx.h"

// General
#include "UnitsStorage.h"

// Additional
#include "../znPluginFBXModels/FBXInterfaces.h"

CUnitsStorage::CUnitsStorage(const IBaseManager& BaseManager)
	: m_BaseManager(BaseManager)
{

	RaceStorage("humans.xml");
	RaceStorage("undeads.xml");
}

void CUnitsStorage::RaceStorage(const std::string& FileName)
{
	CXMLManager xml(m_BaseManager);
	auto rootReader = xml.CreateReaderFromFile(FileName);

	std::string rootFolder = rootReader->GetChild("RootFolder")->GetValue();
	std::string texturesFolder = rootReader->GetChild("TexturesFolder")->GetValue();

	std::vector<std::pair<std::string, std::string>> materials;
	for (const auto& texture : rootReader->GetChild("TexturesOverride")->GetChilds())
		materials.push_back(std::make_pair(texture->GetStrAttribute("Name"), texture->GetValue()));

	for (const auto& child : rootReader->GetChilds())
	{
		if (child->GetName() != "Prefab")
			continue;

		auto model = CreateUnitModel(child, rootFolder, texturesFolder, materials);
		m_Models.push_back(model);
	}
}

std::shared_ptr<IModel> CUnitsStorage::CreateUnitModel(const std::shared_ptr<IXMLReader>& PrefabReader, const std::string& RootFolder, const std::string& TexturesFolder, const std::vector<std::pair<std::string, std::string>>& TexturesOverride)
{
	std::string destinationFilename = PrefabReader->GetStrAttribute("Filename");

	auto filesManager = m_BaseManager.GetManager<IFilesManager>();
	//if (filesManager->IsFileExists(destinationFilename))
	//	return m_BaseManager.GetManager<IznModelsFactory>()->LoadModel(destinationFilename);

	CznFBXLoaderParams fbxLoaderParams;
	fbxLoaderParams.TexturesPathRoot = TexturesFolder;
	for (const auto& t : TexturesOverride)
		fbxLoaderParams.OverrideTextureByMaterial[t.first] = t.second;

	auto fbxModelsLoader = m_BaseManager.GetManager<IznModelsFactory>()->GetLoaderForModel("fbx");
	_ASSERT(fbxModelsLoader != nullptr);
	auto fbxSceneLoader = std::dynamic_pointer_cast<IFBXSceneLoader>(fbxModelsLoader);
	_ASSERT(fbxSceneLoader != nullptr);

	std::string modelFilename = PrefabReader->GetChild("ModelFilename")->GetValue();

	// Original skeleton
	std::shared_ptr<IFile> modelFile = filesManager->Open(RootFolder + modelFilename);
	auto originalSkeletonModel = fbxSceneLoader->LoadScene(modelFile, &fbxLoaderParams)->MergeModels();

	// Fix materials
	for (const auto& connection : originalSkeletonModel->GetConnections())
	{
		std::dynamic_pointer_cast<MaterialModel>(connection.Material)->SetDiffuseColor(ColorRGB(1.0f));
		std::dynamic_pointer_cast<MaterialModel>(connection.Material)->SetDiffuseFactor(1.0f);
	}

	if (PrefabReader->IsChildExists("Animations"))
	{
		for (const auto& animationReader : PrefabReader->GetChild("Animations")->GetChilds())
		{
			std::string animationFilename = RootFolder + animationReader->GetValue();
			std::shared_ptr<IFile> animationFile = filesManager->Open(animationFilename);
			auto animatedSkeletonModel = fbxSceneLoader->LoadScene(animationFile, &fbxLoaderParams)->MergeModels();
			for (const auto& anim : animatedSkeletonModel->GetAnimations())
				originalSkeletonModel->AddAnimation(animationReader->GetStrAttribute("Name"), anim.second);
		}
	}

	std::shared_ptr<IFile> modelResult = nullptr;

	{
		auto znMdlXMLFile = m_BaseManager.GetManager<IznModelsFactory>()->SaveModel(originalSkeletonModel, destinationFilename);
		znMdlXMLFile->Save();

		modelResult = znMdlXMLFile;
	}

	try
	{
		return m_BaseManager.GetManager<IznModelsFactory>()->LoadModel(modelResult);
	}
	catch (const CException& e)
	{
		Log::Error("Error while loading converted model.");
		Log::Error("--->%s", e.MessageCStr());
		throw;
	}

	return nullptr;
}

std::shared_ptr<IModel> CUnitsStorage::GetModel(size_t Index) const
{
	return m_Models.at(Index);
}

const std::vector<std::shared_ptr<IModel>>& CUnitsStorage::GetModels() const
{
	return m_Models;
}
