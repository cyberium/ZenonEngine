#pragma once

// FORWARD BEGIN
class TiXmlDocument;
class TiXmlElement;
// FORWARD END

class ZN_API CXMLReader
	: public IXMLReader
{
public:
	CXMLReader(const std::string& Name, const std::string& Value, const std::vector<std::pair<std::string, std::string>>& Attributes, const std::vector<std::pair<std::string, std::shared_ptr<IXMLReader>>>& Childs);
	virtual ~CXMLReader();

	// IXMLShared
	std::string GetName() const override;
	std::string GetValue() const override;
	bool IsChildExists(const std::string& ChildName) const override;
	bool IsAttributeExists(const std::string& AttributeName) const override;

	// IXMLReader

	// Attributes
	std::string GetStrAttribute(const std::string& AttributeName) const override;
	bool        GetBoolAttribute(const std::string& AttributeName) const override;
	int64       GetIntAttribute(const std::string& AttributeName) const override;
	uint64      GetUIntAttribute(const std::string& AttributeName) const override;
	float       GetFloatAttribute(const std::string& AttributeName) const override;
	double      GetDoubleAttribute(const std::string& AttributeName) const override;
	glm::vec2   GetVec2Attribute(const std::string& AttributeName) const override;
	glm::vec3   GetVec3Attribute(const std::string& AttributeName) const override;
	glm::vec4   GetVec4Attribute(const std::string& AttributeName) const override;
	std::vector<std::string> GetAttributes() const override;

	// Values
	bool        GetBool() const override;
	int64       GetInt() const override;
	uint64      GetUInt() const override;
	float       GetFloat() const override;
	double      GetDouble() const override;
	glm::vec2   GetVec2() const override;
	glm::vec3   GetVec3() const override;
	glm::vec4   GetVec4() const override;
	
	// Childs
	std::shared_ptr<IXMLReader> GetChild(const std::string& ChildName) const override;
	std::vector<std::shared_ptr<IXMLReader>> GetChilds() const override;


private:
	std::string m_Name;
	std::string m_Value;
	std::vector<std::pair<std::string, std::string>> m_Attributes;
	std::vector<std::pair<std::string, std::shared_ptr<IXMLReader>>> m_Childs;
};
