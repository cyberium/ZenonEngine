#pragma once

#include "ResourcesFilesystemIntfs.h"

class CResoureFile
	: public IResourceFile
	, public IResourceFileInternal
{
public:
	CResoureFile(const std::string& Path);
	virtual ~CResoureFile();

	// IResourceFile
	bool IsFile() const override;
	bool IsDirectory() const override;
	Utils::SFileName GetFilenameStruct() const override;
	std::string GetRelativePath() const override;
	EResourceFileType GetResourceType() const override;
	const std::vector<std::shared_ptr<IResourceFile>>& GetChilds() const override;

	// IResourceFileInternal
	void SetResourceType(EResourceFileType ResourceFileType) override;
	void AddChild(std::shared_ptr<IResourceFile> ResourceFile) override;

private:
	Utils::SFileName m_FilenameStruct;
	std::string m_RelativePath;
	EResourceFileType m_FileType;
	std::vector<std::shared_ptr<IResourceFile>> m_Childs;
};
