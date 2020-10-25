#pragma once

#include "BaseManager.h"

// FORWARD BEGIN
ZN_INTERFACE IModel;
// FORWARD END


ZN_INTERFACE ZN_API IznLoaderParams
{
	virtual ~IznLoaderParams() {}
};

ZN_INTERFACE ZN_API IznModelsLoader
{
	virtual ~IznModelsLoader() {}

	virtual std::string GetName() const = 0;
	virtual bool IsSupportedFormat(const std::string& ModelFileName) const = 0;
	virtual bool IsSupportedFormat(const std::shared_ptr<IFile>& ModelFile) const = 0;
	virtual std::shared_ptr<IModel> LoadModel(const std::string& ModelFileName, const std::shared_ptr<IznLoaderParams>& LoaderParams = nullptr) const = 0;
	virtual std::shared_ptr<IModel> LoadModel(const std::shared_ptr<IFile>& ModelFile, const std::shared_ptr<IznLoaderParams>& LoaderParams = nullptr) const = 0;
	virtual std::shared_ptr<IFile> SaveModel(const std::shared_ptr<IModel>& Model, const std::string& FileName) const = 0;
};


ZN_INTERFACE ZN_API
	__declspec(uuid("ED37C829-7EC4-4EB9-9B62-524855EF8CFE"))
	IznModelsManager : public IManager
{
	virtual ~IznModelsManager() {};

	virtual void AddModelsLoader(const std::shared_ptr<IznModelsLoader>& ModelsLaoder) = 0;
	virtual void RemoveModelsLoader(const std::shared_ptr<IznModelsLoader>& ModelsLaoder) = 0;
	virtual const std::shared_ptr<IznModelsLoader> GetLoaderForModel(const std::string& ModelFileName) = 0;
	virtual std::shared_ptr<IModel> LoadModel(const std::string& ModelFileName, const std::shared_ptr<IznLoaderParams>& LoaderParams = nullptr) = 0;
	virtual std::shared_ptr<IModel> LoadModel(const std::shared_ptr<IFile>& ModelFile, const std::shared_ptr<IznLoaderParams>& LoaderParams = nullptr) = 0;
	virtual std::shared_ptr<IFile> SaveModel(const std::shared_ptr<IModel>& Model, const std::string& FileName) = 0;
};
