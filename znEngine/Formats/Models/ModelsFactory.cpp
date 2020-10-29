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

std::shared_ptr<IModel> CznModelsFactory::LoadModel(const std::string& ModelFileName, const std::shared_ptr<IznLoaderParams>& LoaderParams)
{
	// Find existsing cached
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

	for (const auto& loader : m_ModelsLoaders)
	{
		if (loader->IsSupportedFormat(ModelFileName))
		{
			std::shared_ptr<IModel> model = loader->LoadModel(ModelFileName, LoaderParams);
			m_ModelsByName[ModelFileName] = model;
			return model;
		}
	}

	throw CException("The loader for model '%s' doesn't exists.", ModelFileName.c_str());
}

std::shared_ptr<IModel> CznModelsFactory::LoadModel(const std::shared_ptr<IFile>& ModelFile, const std::shared_ptr<IznLoaderParams>& LoaderParams)
{
	if (ModelFile == nullptr)
		throw CException("Can't load nullptr file.");

	// Find existsing cached
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

	for (const auto& loader : m_ModelsLoaders)
	{
		if (loader->IsSupportedFormat(ModelFile))
		{
			std::shared_ptr<IModel> model = loader->LoadModel(ModelFile, LoaderParams);
			m_ModelsByName[ModelFile->Path_Name()] = model;
			return model;
		}
	}

	throw CException("The loader for model '%s' doesn't exists.", ModelFile->Path_Name().c_str());
}

std::shared_ptr<IFile> CznModelsFactory::SaveModel(const std::shared_ptr<IModel>& Model, const std::string& FileName)
{
	_ASSERT(Model != nullptr);
	_ASSERT(FileName.size() > 0);
	
	auto file = m_BaseManager.GetManager<IFilesManager>()->Create(FileName);

	for (const auto& loader : m_ModelsLoaders)
	{
		if (loader->IsSupportedFormat(file))
		{
			return loader->SaveModel(Model, FileName);
		}
	}

	throw CException("The loader for model '%s' doesn't exists.", FileName.c_str());
}
