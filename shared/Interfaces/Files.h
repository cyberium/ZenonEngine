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

	virtual bool          seek(size_t AbsoluteOffset) = 0;
	virtual bool          seekRelative(intptr_t RelativeOffset) = 0;

	virtual bool          readLine(std::string * String) = 0;
	virtual bool          getText(std::string * String) = 0;
	virtual bool          readBytes(void* Destination, size_t BytesCount) = 0;
	virtual bool          readString(std::string * String) = 0;

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
	: public virtual IByteBuffer
	, public std::enable_shared_from_this<IFile>
{
	virtual ~IFile() {}

	virtual void ChangeExtension(const std::string& NewExtension) = 0;
	virtual bool Save() = 0;
	virtual bool SaveAs(const std::string& FileName) = 0;

	virtual std::string  Name() const = 0;
	virtual std::string  Path() const = 0;
	virtual std::string  Extension() const = 0;

	virtual std::string  Path_Name() const = 0;
	virtual std::string  Name_NoExtension() const = 0;
};

//--

ZN_INTERFACE ZN_API IznFilesStorage
	: public std::enable_shared_from_this<IznFilesStorage>
{
	virtual ~IznFilesStorage() {};

	virtual std::shared_ptr<IFile>   Open(std::string FileName) = 0;
	virtual size_t                   GetSize(std::string FileName) const = 0;
	virtual bool                     IsExists(std::string FileName) const = 0;
};


ZN_INTERFACE ZN_API IznFilesStorageExtended
{
	virtual ~IznFilesStorageExtended() {};

	virtual std::shared_ptr<IFile>    Create(std::string FileName) = 0;
	virtual void                      Delete(std::string FileName) const = 0;
	virtual void                      Save(std::shared_ptr<IFile> File) = 0;
	virtual std::vector<std::string>  GetAllFilesInFolder(std::string Directory, std::string Extension = "") const = 0;
};

//--

enum class ZN_API EFilesStorageType
{
	USERDATA = 0,
	GAMEDATA
};

ZN_INTERFACE ZN_API IFilesManager 
	: public IManager
{
	ZN_OBJECTCLASS(cFilesManager)

	virtual ~IFilesManager() {}

	virtual std::shared_ptr<IFile> Create(std::string FileName) const = 0;
	virtual std::shared_ptr<IFile> Open(std::string FileName, EFileAccessType FileAccessType = EFileAccessType::Read) const = 0;
	virtual void Delete(std::string FileName) const = 0;
	virtual size_t GetFileSize(std::string FileName) const = 0;
	virtual bool IsFileExists(std::string FileName) const = 0;

	virtual void AddStorage(EFilesStorageType FilesStorageType, std::shared_ptr<IznFilesStorage> Storage) = 0;
	virtual void RemoveStorage(std::shared_ptr<IznFilesStorage> Storage) = 0;
	virtual std::shared_ptr<IznFilesStorage> GetStorage(EFilesStorageType FilesStorageType) const = 0;
};
