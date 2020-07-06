#pragma once

#define UI64LIT(N) UINT64_C(N)

typedef uint16 ObjectType;  // Kind of object (SceneNode, Material, Texture, etc...)
typedef uint32 ObjectClass;    // Class of object (MaterialDebug, MaterialModel, etc...)
typedef uint32 ObjectCounterType;  // Unique ID of object (same with name)

enum ObjectTypes : ObjectType // Interfaces
{
	otObject = 0,

	otSceneNode3D,
	otSceneNodeUI,
	otSceneNodeComponent,

	otBuffer,
	otMaterial,
	otGeometry,
	otModel
};

class Guid
{
public:
	static const Guid Empty;

public:
	explicit Guid(uint64 guid)
		: m_GUID(guid)
	{ }
	Guid(ObjectType hi, ObjectClass entry, ObjectCounterType counter)
		: m_GUID(counter ? uint64(counter) | (uint64(entry) << 24) | (uint64(hi) << 48) : 0)
	{ }

	inline operator uint64() const { return m_GUID; }
	inline uint64 GetRawValue() const { return m_GUID; }
	inline 	bool IsEmpty() const { return m_GUID == 0; }

	inline ObjectType GetObjectType() const { return ObjectType((m_GUID >> 48) & 0x0000FFFF); }
	inline ObjectClass GetObjectClass() const { return ObjectClass((m_GUID >> 24) & UI64LIT(0x0000000000FFFFFF)); }
	inline ObjectCounterType GetCounter()  const { return ObjectCounterType(m_GUID & UI64LIT(0x0000000000FFFFFF)); }
	inline ObjectCounterType GetMaxCounter() const { return ObjectCounterType(0x00FFFFFF); }
	inline std::string Str() const { return "type_" + std::to_string(GetObjectType()) + "_class_" + std::to_string(GetObjectClass()) + "_id_" + std::to_string(GetCounter()); }
	inline const char* CStr() const { return Str().c_str(); }

	inline bool operator!() const { return IsEmpty(); }
	inline bool operator==(const Guid& guid) const { return GetRawValue() == guid.GetRawValue(); }
	inline bool operator!=(const Guid& guid) const { return GetRawValue() != guid.GetRawValue(); }
	inline bool operator< (const Guid& guid) const { return GetRawValue() < guid.GetRawValue(); }

protected:
	inline void Set(uint64 guid) { m_GUID = guid; }
	inline void Clear() { m_GUID = 0; }

private:
	explicit Guid(uint32) = delete;
	explicit Guid(const uint32&) = delete;
	Guid(ObjectType, ObjectCounterType) = delete;
	Guid(ObjectType, ObjectClass, uint64) = delete;
	Guid(ObjectType, uint64) = delete;

private:
	uint64 m_GUID;
};

namespace std
{
	template<>
	struct hash<Guid>
	{
		size_t operator()(const Guid& Guid) const noexcept
		{
			return Guid.GetRawValue();
		}
	};
}
