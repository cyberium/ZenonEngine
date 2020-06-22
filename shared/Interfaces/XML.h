#pragma once

// FORWARD BEGIN
ZN_INTERFACE IFile;
// FORWARD END

ZN_INTERFACE ZN_API IXMLReader
{
	virtual ~IXMLReader() {}

	virtual std::string GetName() const = 0;
	virtual std::string GetValue() const = 0;
	virtual std::string GetAttribute(std::string AttributeName) const = 0;

	virtual std::shared_ptr<IXMLReader> GetChild(std::string ChildName) const = 0;
	virtual std::vector<std::shared_ptr<IXMLReader>> GetChilds() const = 0;
};

ZN_INTERFACE ZN_API IXMLWriter
{
	virtual ~IXMLWriter() {}

	virtual void SetValue(std::string Value) = 0;
	virtual void AddAttribute(std::string AttributeName, std::string AttributeValue) const = 0;
	virtual void AddIntAttribute(std::string AttributeName, int AttributeValue) const = 0;
	virtual void AddFloatAttribute(std::string AttributeName, double AttributeValue) const = 0;

	virtual std::shared_ptr<IXMLWriter> CreateChild(std::string ChildName) const = 0;

	//

	virtual std::shared_ptr<IFile> SaveToFile(std::string Filename) = 0;
};

ZN_INTERFACE ZN_API IXML
{
	virtual ~IXML() {}

	virtual std::shared_ptr<IXMLReader> CreateReader(std::shared_ptr<IFile> File) = 0;
	virtual std::shared_ptr<IXMLWriter> CreateWriter() = 0;
};