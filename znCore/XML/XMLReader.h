#pragma once

// FORWARD BEGIN
class TiXmlDocument;
class TiXmlElement;
// FORWARD END

class ZN_API CXMLReader
	: public IXMLReader
{
public:
	CXMLReader(std::shared_ptr<TiXmlDocument> Document, const TiXmlElement* Element);
	virtual ~CXMLReader();

	// IXMLReader
	std::string GetName() const override;
	std::string GetValue() const override;
	bool        IsExists(const std::string& AttributeName) const override;
	std::string GetStr(const std::string& AttributeName) const override;
	int8        GetInt8(const std::string& AttributeName = "") const override;
	uint8       GetUInt8(const std::string& AttributeName = "") const override;
	int16       GetInt16(const std::string& AttributeName = "") const override;
	uint16      GetUInt16(const std::string& AttributeName = "") const override;
	int32       GetInt32(const std::string& AttributeName = "") const override;
	uint32      GetUInt32(const std::string& AttributeName = "") const override;
	int64       GetInt64(const std::string& AttributeName) const override;
	uint64      GetUInt64(const std::string& AttributeName) const override;
	float       GetFloat(const std::string& AttributeName) const override;
	double      GetDouble(const std::string& AttributeName) const override;
	glm::vec3   GetVec3(const std::string& AttributeName) const override;
	glm::vec4   GetVec4(const std::string& AttributeName) const override;
	std::shared_ptr<IXMLReader>              GetChild(const std::string& ChildName) const override;
	std::vector<std::shared_ptr<IXMLReader>> GetChilds() const override;

private:
	std::shared_ptr<TiXmlDocument> m_Document;
	const TiXmlElement* m_Element;
};
