#pragma once

// FORWARD BEGIN
ZN_INTERFACE IManager;
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

	virtual std::shared_ptr<IModel> LoadModel(const std::string& ModelFileName, const IznLoaderParams* LoaderParams = nullptr) const = 0;
	virtual std::shared_ptr<IModel> LoadModel(const std::shared_ptr<IFile>& ModelFile, const IznLoaderParams* LoaderParams = nullptr) const = 0;
	virtual std::shared_ptr<IFile> SaveModel(const std::shared_ptr<IModel>& Model, const std::string& FileName) const = 0;
};


ZN_INTERFACE ZN_API	IznModelsFactory 
	: public IManager
{
	ZN_OBJECTCLASS(cModelsFactory)

	virtual ~IznModelsFactory() {};

	virtual void AddModelsLoader(const std::shared_ptr<IznModelsLoader>& ModelsLaoder) = 0;
	virtual void RemoveModelsLoader(const std::shared_ptr<IznModelsLoader>& ModelsLaoder) = 0;
	virtual const std::shared_ptr<IznModelsLoader> GetLoaderForModel(const std::string& ModelFileName) = 0;
	virtual std::shared_ptr<IModel> LoadModel(const std::string& ModelFileName, const IznLoaderParams* LoaderParams = nullptr) = 0;
	virtual std::shared_ptr<IModel> LoadModel(const std::shared_ptr<IFile>& ModelFile, const IznLoaderParams* LoaderParams = nullptr) = 0;
	virtual std::shared_ptr<IFile> SaveModel(const std::shared_ptr<IModel>& Model, const std::string& FileName) = 0;
};
