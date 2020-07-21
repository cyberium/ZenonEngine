#pragma once

// FORWARD BEGIN
class TiXmlDocument;
class TiXmlElement;
// FORWARD END

class ZN_API CXMLWriter
	: public IXMLWriter
{
public:
	CXMLWriter(const std::string& Name);
	virtual ~CXMLWriter();

	// IXMLShared
	std::string GetName() const override;
	std::string GetValue() const override;
	bool IsChildExists(const std::string& ChildName) const override;
	bool IsAttributeExists(const std::string& AttributeName) const override;

	// IXMLWriter
	void SetName(const std::string& Value) override;


	virtual void SetStrAttribute(std::string Value, const std::string& AttributeName) override;
	virtual void SetInt8Attribute(int8 Value, const std::string& AttributeName) override;
	virtual void SetUInt8Attribute(uint8 Value, const std::string& AttributeName) override;
	virtual void SetInt16Attribute(int16 Value, const std::string& AttributeName) override;
	virtual void SetUInt16Attribute(uint16 Value, const std::string& AttributeName) override;
	virtual void SetInt32Attribute(int32 Value, const std::string& AttributeName) override;
	virtual void SetUInt32Attribute(uint32 Value, const std::string& AttributeName) override;
	virtual void SetInt64Attribute(int64 Value, const std::string& AttributeName) override;
	virtual void SetUInt64Attribute(uint64 Value, const std::string& AttributeName) override;
	virtual void SetFloatAttribute(float Value, const std::string& AttributeName) override;
	virtual void SetDoubleAttribute(double Value, const std::string& AttributeName) override;
	virtual void SetVec2Attribute(glm::vec2 Value, const std::string& AttributeName) override;
	virtual void SetVec3Attribute(glm::vec3 Value, const std::string& AttributeName) override;
	virtual void SetVec4Attribute(glm::vec4 Value, const std::string& AttributeName) override;
	std::vector<std::pair<std::string, std::string>> GetAttributes() const override;

	// Values
	void SetValue(const std::string& Value) override;
	void SetInt8(int8 Value) override;
	void SetUInt8(uint8 Value) override;
	void SetInt16(int16 Value) override;
	void SetUInt16(uint16 Value) override;
	void SetInt32(int32 Value) override;
	void SetUInt32(uint32 Value) override;
	void SetInt64(int64 Value) override;
	void SetUInt64(uint64 Value) override;
	void SetFloat(float Value) override;
	void SetDouble(double Value) override;
	void SetVec2(glm::vec2 Value) override;
	void SetVec3(glm::vec3 Value) override;
	void SetVec4(glm::vec4 Value) override;

	// Childs
	std::shared_ptr<IXMLWriter> CreateChild(const std::string& ChildName) override;
	void AddChild(const std::shared_ptr<IXMLWriter>& Writer) override;
	std::vector< std::pair<std::string, std::shared_ptr<IXMLWriter>>> GetChilds() const override;

private:
	std::string m_Name;
	std::string m_Value;
	std::vector<std::pair<std::string, std::string>> m_Attributes;
	std::vector<std::pair<std::string, std::shared_ptr<IXMLWriter>>> m_Childs;
};
