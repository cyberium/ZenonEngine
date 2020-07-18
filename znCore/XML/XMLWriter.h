#pragma once

// FORWARD BEGIN
class TiXmlDocument;
class TiXmlElement;
// FORWARD END

class ZN_API CXMLWriter
	: public IXMLWriter
{
public:
	CXMLWriter(std::shared_ptr<TiXmlDocument> Document, TiXmlElement* Element);
	virtual ~CXMLWriter();

	// IXMLWriter
	void SetName(const std::string& Value) override;
	void SetValue(const std::string& Value) override;

	void AddStr(std::string Value, const std::string& AttributeName) const override;

	void AddInt8(int8 Value, const std::string& AttributeName) const override;
	void AddUInt8(uint8 Value, const std::string& AttributeName) const override;

	void AddInt16(int16 Value, const std::string& AttributeName) const override;
	void AddUInt16(uint16 Value, const std::string& AttributeName) const override;

	void AddInt32(int32 Value, const std::string& AttributeName) const override;
	void AddUInt32(uint32 Value, const std::string& AttributeName) const override;

	void AddInt64(int64 Value, const std::string& AttributeName) const override;
	void AddUInt64(uint64 Value, const std::string& AttributeName) const override;

	void AddFloat(float Value, const std::string& AttributeName) const override;
	void AddDouble(double Value, const std::string& AttributeName) const override;

	void AddVec3(glm::vec3 Value, const std::string& AttributeName) const override;
	void AddVec4(glm::vec4 Value, const std::string& AttributeName) const override;

	std::shared_ptr<IXMLWriter> CreateChild(const std::string& ChildName) const override;

	//

	std::shared_ptr<IFile> SaveToFile(const std::string& Filename) override;

private:
	std::shared_ptr<TiXmlDocument> m_Document;
	TiXmlElement* m_Element;
};
