#include "stdafx.h"

// General
#include "EngineModelsLoader.h"

CznEngineModelsLoader::CznEngineModelsLoader(IBaseManager& BaseManager)
	: m_BaseManager(BaseManager)
{
}

CznEngineModelsLoader::~CznEngineModelsLoader()
{
}



//
// IznModelsLoader
//
std::string CznEngineModelsLoader::GetName() const
{
	return "EngineModelsLoader";
}

bool CznEngineModelsLoader::IsSupportedFormat(const std::shared_ptr<IFile>& ModelFile) const
{
	return ModelFile->Extension() == "znmdl";
}

std::shared_ptr<IModel> CznEngineModelsLoader::LoadModel(const std::shared_ptr<IFile>& ModelFile, const std::string & TexturesPath) const
{
	IRenderDevice& renderDevice = m_BaseManager.GetApplication().GetRenderDevice();
	auto model = renderDevice.GetObjectsFactory().CreateModel();
	if (auto loadSave = std::dynamic_pointer_cast<IObjectLoadSave>(model))
	{
		loadSave->Load(ModelFile);
	}
	else
		_ASSERT(false);
	return model;
}
