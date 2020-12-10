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

	void SetStrAttribute(std::string Value, const std::string& AttributeName) override;
	void SetBoolAttribute(bool Value, const std::string& AttributeName) override;
	void SetIntAttribute(int64 Value, const std::string& AttributeName) override;
	void SetUIntAttribute(uint64 Value, const std::string& AttributeName) override;
	void SetFloatAttribute(float Value, const std::string& AttributeName) override;
	void SetDoubleAttribute(double Value, const std::string& AttributeName) override;
	void SetVec2Attribute(glm::vec2 Value, const std::string& AttributeName) override;
	void SetVec3Attribute(glm::vec3 Value, const std::string& AttributeName) override;
	void SetVec4Attribute(glm::vec4 Value, const std::string& AttributeName) override;
	std::vector<std::pair<std::string, std::string>> GetAttributes() const override;

	// Values
	void SetValue(const std::string& Value) override;
	void SetBool(bool Value) override;
	void SetInt(int64 Value) override;
	void SetUInt(uint64 Value) override;
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
