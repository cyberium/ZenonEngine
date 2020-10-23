#pragma once

#ifdef ZN_FBX_SDK_ENABLE
#include <fbxsdk.h>
#include "FBX/FBXInterfaces.h"

class CznFBXModelsLoader
	: public IznModelsLoader
{
public:
	CznFBXModelsLoader(IBaseManager& BaseManager);
	virtual ~CznFBXModelsLoader();

	// IznModelsLoader
	std::string GetName() const override;
	bool IsSupportedFormat(const std::shared_ptr<IFile>& ModelFile) const override;
	std::shared_ptr<IModel> LoadModel(const std::shared_ptr<IFile>& ModelFile, const std::string& TexturesPath = "") const override;


private:
	fbxsdk::FbxManager* m_FBXManager;

private:
	IBaseManager& m_BaseManager;
};

#endif
