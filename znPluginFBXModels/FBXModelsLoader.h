#pragma once

#ifdef ZN_FBX_SDK_ENABLE
#include <fbxsdk.h>
#include "FBXInterfaces.h"

class CznFBXModelsLoader
	: public IznModelsLoader
	, public IFBXSceneLoader
{
public:
	CznFBXModelsLoader(IBaseManager& BaseManager);
	virtual ~CznFBXModelsLoader();

	// IznModelsLoader
	std::string GetName() const override;
	bool IsSupportedFormat(const std::string& ModelFileName) const override;
	bool IsSupportedFormat(const std::shared_ptr<IFile>& ModelFile) const override;
	std::shared_ptr<IModel> LoadModel(const std::string& ModelFileName, const IznLoaderParams* LoaderParams = nullptr) const override;
	std::shared_ptr<IModel> LoadModel(const std::shared_ptr<IFile>& ModelFile, const IznLoaderParams* LoaderParams) const override;
	std::shared_ptr<IFile> SaveModel(const std::shared_ptr<IModel>& Model, const std::string& FileName) const override;

	std::shared_ptr<IModel> LoadModelXML(const std::string& ModelFileName, const IznLoaderParams* LoaderParams = nullptr) const override;
	std::shared_ptr<IModel> LoadModelXML(const std::shared_ptr<IFile>& ModelFile, const IznLoaderParams* LoaderParams = nullptr) const override;
	std::shared_ptr<IFile> SaveModelXML(const std::shared_ptr<IModel>& Model, const std::string& FileName) const override;

	// IFBXSceneLoader
	std::shared_ptr<IFBXScene> LoadScene(const std::string& ModelFileName, const IznLoaderParams* LoaderParams) const override;
	std::shared_ptr<IFBXScene> LoadScene(const std::shared_ptr<IFile>& ModelFile, const IznLoaderParams* LoaderParams) const override;

private:
	mutable std::mutex m_FBXManagerLock;
	fbxsdk::FbxManager* m_FBXManager;

private:
	IBaseManager& m_BaseManager;
};

#endif
