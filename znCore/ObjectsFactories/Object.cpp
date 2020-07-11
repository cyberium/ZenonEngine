#include "stdafx.h"

// General
#include "Object.h"

bool Object::operator==(const Object& rhs) const
{
	return m_Guid == rhs.m_Guid;
}
bool Object::operator!=(const Object& rhs) const
{
	return m_Guid != rhs.m_Guid;
}

//
// IObject
//
Guid Object::GetGUID() const
{
	return m_Guid;
}
std::string Object::GetName() const
{
	//_ASSERT(m_ClassName.empty() == false);
	return m_Name;
}
void Object::SetName(const std::string& Name)
{
	m_Name = Name;
}
std::string Object::GetTypeName() const
{
	//_ASSERT(m_ClassName.empty() == false);
	return m_TypeName;
}
std::string Object::GetClassNameW() const
{
	_ASSERT(m_ClassName.empty() == false);
	return m_ClassName;
}


void Object::Copy(std::shared_ptr<IObject> Destination) const
{
	auto object = std::dynamic_pointer_cast<Object>(Destination);

	if (GetGUID().GetObjectClass() != object->GetGUID().GetObjectClass() || GetGUID().GetObjectType() != object->GetGUID().GetObjectType())
		throw std::exception(("Unable to copy object with different type and class. Source: " + GetGUID().Str() + ", Destination: " + object->GetGUID().Str()).c_str());

	object->m_Name = m_Name;
	object->m_ClassName = m_ClassName;
	object->m_TypeName = m_TypeName;
}


//
// IObjectInternal
//
void Object::SetClassName(const std::string& Name)
{
	m_ClassName = Name;
}
void Object::SetTypeName(const std::string& Name)
{
	m_TypeName = Name;
}



//
// IObjectLoadSave
//
void Object::Load(const std::shared_ptr<IByteBuffer>& Buffer)
{
	_ASSERT(FALSE);
}

void Object::Save(const std::shared_ptr<IByteBuffer>& Buffer) const
{
	_ASSERT(FALSE);
}

void Object::Load(const std::shared_ptr<IXMLReader>& Reader)
{
	_ASSERT(FALSE);
}

void Object::Save(const std::shared_ptr<IXMLWriter>& Writer) const
{
	_ASSERT(FALSE);
}



//
// IObjectPrivate
//
void Object::SetGUID(const Guid& NewGuid)
{
	if (GetGUID().IsEmpty() == false)
		throw std::exception(("Object " + GetGUID().Str() + " already has Guid.").c_str());

	if (NewGuid.IsEmpty())
		throw std::exception("Cannot assign empty GUID for object.");

	if (NewGuid.GetCounter() == 0)
		throw std::exception(("Guid " + NewGuid.Str() + " not fully initialized.").c_str());

	m_Guid = Guid(NewGuid.GetRawValue());

	if (m_Name.empty())
		m_Name = GetClassNameW() + "_id" + std::to_string(GetGUID().GetCounter());

	if (m_ClassName.empty())
		m_ClassName = "class" + std::to_string(GetClass());

	if (m_TypeName.empty())
		m_TypeName = "type" + std::to_string(GetType());
}



Object::Object()
	: m_Guid(0ull)
{
	m_Name = "";
}
Object::Object(ObjectType Factory, ObjectClass Class)
	: m_Guid(0ull)
{
	m_Name = "";
}
Object::~Object()
{}