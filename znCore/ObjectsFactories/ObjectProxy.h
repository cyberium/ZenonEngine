#pragma once

class ZN_API CObjectProxy
	: virtual public IObject
{
public:
	CObjectProxy(std::shared_ptr<IObject> Object);
	virtual ~CObjectProxy();

	// IObject
	Guid GetGUID() const override;
	std::string GetObjectTypeName() const override;
	std::string GetObjectClassName() const override;
	std::string GetName() const override;
	void SetName(const std::string& Name) override;

	const IBaseManager& GetBaseManager() const override final;
	std::shared_ptr<IPropertiesGroup> GetProperties() const override final;

	// IObjectLoadSave
	std::shared_ptr<IObject> Copy() const override;
	void CopyTo(std::shared_ptr<IObject> Destination) const override;
	void Load(const std::shared_ptr<IByteBuffer>& Buffer) override;
	void Save(const std::shared_ptr<IByteBuffer>& Buffer) const override;
	void Load(const std::shared_ptr<IXMLReader>& Reader) override;
	void Save(const std::shared_ptr<IXMLWriter>& Writer) const override;

private:
	std::shared_ptr<IObject> m_Object;
};