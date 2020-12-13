#include "stdafx.h"

// General
#include "ResourcesFilesystem.h"

// Additional
#include "ResourceFile.h"

#include <filesystem>
namespace fs = std::experimental::filesystem;


CResourceFilesystem::CResourceFilesystem()
{}

CResourceFilesystem::~CResourceFilesystem()
{}



namespace
{
	void BuildFileTreeRec(const std::string& RootPath, const std::string& Path, std::shared_ptr<IResourceFile> OwnerResourceFile)
	{
		if (false == fs::exists(Path))
			throw CException("Unable to get all files. '%s' is not exists.", Path.c_str());

		if (false == fs::is_directory(Path))
			throw CException("Unable to get all files. '%s' is not a directory.", Path.c_str());

		try
		{
			fs::recursive_directory_iterator iter(Path), end;
			while (iter != end)
			{
				std::string absoluteCurrentFilename = Utils::FixFilePath(iter->path().string());
				std::string relativeCurrectFilename = absoluteCurrentFilename.substr(RootPath.length() + 1);

				if (fs::is_regular_file(iter->path()))
				{
					std::shared_ptr<CResoureFile> file = MakeShared(CResoureFile, relativeCurrectFilename);
					file->SetResourceType(EResourceFileType::File);
					std::dynamic_pointer_cast<IResourceFileInternal>(OwnerResourceFile)->AddChild(file);
				}
				else if (fs::is_directory(iter->path()))
				{
					std::shared_ptr<CResoureFile> directory = MakeShared(CResoureFile, relativeCurrectFilename);
					directory->SetResourceType(EResourceFileType::Directory);
					std::dynamic_pointer_cast<IResourceFileInternal>(OwnerResourceFile)->AddChild(directory);

					iter.disable_recursion_pending();

					BuildFileTreeRec(RootPath, absoluteCurrentFilename, directory);
				}
				else
					throw CException("Unknown file '%s'.", absoluteCurrentFilename.c_str());

				std::error_code ec;
				iter.increment(ec);
				if (ec)
					throw CException("Error while accessing '%s'. Error: '%s'.", absoluteCurrentFilename.c_str(), ec.message().c_str());
			}
		}
		catch (const std::system_error& e)
		{
			throw CException("std::system_error '%s'.", e.what());
		}
	}
}



//
// IResourceFilesystem
//
void CResourceFilesystem::Initailize(const std::string& Path)
{
	std::shared_ptr<CResoureFile> rootDirectory = MakeShared(CResoureFile, Path);
	rootDirectory->SetResourceType(EResourceFileType::Directory);

	BuildFileTreeRec(Path, Path, rootDirectory);

	m_RootFile = rootDirectory;
}

namespace
{
	void PrintFilesystemInernal(std::shared_ptr<IResourceFile> File, size_t Offset)
	{
		std::string result = "";
		for (size_t i = 0; i < Offset; i++)
			result += "   ";
		result += File->GetRelativePath();
		Log::Print(result.c_str());

		for (const auto& child : File->GetChilds())
			PrintFilesystemInernal(child, Offset + 1);
	}
}

void CResourceFilesystem::PrintFilesystem()
{
	PrintFilesystemInernal(m_RootFile, 0);
}

std::shared_ptr<IResourceFile> CResourceFilesystem::GetRootFile() const
{
	return m_RootFile;
}
