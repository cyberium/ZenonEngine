#pragma once

class ZN_API Object
	: virtual public IObject
	, virtual public IObjectLoadSave
	, public IObjectPrivate
{
public:
	//static std::string ConvertInputName(const std::string& OriginalName);
	//static std::pair<std::string, ObjectCounterType> GetClearName(std::string DirtyName);

public:
	// IObject
	virtual Guid GetGUID() const override;
	virtual std::string GetObjectTypeName() const;
	virtual std::string GetObjectClassName() const;
	virtual std::string GetName() const override;
	virtual void SetName(const std::string& Name) override;

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
	Object();
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

	const IBaseManager* m_BaseManager;
};
