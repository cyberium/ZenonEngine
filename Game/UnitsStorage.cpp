#include "stdafx.h"

// General
#include "UnitsStorage.h"

// Additional
#include "../znPluginFBXModels/FBXInterfaces.h"

CUnitsStorage::CUnitsStorage(const IBaseManager& BaseManager)
	: m_BaseManager(BaseManager)
{

	CXMLManager xml(m_BaseManager);
	auto rootReader = xml.CreateReaderFromFile("prefabs.xml");

	for (const auto& child : rootReader->GetChilds())
	{
		auto model = CreateUnitModel(child);
		m_Models.push_back(model);
	}
}

std::shared_ptr<IModel> CUnitsStorage::CreateUnitModel(const std::shared_ptr<IXMLReader>& PrefabReader)
{
	std::string modelFilename = PrefabReader->GetChild("ModelFilename")->GetValue();
	std::string destinationFilename = PrefabReader->GetChild("DestinationFilename")->GetValue();

	auto filesManager = m_BaseManager.GetManager<IFilesManager>();

	//if (filesManager->IsFileExists(destinationFilename))
	//	return m_BaseManager.GetManager<IznModelsFactory>()->LoadModel(destinationFilename);

	CznFBXLoaderParams fbxLoaderParams;
	fbxLoaderParams.TexturesPathRoot = PrefabReader->GetChild("TexturesPathRoot")->GetValue();
	fbxLoaderParams.OverrideTextureByMaterial["WK_Standard_Units"] = "WK_Standard_Units.png";
	fbxLoaderParams.OverrideTextureByMaterial["WK_Horse"] = "WK_Horse_A.png";

	auto fbxModelsLoader = m_BaseManager.GetManager<IznModelsFactory>()->GetLoaderForModel("fbx");
	_ASSERT(fbxModelsLoader != nullptr);
	auto fbxSceneLoader = std::dynamic_pointer_cast<IFBXSceneLoader>(fbxModelsLoader);
	_ASSERT(fbxSceneLoader != nullptr);

	// Original skeleton
	std::shared_ptr<IFile> modelFile = filesManager->Open(modelFilename);
	auto originalSkeletonModel = fbxSceneLoader->LoadScene(modelFile, &fbxLoaderParams)->MergeModels();

	// Fix materials
	for (const auto& connection : originalSkeletonModel->GetConnections())
	{
		std::dynamic_pointer_cast<MaterialModel>(connection.Material)->SetDiffuseColor(glm::vec3(1.0f));
		std::dynamic_pointer_cast<MaterialModel>(connection.Material)->SetDiffuseFactor(1.0f);
	}

	if (PrefabReader->IsChildExists("Animations"))
	{
		for (const auto& animationReader : PrefabReader->GetChild("Animations")->GetChilds())
		{
			std::shared_ptr<IFile> animationFile = filesManager->Open(animationReader->GetValue());
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
