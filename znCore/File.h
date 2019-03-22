#pragma once

#include "ByteBuffer.h"

class CFile : public IFile
{
public:
	 CFile(cstring _root, cstring _fullFileName);
	 CFile(cstring _root, cstring _name, cstring _path);
	 virtual ~CFile();

	 CByteBuffer& GetByteBuffer() { return m_ByteBuffer; }

	 // IFile
	 std::string Name() const override { return m_Name; }
	 std::string Path() const override { return m_Path; }
	 std::string Extension() const override { return m_Extension; }
	 std::string Path_Name() const override { return std::string(m_Path + m_Name); }
	 std::string Full_Path_Name() const override { return std::string(m_Root + m_Path + m_Name); }

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
	 bool isEof() const override 
	 { 
		 return m_ByteBuffer.isEof(); 
	 }
	 void seek(size_t _bufferOffsetAbsolute) override
	 { 
		 m_ByteBuffer.seek(_bufferOffsetAbsolute); 
	 }
	 void seekRelative(intptr_t _bufferOffsetRelative) override
	 { 
		 m_ByteBuffer.seekRelative(_bufferOffsetRelative); 
	 }
	 bool readLine(std::string* _string) override
	 { 
		 return m_ByteBuffer.readLine(_string);
	 }
	 void readBytes(void* _destination, size_t _size) override
	 { 
		 m_ByteBuffer.readBytes(_destination, _size); 
	 }
	 void readString(std::string* _string) override
	 {
		 m_ByteBuffer.readString(_string);
	 }

public:
	static void FixFilePath(std::string& _string);

private:
	void ParsePathAndExtension();

protected:
	CByteBuffer m_ByteBuffer;

private: // IFile
	std::string m_Name;
	std::string m_Path;
	std::string m_Extension;
	std::string m_Root;
};