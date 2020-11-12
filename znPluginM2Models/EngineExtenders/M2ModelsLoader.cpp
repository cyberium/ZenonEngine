#include "stdafx.h"

// General
#include "M2ModelsLoader.h"

namespace
{
	const char * cLoaderSupportedExtension = "m2";
}

CznM2ModelsLoader::CznM2ModelsLoader(IBaseManager& BaseManager)
	: m_BaseManager(BaseManager)
{

}

CznM2ModelsLoader::~CznM2ModelsLoader()
{

}



//
// IznModelsLoader
//
std::string CznM2ModelsLoader::GetName() const
{
	return "M2ModelsLoader";
}

bool CznM2ModelsLoader::IsSupportedFormat(const std::string& ModelFileName) const
{
	std::string lowerFileName = Utils::ToLower(ModelFileName);
	std::string extension = lowerFileName;
	if (ModelFileName.length() > strlen(cLoaderSupportedExtension))
		extension = extension.substr(extension.length() - strlen(cLoaderSupportedExtension));
	return extension == cLoaderSupportedExtension;
}

bool CznM2ModelsLoader::IsSupportedFormat(const std::shared_ptr<IFile>& ModelFile) const
{
	return ModelFile->Extension() == cLoaderSupportedExtension;
}

std::shared_ptr<IModel> CznM2ModelsLoader::LoadModel(const std::string& ModelFileName, const IznLoaderParams* LoaderParams) const
{
	auto modelFile = m_BaseManager.GetManager<IFilesManager>()->Open(ModelFileName);
	if (modelFile == nullptr)
		throw CException("Model file '%s' not found.", ModelFileName.c_str());
	return LoadModel(modelFile, LoaderParams);
}

std::shared_ptr<IModel> CznM2ModelsLoader::LoadModel(const std::shared_ptr<IFile>& ModelFile, const IznLoaderParams* LoaderParams) const
{
	return nullptr; // LoadScene(ModelFile, LoaderParams)->MergeModels();
}

std::shared_ptr<IFile> CznM2ModelsLoader::SaveModel(const std::shared_ptr<IModel>& Model, const std::string & FileName) const
{
	throw CException("Operation not supported.");
}
