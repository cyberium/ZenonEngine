#pragma once

ZN_INTERFACE IManager;
ZN_INTERFACE IByteBuffer;

#define UI64LIT(N) UINT64_C(N)

typedef uint16 ObjectFactoryType;  // Kind of object (SceneNode, Material, Texture, etc...)
typedef uint32 ObjectClassType;    // Class of object (MaterialDebug, MaterialModel, etc...)
typedef uint32 ObjectCounterType;  // Unique ID of object (same with name)

enum ObjectFactoryKey : ObjectFactoryType
{
	ofkEmpty = 0x0000,
	ofkObject = 0x0001,

	ofkSceneNode3D = 0x0100,
	ofkSceneNodeUI = 0x0101,

	ofkMaterial = 0x1100,
	ofkModel = 0x1101
};

ZN_INTERFACE ZN_API IObject
{
	virtual ~IObject() {}

	virtual std::string GetName() const = 0;
	virtual std::string GetClassName() const = 0;

	virtual ObjectFactoryType GetFactory() const = 0;
	virtual ObjectClassType GetClass() const = 0;
	virtual bool Is(ObjectClassType ClassType) const = 0;
};

ZN_INTERFACE ZN_API IObjectInternal
{
	virtual ~IObjectInternal() {}

	virtual void SetName(const std::string& Name) = 0;
	virtual void SetClassName(const std::string& Name) = 0;
	virtual void SetGuid(uint64 guid) = 0;
};

ZN_INTERFACE ZN_API	IObjectLoadSave
{
	virtual ~IObjectLoadSave() {}

	virtual void Load(const std::shared_ptr<IByteBuffer>& ByteBuffer) = 0;
	virtual void Save(const std::shared_ptr<IByteBuffer>& ByteBuffer) = 0;
};

class ZN_API Object
	: public IObject
	, public IObjectInternal
{
public:
	class Guid
	{
	public:
		static const Guid Empty;

	public:
		explicit Guid(uint64 guid)
			: m_GUID(guid)
		{ }
		Guid(ObjectFactoryType hi, ObjectClassType entry, ObjectCounterType counter)
			: m_GUID(counter ? uint64(counter) | (uint64(entry) << 24) | (uint64(hi) << 48) : 0)
		{ }

		operator uint64() const { return m_GUID; }
		uint64 GetRawValue() const { return m_GUID; }
		bool IsEmpty() const { return m_GUID == 0; }

		ObjectFactoryType GetFactoryKey() const { return ObjectFactoryType((m_GUID >> 48) & 0x0000FFFF); }
		ObjectClassType GetClass() const { return ObjectClassType((m_GUID >> 24) & UI64LIT(0x0000000000FFFFFF)); }
		ObjectCounterType GetCounter()  const { return ObjectCounterType(m_GUID & UI64LIT(0x0000000000FFFFFF)); }
		ObjectCounterType GetMaxCounter() const { return ObjectCounterType(0x00FFFFFF); }
		std::string Str() const { return "factory_" + std::to_string(GetFactoryKey()) + "_class_" + std::to_string(GetClass()) + "_id_" + std::to_string(GetCounter()); }
		const char* CStr() const { return Str().c_str(); }

		bool operator!() const { return IsEmpty(); }
		bool operator==(const Guid& guid) const { return GetRawValue() == guid.GetRawValue(); }
		bool operator!=(const Guid& guid) const { return GetRawValue() != guid.GetRawValue(); }
		bool operator< (const Guid& guid) const { return GetRawValue() < guid.GetRawValue(); }

	protected:
		void Set(uint64 guid) { m_GUID = guid; }
		void Clear() { m_GUID = 0; }

	private:
		explicit Guid(uint32) = delete;
		explicit Guid(const uint32&) = delete;
		Guid(ObjectFactoryType, ObjectCounterType) = delete;
		Guid(ObjectFactoryType, ObjectClassType, uint64) = delete;
		Guid(ObjectFactoryType, uint64) = delete;

	private:
		uint64 m_GUID;
	};

public:
	/*virtual bool operator==(const Object& rhs) const
	{
		return m_Guid == rhs.m_Guid;
	}
	virtual bool operator!=(const Object& rhs) const
	{
		return m_Guid != rhs.m_Guid;
	}*/

	//
	// IObject
	//
	std::string GetName() const override
	{
		if (m_Name.empty())
			return GetGuid().Str();
		return m_Name;
	}
	std::string GetClassName() const  override
 	{
		if (m_ClassName.empty())
			return "class_" + std::to_string(GetClass());
		return m_ClassName;
	}
	ObjectFactoryType GetFactory() const  override
	{
		return m_Guid.GetFactoryKey();
	}
	ObjectClassType GetClass() const  override
	{
		return m_Guid.GetClass();
	}
	bool Is(ObjectClassType ClassType) const  override
	{
		return GetClass() == ClassType;
	}

	//
	// IObjectInternal
	//
	void SetName(const std::string& Name) override
	{
		//_ASSERT(Name.find(' ') == Name.end() && Name.find('.') == Name.end());
		m_Name = Name;
	}
	void SetClassName(const std::string& Name)  override
	{
		m_ClassName = Name;
	}
	void SetGuid(uint64 guid) override
	{
		if (!m_Guid.IsEmpty())
			throw std::exception(("Object " + GetGuid().Str() + " already has Guid.").c_str());
		m_Guid = Guid(guid);
	}

protected:
	Object()
		: m_Guid(0ull)
	{
		m_Name = "";
	}
	Object(ObjectFactoryType Factory, ObjectClassType Class)
		: m_Guid(0ull)
	{
		m_Name = "";
	}
	virtual ~Object()
	{}

	const Guid& GetGuid() const
	{
		return m_Guid;
	}

private:
	Object(const Object&) = delete;
	Object(Object&&) = delete;
	Object& operator=(const Object&) = delete;
	Object& operator=(Object&&) = delete;

private:
	Guid m_Guid;
	std::string m_Name;
	std::string m_ClassName;
};



namespace std
{
	template<>
	struct hash<Object::Guid>
	{
		size_t operator()(const Object::Guid& Guid) const noexcept
		{
			return Guid.GetRawValue();
		}
	};
}