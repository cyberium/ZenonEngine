#include "stdafx.h"

// General
#include "ModelTreeViewItem.h"

// Additional
#include "../znPluginFBXModels/FBXInterfaces.h"

CModelTreeViewItem::CModelTreeViewItem(IBaseManager& BaseManager, std::string ModelFileName)
	: m_BaseManager(BaseManager)
	, m_ModelFileName(ModelFileName)
{
}

CModelTreeViewItem::~CModelTreeViewItem()
{
}



//
// IznTreeViewItem
//
ETreeViewItemType CModelTreeViewItem::GetType() const
{
	return ETreeViewItemType::Model;
}

std::string CModelTreeViewItem::GetText() const
{
	if (GetState() != ILoadable::ELoadableState::Loaded)
		return m_ModelFileName;
	if (m_Model == nullptr)
		return "Error. " + m_ModelFileName;
	return m_Model->GetName();
}

std::string CModelTreeViewItem::GetIconName() const
{
	return "model";
}

std::shared_ptr<IObject> CModelTreeViewItem::GetObject_() const
{
	if (GetState() != ILoadable::ELoadableState::Loaded)
		return nullptr;
	return m_Model;
}



//
// ILoadable
//
bool CModelTreeViewItem::Load()
{
	auto filesManager = m_BaseManager.GetManager<IFilesManager>();
	auto gameDataStorage = filesManager->GetStorage(EFilesStorageType::USERDATA);
	auto gameDataStorageEx = std::dynamic_pointer_cast<IznFilesStorageExtended>(gameDataStorage);
	_ASSERT(gameDataStorageEx != nullptr);

	try
	{
		if (m_ModelFileName.empty())
			throw CException("Filename is emprty.");

		auto fileNameStruct = Utils::SplitFilename(m_ModelFileName);
		std::string convertedModelName = fileNameStruct.Path + fileNameStruct.NameWithoutExtension + ".znmdl";
		std::string convertedModelNameXML = fileNameStruct.Path + fileNameStruct.NameWithoutExtension + ".znxmdl";

		if (fileNameStruct.Extension == "fbx")
		{
			if (filesManager->IsFileExists(convertedModelNameXML))
			{
				m_Model = GetBaseManager().GetManager<IznModelsFactory>()->LoadModel(convertedModelNameXML);
				m_Model->SetName(fileNameStruct.NameWithoutExtension);
				Log::Info("Model '%s' loaded.", m_ModelFileName.c_str());
				return true;
			}
			else if (filesManager->IsFileExists(convertedModelName)) // .znmdl file exists. Load it.
			{
				m_Model = GetBaseManager().GetManager<IznModelsFactory>()->LoadModel(convertedModelName);
				m_Model->SetName(fileNameStruct.NameWithoutExtension);
				Log::Info("Model '%s' loaded.", m_ModelFileName.c_str());
				return true;
			}
			else if (filesManager->IsFileExists(m_ModelFileName)) // *.fbx file exists. Convert & Load
			{
				// Convert FBX to zn mdl

				CznFBXLoaderParams loader;
				if (m_ModelFileName.find("ground_dirt") != std::string::npos)
					loader.MakeCenterIsX0Z = true;
				if (m_ModelFileName.find("cliffGrey") != std::string::npos)
					loader.MakeCenterIsX0Z = true;
				if (m_ModelFileName.find("cliffBrown") != std::string::npos)
					loader.MakeCenterIsX0Z = true;
				//loader.ApplyFullTransform = true;


				std::shared_ptr<IModel> fbxModel = GetBaseManager().GetManager<IznModelsFactory>()->LoadModel(m_ModelFileName, &loader);


				// Save znmdl
				auto znModelFile = GetBaseManager().GetManager<IznModelsFactory>()->SaveModel(fbxModel, convertedModelName);
				znModelFile->Save();


				// Save znxmdl
				GetBaseManager().GetManager<IznModelsFactory>()->SaveModel(fbxModel, convertedModelNameXML)->Save();;

				
				m_Model = GetBaseManager().GetManager<IznModelsFactory>()->LoadModel(znModelFile);
				m_Model->SetName(fileNameStruct.NameWithoutExtension);
				Log::Info("Model '%s' loaded.", m_ModelFileName.c_str());
				return true;
			}
		}
		else if (fileNameStruct.Extension == "znmdl" || fileNameStruct.Extension == "znxmdl")
		{
			_ASSERT(filesManager->IsFileExists(m_ModelFileName));

			m_Model = GetBaseManager().GetManager<IznModelsFactory>()->LoadModel(m_ModelFileName);
			m_Model->SetName(fileNameStruct.NameWithoutExtension);
			Log::Info("Model '%s' loaded.", m_ModelFileName.c_str());
			return true;
		}
		else
		{
			throw CException("Model '%s' has unsupported extension '%s'.", m_ModelFileName.c_str(), fileNameStruct.Extension.c_str());
		}
	}
	catch (const CException& e)
	{
		Log::Error("Error while loading '%s' Model.", m_ModelFileName.c_str());
		Log::Error(e.MessageCStr());
	}

	return false;
}

bool CModelTreeViewItem::Delete()
{
	return false;
}



//
// Protected
//
IBaseManager & CModelTreeViewItem::GetBaseManager() const
{
	return m_BaseManager;
}
