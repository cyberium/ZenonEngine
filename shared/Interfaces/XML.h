#pragma once

// FORWARD BEGIN
ZN_INTERFACE IFile;
// FORWARD END

ZN_INTERFACE ZN_API IXMLReader
{
	virtual ~IXMLReader() {}

	virtual std::string GetName  () const = 0;
	virtual std::string GetValue () const = 0;

	virtual bool        IsExists (const std::string& AttributeName = "") const = 0;
	virtual std::string GetStr   (const std::string& AttributeName = "") const = 0;
	virtual int8        GetInt8  (const std::string& AttributeName = "") const = 0;
	virtual uint8       GetUInt8 (const std::string& AttributeName = "") const = 0;
	virtual int16       GetInt16 (const std::string& AttributeName = "") const = 0;
	virtual uint16      GetUInt16(const std::string& AttributeName = "") const = 0;
	virtual int32       GetInt32 (const std::string& AttributeName = "") const = 0;
	virtual uint32      GetUInt32(const std::string& AttributeName = "") const = 0;
	virtual int64       GetInt64 (const std::string& AttributeName = "") const = 0;
	virtual uint64      GetUInt64(const std::string& AttributeName = "") const = 0;
	virtual float       GetFloat (const std::string& AttributeName = "") const = 0;
	virtual double      GetDouble(const std::string& AttributeName = "") const = 0;
	virtual glm::vec3   GetVec3  (const std::string& AttributeName = "") const = 0;
	virtual glm::vec4   GetVec4  (const std::string& AttributeName = "") const = 0;
	virtual std::shared_ptr<IXMLReader>              GetChild(const std::string& ChildName) const = 0;
	virtual std::vector<std::shared_ptr<IXMLReader>> GetChilds() const = 0;
};

ZN_INTERFACE ZN_API IXMLWriter
{
	virtual ~IXMLWriter() {}

	virtual void SetName(const std::string& Value) = 0;
	virtual void SetValue(const std::string& Value) = 0;

	virtual void AddStr(std::string Value, const std::string& AttributeName = "") const = 0;

	virtual void AddInt8(int8 Value, const std::string& AttributeName = "") const = 0;
	virtual void AddUInt8(uint8 Value, const std::string& AttributeName = "") const = 0;

	virtual void AddInt16(int16 Value, const std::string& AttributeName = "") const = 0;
	virtual void AddUInt16(uint16 Value, const std::string& AttributeName = "") const = 0;

	virtual void AddInt32(int32 Value, const std::string& AttributeName = "") const = 0;
	virtual void AddUInt32(uint32 Value, const std::string& AttributeName = "") const = 0;

	virtual void AddInt64(int64 Value, const std::string& AttributeName = "") const = 0;
	virtual void AddUInt64(uint64 Value, const std::string& AttributeName = "") const = 0;

	virtual void AddFloat(float Value, const std::string& AttributeName = "") const = 0;
	virtual void AddDouble(double Value, const std::string& AttributeName = "") const = 0;

	virtual void AddVec3(glm::vec3 Value, const std::string& AttributeName = "") const = 0;
	virtual void AddVec4(glm::vec4 Value, const std::string& AttributeName = "") const = 0;

	virtual std::shared_ptr<IXMLWriter> CreateChild(const std::string& ChildName) const = 0;

	//

	virtual std::shared_ptr<IFile> SaveToFile(const std::string& Filename) = 0;
};

ZN_INTERFACE ZN_API IXML
{
	virtual ~IXML() {}

	virtual std::shared_ptr<IXMLReader> CreateReader(std::shared_ptr<IFile> File) = 0;
	virtual std::shared_ptr<IXMLWriter> CreateWriter() = 0;
};