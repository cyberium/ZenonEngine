#pragma once

#include "ByteBuffer.h"

class ZN_API CFile 
	: public CByteBuffer
	, public IFile
{
public:
	 CFile(const std::string& FullFileName, const std::shared_ptr<IznFilesStorage>& OwnerFileStorage);
	 virtual ~CFile();

	 CByteBuffer& GetByteBuffer() { return *this; }
	 const CByteBuffer& GetByteBuffer() const { return *this; }

	 // IFile
	 void ChangeExtension(const std::string& NewExtension) override;
	 bool Save() override;
	 bool SaveAs(const std::string& FileName) override;

	 std::string Name() const override;
	 std::string Path() const override;
	 std::string Extension() const override;

	 std::string Path_Name() const override;
	 std::string Name_NoExtension() const override;

private:
	void ParsePathAndExtension(const std::string& FullPath);
	void RebuildFullName();

private: // IFile
	std::string m_Name;
	std::string m_NameWithoutExtension;
	std::string m_Path;
	std::string m_Extension;
	std::shared_ptr<IznFilesStorage> m_OwnerFilesStorage;
};