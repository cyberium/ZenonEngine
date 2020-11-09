#include "stdafx.h"

// General
#include "Model3DTreeViewItemSource.h"


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

std::string CznModel3DTreeViewItemSource::GetName() const
{
	if (GetState() != ILoadable::ELoadableState::Loaded)
		return m_FileName;
	if (m_Model == nullptr)
		return "Error. " + m_FileName;
	return m_Model->GetName();
}

size_t CznModel3DTreeViewItemSource::GetChildsCount() const
{
	return 0;
}

std::shared_ptr<IznTreeViewItemSource> CznModel3DTreeViewItemSource::GetChild(size_t Index) const
{
	_ASSERT(false);
	return nullptr;
}

std::shared_ptr<IObject> CznModel3DTreeViewItemSource::Object() const
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
	auto gameDataStorage = filesManager->GetStorage(EFilesStorageType::GAMEDATA);
	auto gameDataStorageEx = std::dynamic_pointer_cast<IznFilesStorageExtended>(gameDataStorage);
	_ASSERT(gameDataStorageEx != nullptr);

	try
	{
		if (m_FileName.empty())
			throw CException("Filename is emprty.");

		auto pathAndNameExtension = Utils::GetFilenamePathAndNameExtension(m_FileName);
		auto pathNameAndExtension = Utils::GetFilenamePathNameAndExtension(m_FileName);
		if (pathNameAndExtension.second == "fbx")
		{
			std::string convertedModelName = pathNameAndExtension.first + ".znmdl";
			if (filesManager->IsFileExists(convertedModelName)) // .znmdl file exists. Load it.
			{
				m_Model = GetBaseManager().GetManager<IznModelsFactory>()->LoadModel(convertedModelName);
				m_Model->SetName(pathAndNameExtension.second);
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
				
				m_Model = GetBaseManager().GetManager<IznModelsFactory>()->LoadModel(znModelFile);
				m_Model->SetName(pathAndNameExtension.second);
				Log::Info("Model '%s' loaded.", m_FileName.c_str());
				return true;
			}
		}
		else if (pathNameAndExtension.second == "znmdl")
		{
			if (filesManager->IsFileExists(m_FileName)) // znMDL file exists. Load it.
			{
				m_Model = GetBaseManager().GetManager<IznModelsFactory>()->LoadModel(m_FileName);
				m_Model->SetName(pathAndNameExtension.second);
				Log::Info("Model '%s' loaded.", m_FileName.c_str());
				return true;
			}
		}
		else
		{
			throw CException("Model '%s' has unsupported extension '%s'.", m_FileName.c_str(), pathNameAndExtension.second.c_str());
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
std::shared_ptr<IFile> CznModel3DTreeViewItemSource::ConvertFBXToZNMDL(const std::string & FbxFilename)
{
	return std::shared_ptr<IFile>();
}

IBaseManager & CznModel3DTreeViewItemSource::GetBaseManager() const
{
	return m_BaseManager;
}
