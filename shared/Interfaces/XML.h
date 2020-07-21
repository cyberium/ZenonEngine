#pragma once

// FORWARD BEGIN
ZN_INTERFACE IFile;
// FORWARD END

ZN_INTERFACE ZN_API IXMLShared
{
	virtual ~IXMLShared() {}

	virtual std::string GetName() const = 0;
	virtual std::string GetValue() const = 0;
	virtual bool IsChildExists(const std::string& ChildName) const = 0;
	virtual bool IsAttributeExists(const std::string& AttributeName) const = 0;
};

ZN_INTERFACE ZN_API IXMLReader
	: public IXMLShared
{
	virtual ~IXMLReader() {}

	virtual bool        IsAttributeExists(const std::string& AttributeName) const = 0;
	virtual std::string GetStrAttribute(const std::string& AttributeName) const = 0;
	virtual int8        GetInt8Attribute(const std::string& AttributeName) const = 0;
	virtual uint8       GetUInt8Attribute(const std::string& AttributeName) const = 0;
	virtual int16       GetInt16Attribute(const std::string& AttributeName) const = 0;
	virtual uint16      GetUInt16Attribute(const std::string& AttributeName) const = 0;
	virtual int32       GetInt32Attribute(const std::string& AttributeName) const = 0;
	virtual uint32      GetUInt32Attribute(const std::string& AttributeName) const = 0;
	virtual int64       GetInt64Attribute(const std::string& AttributeName) const = 0;
	virtual uint64      GetUInt64Attribute(const std::string& AttributeName) const = 0;
	virtual float       GetFloatAttribute(const std::string& AttributeName) const = 0;
	virtual double      GetDoubleAttribute(const std::string& AttributeName) const = 0;
	virtual glm::vec2   GetVec2Attribute(const std::string& AttributeName) const = 0;
	virtual glm::vec3   GetVec3Attribute(const std::string& AttributeName) const = 0;
	virtual glm::vec4   GetVec4Attribute(const std::string& AttributeName) const = 0;
	virtual std::vector<std::string> GetAttributes() const = 0;
	
	// Values
	virtual int8        GetInt8  () const = 0;
	virtual uint8       GetUInt8 () const = 0;
	virtual int16       GetInt16 () const = 0;
	virtual uint16      GetUInt16() const = 0;
	virtual int32       GetInt32 () const = 0;
	virtual uint32      GetUInt32() const = 0;
	virtual int64       GetInt64 () const = 0;
	virtual uint64      GetUInt64() const = 0;
	virtual float       GetFloat () const = 0;
	virtual double      GetDouble() const = 0;
	virtual glm::vec2   GetVec2  () const = 0;
	virtual glm::vec3   GetVec3  () const = 0;
	virtual glm::vec4   GetVec4  () const = 0;
	

	virtual std::shared_ptr<IXMLReader> GetChild(const std::string& ChildName) const = 0;
	virtual std::vector<std::shared_ptr<IXMLReader>> GetChilds() const = 0;
};

ZN_INTERFACE ZN_API IXMLWriter
	: public IXMLShared
{
	virtual ~IXMLWriter() {}

	virtual void SetName(const std::string& Value) = 0;
	virtual void SetStrAttribute(std::string Value, const std::string& AttributeName) = 0;
	virtual void SetInt8Attribute(int8 Value, const std::string& AttributeName) = 0;
	virtual void SetUInt8Attribute(uint8 Value, const std::string& AttributeName) = 0;
	virtual void SetInt16Attribute(int16 Value, const std::string& AttributeName) = 0;
	virtual void SetUInt16Attribute(uint16 Value, const std::string& AttributeName) = 0;
	virtual void SetInt32Attribute(int32 Value, const std::string& AttributeName) = 0;
	virtual void SetUInt32Attribute(uint32 Value, const std::string& AttributeName) = 0;
	virtual void SetInt64Attribute(int64 Value, const std::string& AttributeName) = 0;
	virtual void SetUInt64Attribute(uint64 Value, const std::string& AttributeName) = 0;
	virtual void SetFloatAttribute(float Value, const std::string& AttributeName) = 0;
	virtual void SetDoubleAttribute(double Value, const std::string& AttributeName) = 0;
	virtual void SetVec2Attribute(glm::vec2 Value, const std::string& AttributeName) = 0;
	virtual void SetVec3Attribute(glm::vec3 Value, const std::string& AttributeName) = 0;
	virtual void SetVec4Attribute(glm::vec4 Value, const std::string& AttributeName) = 0;
	virtual std::vector<std::pair<std::string, std::string>> GetAttributes() const = 0;

	// Values
	virtual void SetValue(const std::string& Value) = 0;
	virtual void SetInt8(int8 Value) = 0;
	virtual void SetUInt8(uint8 Value) = 0;
	virtual void SetInt16(int16 Value) = 0;
	virtual void SetUInt16(uint16 Value) = 0;
	virtual void SetInt32(int32 Value) = 0;
	virtual void SetUInt32(uint32 Value) = 0;
	virtual void SetInt64(int64 Value) = 0;
	virtual void SetUInt64(uint64 Value) = 0;
	virtual void SetFloat(float Value) = 0;
	virtual void SetDouble(double Value) = 0;
	virtual void SetVec2(glm::vec2 Value) = 0;
	virtual void SetVec3(glm::vec3 Value) = 0;
	virtual void SetVec4(glm::vec4 Value) = 0;

	virtual std::shared_ptr<IXMLWriter> CreateChild(const std::string& ChildName) = 0;
	virtual void AddChild(const std::shared_ptr<IXMLWriter>& Writer) = 0;
	virtual std::vector<std::pair<std::string, std::shared_ptr<IXMLWriter>>> GetChilds() const = 0;
};

ZN_INTERFACE ZN_API IXML
{
	virtual ~IXML() {}

	virtual std::shared_ptr<IXMLReader> CreateReader(std::shared_ptr<IFile> File) = 0;
	virtual std::shared_ptr<IXMLWriter> CreateWriter() = 0;
	virtual std::shared_ptr<IXMLWriter> CreateWriter(const std::string& NodeName) = 0;
	virtual std::shared_ptr<IFile> SaveWriterToFile(const std::shared_ptr<IXMLWriter>& Writer, const std::string& FileName) = 0;
};