#include "stdafx.h"

// General
#include "File.h"

// Additional
#include "Utils/Utils.h"

CFile::CFile(const std::string& FullFileName, const std::shared_ptr<IznFilesStorage>& OwnerFileStorage)
	: m_Name(FullFileName)
	, m_Path("")
	, m_OwnerFilesStorage(OwnerFileStorage)
{
	ParsePathAndExtension(FullFileName);
}

CFile::~CFile()
{
	//Log::Info("File[%s] closed.", Path_Name().c_str());
}



//
// IFile
//
void CFile::ChangeExtension(const std::string& NewExtension)
{
	m_Extension = NewExtension;
	m_Extension = Utils::ToLower(m_Extension);
	RebuildFullName();
}

bool CFile::Save()
{
	try
	{
		std::dynamic_pointer_cast<IznFilesStorageExtended>(m_OwnerFilesStorage)->Save(shared_from_this());
		return true;
	}
	catch (const CException& e)
	{
		Log::Error("Error while saving file '%s'.", Path_Name().c_str());
		Log::Error("---> '%s'", e.MessageCStr());
		return false;
	}

	return false;
}

bool CFile::SaveAs(const std::string& FileName)
{
	ParsePathAndExtension(FileName);
	return Save();
}

std::string CFile::Name() const
{
	return m_Name;
}

std::string CFile::Path() const
{
	return m_Path;
}

std::string CFile::Extension() const
{
	return m_Extension;
}

std::string CFile::Path_Name() const
{
	return std::string(m_Path + m_Name);
}

std::string CFile::Name_NoExtension() const
{
	return m_NameWithoutExtension;
}



//
// Private
//
void CFile::ParsePathAndExtension(const std::string& FullPath)
{
	// Replace slahes
	m_Name = Utils::FixFilePath(FullPath);
	m_Path = "";
	m_NameWithoutExtension = "";
	m_Extension = "";

	// Find name and path
	auto lastSlashPos = m_Name.find_last_of('/');
	if (lastSlashPos != std::string::npos)
	{
		m_Path += m_Name.substr(0, lastSlashPos + 1);
		m_Name = m_Name.substr(lastSlashPos + 1);
		m_NameWithoutExtension = m_Name;
	}

	// Extension
	auto lastPointPos = m_Name.find_last_of('.');
	if (lastPointPos != std::string::npos)
	{
		m_NameWithoutExtension = m_Name.substr(0, lastPointPos /*+ 1*/);
		m_Extension = m_Name.substr(lastPointPos + 1);
		m_Extension = Utils::ToLower(m_Extension);
	}
}

void CFile::RebuildFullName()
{
	ParsePathAndExtension(m_Path + m_NameWithoutExtension + "." + m_Extension);
}
