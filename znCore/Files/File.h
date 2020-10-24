#pragma once

#include "ByteBuffer.h"

class ZN_API CFile 
	: public IFile
{
public:
	 CFile(const std::string& FullFileName, const std::shared_ptr<IFilesStorage>& OwnerFileStorage);
	 virtual ~CFile();

	 CByteBuffer& GetByteBuffer() { return m_ByteBuffer; }

	 // IFile
	 void ChangeExtension(const std::string& NewExtension) override;
	 bool Save() override;
	 bool SaveAs(const std::string& FileName) override;

	 std::string Name() const override;
	 std::string Path() const override;
	 std::string Extension() const override;

	 std::string Path_Name() const override;
	 std::string Name_NoExtension() const override;


	 // IByteBuffer
	 size_t getSize() const override
	 { 
		 return m_ByteBuffer.getSize(); 
	 }
	 size_t getPos() const override
	 { 
		 return m_ByteBuffer.getPos(); 
	 }
	 const uint8* getData() const override 
	 { 
		 return m_ByteBuffer.getData(); 
	 }
	 const uint8* getDataFromCurrent() const override 
	 { 
		 return m_ByteBuffer.getDataFromCurrent(); 
	 }
	 uint8* getDataEx() override
	 {
		 return m_ByteBuffer.getDataEx();
	 }
	 uint8* getDataFromCurrentEx() override
	 {
		 return m_ByteBuffer.getDataFromCurrentEx();
	 }
	 bool isEof() const override 
	 { 
		 return m_ByteBuffer.isEof(); 
	 }
	 bool seek(size_t _bufferOffsetAbsolute) override
	 { 
		 return m_ByteBuffer.seek(_bufferOffsetAbsolute);
	 }
	 bool seekRelative(intptr_t _bufferOffsetRelative) override
	 { 
		 return m_ByteBuffer.seekRelative(_bufferOffsetRelative); 
	 }
	 bool readLine(std::string* _string) override
	 { 
		 return m_ByteBuffer.readLine(_string);
	 }
	 bool getText(std::string * String) override
	 {
		 return m_ByteBuffer.getText(String);
	 }
	 bool readBytes(void* _destination, size_t _size) override
	 { 
		 return m_ByteBuffer.readBytes(_destination, _size); 
	 }
	 bool readString(std::string* _string) override
	 {
		 return m_ByteBuffer.readString(_string);
	 }

	 void writeLine(const std::string& String) override
	 {
		 m_ByteBuffer.writeLine(String);
	 }
	 void writeBytes(const void * Source, size_t BytesCount) override
	 {
		 m_ByteBuffer.writeBytes(Source, BytesCount);
	 }
	 void writeDummy(size_t BytesCount) override
	 {
		 m_ByteBuffer.writeDummy(BytesCount);
	 }
	 void insert(size_t Position, const void * DataPtr, size_t DataSize)
	 {
		 m_ByteBuffer.insert(Position, DataPtr, DataSize);
	 }
	 void writeString(const std::string& String) override
	 {
		 m_ByteBuffer.writeString(String);
	 }

private:
	void ParsePathAndExtension(const std::string& FullPath);
	void RebuildFullName();

protected:
	CByteBuffer m_ByteBuffer;

private: // IFile
	std::string m_Name;
	std::string m_NameWithoutExtension;
	std::string m_Path;
	std::string m_Extension;
	std::shared_ptr<IFilesStorage> m_OwnerFilesStorage;
};