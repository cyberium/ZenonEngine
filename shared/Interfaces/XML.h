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
	virtual bool        GetBoolAttribute(const std::string& AttributeName) const = 0;
	virtual int64       GetIntAttribute(const std::string& AttributeName) const = 0;
	virtual uint64      GetUIntAttribute(const std::string& AttributeName) const = 0;
	virtual float       GetFloatAttribute(const std::string& AttributeName) const = 0;
	virtual double      GetDoubleAttribute(const std::string& AttributeName) const = 0;
	virtual glm::vec2   GetVec2Attribute(const std::string& AttributeName) const = 0;
	virtual glm::vec3   GetVec3Attribute(const std::string& AttributeName) const = 0;
	virtual glm::vec4   GetVec4Attribute(const std::string& AttributeName) const = 0;
	virtual std::vector<std::string> GetAttributes() const = 0;
	
	// Values
	virtual bool        GetBool  () const = 0;
	virtual int64       GetInt   () const = 0;
	virtual uint64      GetUInt  () const = 0;
	virtual float       GetFloat () const = 0;
	virtual double      GetDouble() const = 0;
	virtual glm::vec2   GetVec2  () const = 0;
	virtual glm::vec3   GetVec3  () const = 0;
	virtual glm::vec4   GetVec4  () const = 0;
	virtual ColorRGB    GetColorRGB() const = 0;
	virtual ColorRGBA   GetColorRGBA() const = 0;
	

	virtual std::shared_ptr<IXMLReader> GetChild(const std::string& ChildName) const = 0;
	virtual std::vector<std::shared_ptr<IXMLReader>> GetChilds() const = 0;
};

ZN_INTERFACE ZN_API IXMLWriter
	: public IXMLShared
{
	virtual ~IXMLWriter() {}

	virtual void SetName(const std::string& Value) = 0;
	virtual void SetStrAttribute(std::string Value, const std::string& AttributeName) = 0;
	virtual void SetBoolAttribute(bool Value, const std::string& AttributeName) = 0;
	virtual void SetIntAttribute(int64 Value, const std::string& AttributeName) = 0;
	virtual void SetUIntAttribute(uint64 Value, const std::string& AttributeName) = 0;
	virtual void SetFloatAttribute(float Value, const std::string& AttributeName) = 0;
	virtual void SetDoubleAttribute(double Value, const std::string& AttributeName) = 0;
	virtual void SetVec2Attribute(glm::vec2 Value, const std::string& AttributeName) = 0;
	virtual void SetVec3Attribute(glm::vec3 Value, const std::string& AttributeName) = 0;
	virtual void SetVec4Attribute(glm::vec4 Value, const std::string& AttributeName) = 0;
	virtual std::vector<std::pair<std::string, std::string>> GetAttributes() const = 0;

	// Values
	virtual void SetValue(const std::string& Value) = 0;
	virtual void SetBool(bool Value) = 0;
	virtual void SetInt(int64 Value) = 0;
	virtual void SetUInt(uint64 Value) = 0;
	virtual void SetFloat(float Value) = 0;
	virtual void SetDouble(double Value) = 0;
	virtual void SetVec2(glm::vec2 Value) = 0;
	virtual void SetVec3(glm::vec3 Value) = 0;
	virtual void SetVec4(glm::vec4 Value) = 0;
	virtual void SetColorRGB(ColorRGB Value) = 0;
	virtual void SetColorRGBA(ColorRGBA Value) = 0;

	virtual std::shared_ptr<IXMLWriter> CreateChild(const std::string& ChildName) = 0;
	virtual void AddChild(const std::shared_ptr<IXMLWriter>& Writer) = 0;
	virtual std::vector<std::pair<std::string, std::shared_ptr<IXMLWriter>>> GetChilds() const = 0;
};
