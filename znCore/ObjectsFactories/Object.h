#pragma once

class ZN_API Object
	: public IObject
	, public IObjectPrivate
	, public IObjectLoadSave
{
public:
	bool operator==(const Object& rhs) const;
	bool operator!=(const Object& rhs) const;

	// IObject
	virtual Guid GetGUID() const override;
	virtual std::string GetName() const override;
	virtual void SetName(const std::string& Name) override;
	virtual std::string GetTypeName() const override;
	virtual std::string GetClassNameW() const override;
	virtual void Copy(std::shared_ptr<IObject> Destination) const;

	// IObjectLoadSave
	virtual void Load(const std::shared_ptr<IByteBuffer>& Buffer) override;
	virtual void Save(const std::shared_ptr<IByteBuffer>& Buffer) const override;
	virtual void Load(const std::shared_ptr<IXMLReader>& Reader) override;
	virtual void Save(const std::shared_ptr<IXMLWriter>& Writer) const override;

private:
	// IObjectPrivate
	void SetGUID(const IBaseManager* BaseManager, const Guid& NewGuid) override;

protected:
	Object();
	Object(ObjectType Factory, ObjectClass Class);
	virtual ~Object();

private:
	Object(const Object&) = delete;
	Object(Object&&) = delete;
	Object& operator=(const Object&) = delete;
	Object& operator=(Object&&) = delete;

private:
	Guid m_Guid;
	std::string m_Name;

	const IBaseManager* m_BaseManager;
};
