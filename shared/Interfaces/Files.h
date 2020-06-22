#pragma once

enum class ZN_API EFileAccessType
{
	Read = 0,
	Write,
	Any
};

ZN_INTERFACE ZN_API IByteBuffer
{
	virtual ~IByteBuffer() {}

	virtual size_t        getSize() const = 0;
	virtual size_t        getPos() const = 0;
	virtual const uint8*  getData() const = 0;
	virtual const uint8*  getDataFromCurrent() const = 0;
	virtual uint8*        getDataEx() = 0;
	virtual uint8*        getDataFromCurrentEx() = 0;
	virtual bool          isEof() const = 0;

	virtual void          seek(size_t AbsoluteOffset) = 0;
	virtual void          seekRelative(intptr_t RelativeOffset) = 0;

	virtual bool          readLine(std::string * String) = 0;
	virtual bool          readBytes(void* _destination, size_t BytesCount) = 0;
	virtual void          readString(std::string * String) = 0;

	virtual void          writeLine(const std::string& String) = 0;
	virtual void          writeBytes(const void* Source, size_t BytesCount) = 0;
	virtual void          writeDummy(size_t BytesCount) = 0;
	virtual void          insert(size_t Position, const void * DataPtr, size_t DataSize) = 0;
	virtual void          writeString(const std::string& String) = 0;

	template <typename T>
	inline bool read(T* Destination)
	{
		return readBytes(Destination, sizeof(T));
	}

	template <typename T>
	inline void write(const T* Source)
	{
		writeBytes(Source, sizeof(T));
	}
};

//--

ZN_INTERFACE ZN_API IByteBufferEx
{
	virtual ~IByteBufferEx() {}

	virtual void    Allocate(size_t _size) = 0;
	virtual void    SetFilled() = 0;
	virtual void    CopyData(const uint8* _data, size_t _size) = 0;
	virtual uint8*  getDataEx() = 0;
	virtual uint8*  getDataFromCurrentEx() = 0;
};

//--

ZN_INTERFACE ZN_API IFile 
	: public IByteBuffer
{
	virtual ~IFile() {}

	virtual std::string  Name() const = 0;
	virtual std::string  Path() const = 0;
	virtual std::string  Extension() const = 0;
	virtual std::string  Path_Name() const = 0;
};

//--

ZN_INTERFACE ZN_API IFilesStorage
{
	virtual ~IFilesStorage() {};

	virtual std::shared_ptr<IFile>  OpenFile(std::string FileName, EFileAccessType FileAccessType = EFileAccessType::Read) = 0;
	virtual bool                    SaveFile(std::shared_ptr<IFile> File) = 0;
	virtual size_t                  GetFileSize(std::string FileName) = 0;
	virtual bool                    IsFileExists(std::string FileName) = 0;
};

//--

ZN_INTERFACE ZN_API	IFilesStorageEx
{
	enum class ZN_API Priority
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

ZN_INTERFACE ZN_API __declspec(uuid("5DC32EB8-9A63-4FAD-A4BF-81916B8EF86A")) IFilesManager 
	: public IManager
{
	virtual ~IFilesManager() {}

	virtual std::shared_ptr<IFile> Open(std::string FileName, EFileAccessType FileAccessType = EFileAccessType::Read) = 0;
	virtual size_t GetFileSize(std::string FileName) = 0;
	virtual bool IsFileExists(std::string FileName) = 0;

	virtual void AddFilesStorage(std::string StorageName, std::shared_ptr<IFilesStorage> Storage) = 0;
	virtual void RemoveFilesStorage(std::shared_ptr<IFilesStorage> Storage) = 0;
	virtual std::shared_ptr<IFilesStorage> GetFilesStorage(std::string StorageName) const = 0;
};
