#pragma once

enum class EResourceFileType
{
	File,
	Directory
};

ZN_INTERFACE IResourceFile
{
	virtual ~IResourceFile() {}

	virtual bool IsFile() const = 0;
	virtual bool IsDirectory() const = 0;
	virtual Utils::SFileName GetFilenameStruct() const = 0;
	virtual std::string GetRelativePath() const = 0;
	virtual EResourceFileType GetResourceType() const = 0;
	virtual const std::vector<std::shared_ptr<IResourceFile>>& GetChilds() const = 0;
};


ZN_INTERFACE IResourceFileInternal
{
	virtual ~IResourceFileInternal() {}

	virtual void SetResourceType(EResourceFileType ResourceFileType) = 0;
	virtual void AddChild(std::shared_ptr<IResourceFile> ResourceFile) = 0;
};


ZN_INTERFACE IResourceFilesystem
{
	virtual ~IResourceFilesystem() {}

	virtual void Initailize(const std::string& Path) = 0;
	virtual void PrintFilesystem() = 0;
	virtual std::shared_ptr<IResourceFile> GetRootFile() const = 0;
};