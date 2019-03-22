#pragma once

struct
	__declspec(novtable)
	IByteBuffer
{
	virtual ~IByteBuffer() {}

	virtual size_t        getSize() const = 0;
	virtual size_t        getPos() const = 0;
	virtual const uint8*  getData() const = 0;
	virtual const uint8*  getDataFromCurrent() const = 0;
	virtual bool          isEof() const = 0;

	virtual void          seek(size_t _bufferOffsetAbsolute) = 0;
	virtual void          seekRelative(intptr_t _bufferOffsetRelative) = 0;
	virtual bool          readLine(std::string* _string) = 0;
	virtual void          readBytes(void* _destination, size_t _size = 1) = 0;
	virtual void          readString(std::string* _string) = 0;
};

//--

struct
	__declspec(novtable)
	IByteBufferEx
{
	virtual         ~IByteBufferEx() {}

	virtual void    Allocate(size_t _size) = 0;
	virtual void    SetFilled() = 0;
	virtual void    CopyData(const uint8* _data, size_t _size) = 0;
	virtual uint8*  getDataEx() = 0;
	virtual uint8*  getDataFromCurrentEx() = 0;
};

//--

struct
	__declspec(novtable)
	IFile : public IByteBuffer
{
	virtual              ~IFile() {}

	virtual std::string  Name() const = 0;
	virtual std::string  Path() const = 0;
	virtual std::string  Extension() const = 0;
	virtual std::string  Path_Name() const = 0;
	virtual std::string  Full_Path_Name() const = 0;
};

//--

struct
	__declspec(novtable)
	IFilesStorage
{
	virtual ~IFilesStorage() {};

	virtual std::shared_ptr<IFile>  CreateFile(cstring _name) = 0;
	virtual size_t                  GetFileSize(cstring _name) = 0;
	virtual bool                    IsFileExists(cstring _name) = 0;
};

//--

struct
	__declspec(novtable)
	IFilesStorageEx
{
	enum Priority : uint8
	{
		PRIOR_LOWEST = 0,
		PRIOR_LOW,
		PRIOR_NORMAL,
		PRIOR_HIGH,
		PRIOR_HIGHEST
	};

	virtual ~IFilesStorageEx() {};

	virtual Priority GetPriority() const = 0;
};

//--

struct 
	__declspec(novtable, uuid("5DC32EB8-9A63-4FAD-A4BF-81916B8EF86A"))
	IFilesManager : public IManager
{
	virtual ~IFilesManager() {}

	virtual std::shared_ptr<IFile> Open(cstring _fileName) = 0;
	virtual size_t                 GetFileSize(cstring _fileName) = 0;
	virtual bool                   IsFileExists(cstring _fileName) = 0;

	virtual void RegisterFilesStorage(std::shared_ptr<IFilesStorage> _storage) = 0;
	virtual void UnRegisterFilesStorage(std::shared_ptr<IFilesStorage> _storage) = 0;
};
