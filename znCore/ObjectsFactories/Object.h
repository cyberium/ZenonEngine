#pragma once

class ZN_API Object
	: public IObject
	, public IObjectPrivate
	, public IObjectLoadSave
{
public:
	static std::pair<std::string, ObjectCounterType> GetClearName(std::string DirtyName)
	{
		auto chIt = DirtyName.find_last_of('#');
		if (chIt != std::string::npos)
		{
			std::string clearName = DirtyName.substr(0, chIt);
			std::string idNumber = DirtyName.substr(chIt + 1);

			int num;
			if (sscanf(idNumber.c_str(), "%d", &num) == 1)
			{
				return std::make_pair(clearName, num);
			}

			return std::make_pair(clearName, 0);
		}

		return std::make_pair(DirtyName, 0);
	}

public:
	bool operator==(const Object& rhs) const;
	bool operator!=(const Object& rhs) const;

	virtual std::string GetTypeName() const;
	virtual std::string GetClassNameW() const;

	// IObject
	virtual Guid GetGUID() const override;
	virtual std::string GetName() const override;
	virtual void SetName(const std::string& Name) override;
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
