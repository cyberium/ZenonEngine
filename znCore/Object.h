#pragma once

class Object
{
public:
	virtual bool operator==(const Object& rhs) const;

protected:
	// Objects should not be created or destroyed unless explicitly stated by overriding these methods
	Object();

	enum ConstructorType
	{
		NoUUID,
	};
	// Generating UUIDs is expensive. When creating temporary objects, use this constructor.
	Object(ConstructorType type);
	virtual ~Object();

	GUID GetUUID() const;

private:
	Object(const Object&) = delete;
	Object(Object&&) = delete;
	Object& operator=(const Object&) = delete;
	Object& operator=(Object&&) = delete;

	GUID m_UUID;
};