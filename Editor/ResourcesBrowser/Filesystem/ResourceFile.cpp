#include "stdafx.h"

// General
#include "ResourceFile.h"


//
// CResoureFile
//
CResoureFile::CResoureFile(const std::string& Path)
	: m_RelativePath(Path)
	, m_FileType(EResourceFileType::File)
{
	m_FilenameStruct = Utils::SplitFilename(Path);
}

CResoureFile::~CResoureFile()
{
}



//
// IResourceFile
//
bool CResoureFile::IsFile() const
{
	return m_FileType != EResourceFileType::Directory;
}

bool CResoureFile::IsDirectory() const
{
	return m_FileType == EResourceFileType::Directory;
}

Utils::SFileName CResoureFile::GetFilenameStruct() const
{
	return m_FilenameStruct;
}

std::string CResoureFile::GetRelativePath() const
{
	return m_RelativePath;
}

EResourceFileType CResoureFile::GetResourceType() const
{
	return m_FileType;
}

const std::vector<std::shared_ptr<IResourceFile>>& CResoureFile::GetChilds() const
{
	return m_Childs;
}



//
// IResourceFileInternal
//
void CResoureFile::SetResourceType(EResourceFileType ResourceFileType)
{
	m_FileType = ResourceFileType;
}

void CResoureFile::AddChild(std::shared_ptr<IResourceFile> ResourceFile)
{
	_ASSERT(IsDirectory());
	m_Childs.push_back(ResourceFile);
}
