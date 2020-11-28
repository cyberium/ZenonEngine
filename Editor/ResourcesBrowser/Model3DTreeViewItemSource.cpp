#include "stdafx.h"

// General
#include "Model3DTreeViewItemSource.h"

// Additional
#include "../znPluginFBXModels/FBXInterfaces.h"

CznModel3DTreeViewItemSource::CznModel3DTreeViewItemSource(IBaseManager& BaseManager, std::string FileName)
	: m_BaseManager(BaseManager)
	, m_FileName(FileName)
{
}

CznModel3DTreeViewItemSource::~CznModel3DTreeViewItemSource()
{
}



//
// IznTreeViewItemSource
//
ETreeViewItemType CznModel3DTreeViewItemSource::GetType() const
{
	return ETreeViewItemType::Model;
}

std::string CznModel3DTreeViewItemSource::GetText() const
{
	if (GetState() != ILoadable::ELoadableState::Loaded)
		return m_FileName;
	if (m_Model == nullptr)
		return "Error. " + m_FileName;
	return m_Model->GetName();
}

std::shared_ptr<IObject> CznModel3DTreeViewItemSource::GetObject_() const
{
	if (GetState() != ILoadable::ELoadableState::Loaded)
		return nullptr;
	return m_Model;
}



//
// ILoadable
//
bool CznModel3DTreeViewItemSource::Load()
{
	auto filesManager = m_BaseManager.GetManager<IFilesManager>();
	auto gameDataStorage = filesManager->GetStorage(EFilesStorageType::USERDATA);
	auto gameDataStorageEx = std::dynamic_pointer_cast<IznFilesStorageExtended>(gameDataStorage);
	_ASSERT(gameDataStorageEx != nullptr);

	try
	{
		if (m_FileName.empty())
			throw CException("Filename is emprty.");

		auto fileNameStruct = Utils::SplitFilename(m_FileName);
		if (fileNameStruct.Extension == "fbx")
		{
			std::string convertedModelNameXML = fileNameStruct.Path + fileNameStruct.NameWithoutExtension + ".xml";
			std::string convertedModelName = fileNameStruct.Path + fileNameStruct.NameWithoutExtension + ".znmdl";

			if (filesManager->IsFileExists(convertedModelName))
			{
				auto znmdlModelsLoader = GetBaseManager().GetManager<IznModelsFactory>()->GetLoaderForModel("znmdl");
				m_Model = znmdlModelsLoader->LoadModelXML(convertedModelNameXML);
				m_Model->SetName(fileNameStruct.NameWithoutExtension);
				Log::Info("Model '%s' loaded.", m_FileName.c_str());
				return true;
			}
			else if (filesManager->IsFileExists(convertedModelName)) // .znmdl file exists. Load it.
			{
				m_Model = GetBaseManager().GetManager<IznModelsFactory>()->LoadModel(convertedModelName);
				m_Model->SetName(fileNameStruct.NameWithoutExtension);
				Log::Info("Model '%s' loaded.", m_FileName.c_str());
				return true;
			}
			else if (filesManager->IsFileExists(m_FileName)) // *.fbx file exists. Convert & Load
			{
				// Convert FBX to zn mdl

				CznFBXLoaderParams loader;
				if (m_FileName.find("ground_dirt") != std::string::npos)
					loader.MakeCenterIsX0Z = true;
				if (m_FileName.find("cliffGrey") != std::string::npos)
					loader.MakeCenterIsX0Z = true;
				if (m_FileName.find("cliffBrown") != std::string::npos)
					loader.MakeCenterIsX0Z = true;
				//loader.ApplyFullTransform = true;


				IModelPtr fbxModel = GetBaseManager().GetManager<IznModelsFactory>()->LoadModel(m_FileName, &loader);

				auto znModelFile = GetBaseManager().GetManager<IznModelsFactory>()->SaveModel(fbxModel, convertedModelName);
				znModelFile->Save();


				// Save XML
				{
					auto fileNameStruct = Utils::SplitFilename(m_FileName);

					std::string znModelFilename = fileNameStruct.Path + fileNameStruct.NameWithoutExtension + ".xml";
					if (filesManager->IsFileExists(znModelFilename))
						filesManager->Delete(znModelFilename);

					auto znmdlModelsLoader = GetBaseManager().GetManager<IznModelsFactory>()->GetLoaderForModel("znmdl");
					auto znMdlXMLFile = znmdlModelsLoader->SaveModelXML(fbxModel, znModelFilename);
					znMdlXMLFile->Save();
				}

				
				m_Model = GetBaseManager().GetManager<IznModelsFactory>()->LoadModel(znModelFile);
				m_Model->SetName(fileNameStruct.NameWithoutExtension);
				Log::Info("Model '%s' loaded.", m_FileName.c_str());
				return true;
			}
		}
		else if (fileNameStruct.Extension == "znmdl")
		{
			_ASSERT(filesManager->IsFileExists(m_FileName));

			m_Model = GetBaseManager().GetManager<IznModelsFactory>()->LoadModel(m_FileName);
			m_Model->SetName(fileNameStruct.NameWithoutExtension);
			Log::Info("Model '%s' loaded.", m_FileName.c_str());
			return true;
		}
		else
		{
			throw CException("Model '%s' has unsupported extension '%s'.", m_FileName.c_str(), fileNameStruct.Extension.c_str());
		}
	}
	catch (const CException& e)
	{
		Log::Error("Error while loading '%s' Model.", m_FileName.c_str());
		Log::Error(e.MessageCStr());
	}

	return false;
}

bool CznModel3DTreeViewItemSource::Delete()
{
	return false;
}



//
// Protected
//
IBaseManager & CznModel3DTreeViewItemSource::GetBaseManager() const
{
	return m_BaseManager;
}
