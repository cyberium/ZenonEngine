#include "stdafx.h"

// General
#include "ModelsFactory.h"

CznModelsFactory::CznModelsFactory(IBaseManager& BaseManager)
	: m_BaseManager(BaseManager)
{
}

CznModelsFactory::~CznModelsFactory()
{
}



//
// IznModelsFactory
//
void CznModelsFactory::AddModelsLoader(const std::shared_ptr<IznModelsLoader>& ModelsLaoder)
{
	_ASSERT(std::find_if(m_ModelsLoaders.begin(), m_ModelsLoaders.end(), [&ModelsLaoder] (const std::shared_ptr<IznModelsLoader>& Loader) ->  bool {
		return Loader->GetName() == ModelsLaoder->GetName();
	}) == m_ModelsLoaders.end());
	m_ModelsLoaders.push_back(ModelsLaoder);
}

void CznModelsFactory::RemoveModelsLoader(const std::shared_ptr<IznModelsLoader>& ModelsLaoder)
{
	const auto& it = std::find_if(m_ModelsLoaders.begin(), m_ModelsLoaders.end(), [&ModelsLaoder](const std::shared_ptr<IznModelsLoader>& Loader) ->  bool {	return Loader->GetName() == ModelsLaoder->GetName(); });
	if (it == m_ModelsLoaders.end())
		return;
	m_ModelsLoaders.erase(it);
}

const std::shared_ptr<IznModelsLoader> CznModelsFactory::GetLoaderForModel(const std::string & ModelFileName)
{
	for (const auto& loader : m_ModelsLoaders)
		if (loader->IsSupportedFormat(ModelFileName))
			return loader;
	return nullptr;
}

std::shared_ptr<IModel> CznModelsFactory::LoadModel(const std::string& ModelFileName, const IznLoaderParams* LoaderParams)
{
	// Find existsing cached
	{
		std::lock_guard<std::mutex> lock(m_LockMutex);

		const auto& iter = m_ModelsByName.find(ModelFileName);
		if (iter != m_ModelsByName.end())
		{
			if (auto model = iter->second.lock())
			{
				return model;
			}
			else
			{
				m_ModelsByName.erase(iter);
			}
		}
	}

	for (const auto& loader : m_ModelsLoaders)
	{
		if (loader->IsSupportedFormat(ModelFileName))
		{
			std::shared_ptr<IModel> model = loader->LoadModel(ModelFileName, LoaderParams);
			{
				std::lock_guard<std::mutex> lock(m_LockMutex);
				m_ModelsByName[ModelFileName] = model;
			}
			return model;
		}
	}

	throw CException("The loader for model '%s' doesn't exists.", ModelFileName.c_str());
}

std::shared_ptr<IModel> CznModelsFactory::LoadModel(const std::shared_ptr<IFile>& ModelFile, const IznLoaderParams* LoaderParams)
{
	if (ModelFile == nullptr)
		throw CException("Can't load nullptr file.");

	ModelFile->seek(0);

	// Find existsing cached
	{
		std::lock_guard<std::mutex> lock(m_LockMutex);

		const auto& iter = m_ModelsByName.find(ModelFile->Path_Name());
		if (iter != m_ModelsByName.end())
		{
			if (auto model = iter->second.lock())
			{
				return model;
			}
			else
			{
				m_ModelsByName.erase(iter);
			}
		}
	}

	for (const auto& loader : m_ModelsLoaders)
	{
		if (loader->IsSupportedFormat(ModelFile))
		{
			std::shared_ptr<IModel> model = loader->LoadModel(ModelFile, LoaderParams);
			{
				std::lock_guard<std::mutex> lock(m_LockMutex);
				m_ModelsByName[ModelFile->Path_Name()] = model;
			}
			return model;
		}
	}

	throw CException("The loader for model '%s' doesn't exists.", ModelFile->Path_Name().c_str());
}

std::shared_ptr<IFile> CznModelsFactory::SaveModel(const std::shared_ptr<IModel>& Model, const std::string& FileName)
{
	if (Model == nullptr)
		throw CException("Can't load nullptr file.");

	if (FileName.empty())
		throw CException("Filename is empty.");
	
	auto filesManager = m_BaseManager.GetManager<IFilesManager>();
	if (filesManager->IsFileExists(FileName))
	{
		Log::Warn("ModelsFactory: Model file '%s' already exists. It will be deleted.", FileName.c_str());
		filesManager->Delete(FileName);
	}

	auto file = filesManager->Create(FileName);

	for (const auto& loader : m_ModelsLoaders)
		if (loader->IsSupportedFormat(file))
			return loader->SaveModel(Model, FileName);

	throw CException("The loader for model '%s' doesn't exists.", FileName.c_str());
}
