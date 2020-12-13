#pragma once

#include "ResourcesFilesystemIntfs.h"

class CResourceFilesystem
	: public IResourceFilesystem
{
public:
	CResourceFilesystem();
	virtual ~CResourceFilesystem();

	// IResourceFilesystem
	void Initailize(const std::string& Path) override;
	void PrintFilesystem() override;
	std::shared_ptr<IResourceFile> GetRootFile() const override;

private:
	std::shared_ptr<IResourceFile> m_RootFile;
};