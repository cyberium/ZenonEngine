#pragma once

class ZN_API Object
	: virtual public IObject
	, public IObjectPrivate
{
public:
	//static std::string ConvertInputName(const std::string& OriginalName);
	//static std::pair<std::string, ObjectCounterType> GetClearName(std::string DirtyName);

public:
	// IObject
	Guid GetGUID() const override final;
	std::string GetObjectTypeName() const final;
	std::string GetObjectClassName() const final;
	virtual std::string GetName() const override;
	virtual void SetName(const std::string& Name) override;

	const IBaseManager& GetBaseManager() const override final;
	std::shared_ptr<IPropertiesGroup> GetProperties() const override final;

	// IObjectLoadSave
	virtual std::shared_ptr<IObject> Copy() const override;
	virtual void CopyTo(std::shared_ptr<IObject> Destination) const override;
	virtual void Load(const std::shared_ptr<IByteBuffer>& Buffer) override;
	virtual void Save(const std::shared_ptr<IByteBuffer>& Buffer) const override;
	virtual void Load(const std::shared_ptr<IXMLReader>& Reader) override;
	virtual void Save(const std::shared_ptr<IXMLWriter>& Writer) const override;

private:
	// IObjectPrivate
	void SetGUID(const Guid& NewGuid) override;

protected:
	//Object();
	Object(const IBaseManager& BaseManager);
	virtual ~Object();

private:
	Object(const Object&) = delete;
	Object(Object&&) = delete;
	Object& operator=(const Object&) = delete;
	Object& operator=(Object&&) = delete;

private:
	Guid m_Guid;
	std::string m_Name;
	std::shared_ptr<IPropertiesGroup> m_Properties;

	const IBaseManager* m_BaseManager;
};
