#include "stdafx.h"

// General
#include "Object.h"



std::string Object::ConvertInputName(const std::string& OriginalName)
{
	/*std::locale loc2("en_US.UTF-8");
	for (const auto& it : OriginalName)
		if (false == std::isalnum(it, loc2))
			throw CException("Object name '%s' is incorrect. Name must contains only characters and digits.");*/
	return OriginalName;
}

std::pair<std::string, ObjectCounterType> Object::GetClearName(std::string DirtyName)
{
	auto chIt = DirtyName.find_last_of('#');
	if (chIt != std::string::npos)
	{
		std::string clearName = DirtyName.substr(0, chIt);
		std::string idNumber = DirtyName.substr(chIt + 1);

		int num;
		if (sscanf_s(idNumber.c_str(), "%d", &num) == 1)
			return std::make_pair(clearName, num);

		return std::make_pair(clearName, 0);
	}

	return std::make_pair(DirtyName, 0);
}


//
// IObject
//
Guid Object::GetGUID() const
{
	return m_Guid;
}

std::string Object::GetObjectTypeName() const
{
	if (m_BaseManager != nullptr)
		return m_BaseManager->GetManager<IObjectsFactory>()->GetObjectTypeNameByObjectType(m_Guid.GetObjectType());

	return "type" + std::to_string(m_Guid.GetObjectType());
}

std::string Object::GetObjectClassName() const
{
	if (m_BaseManager != nullptr)
		return m_BaseManager->GetManager<IObjectsFactory>()->GetObjectClassNameByObjectClass(m_Guid.GetObjectClass());

	return "class" + std::to_string(m_Guid.GetObjectClass());
}

std::string Object::GetName() const
{
	return m_Name;
}

void Object::SetName(const std::string& Name)
{
	m_Name = ConvertInputName(Name);
}



//
// IObjectLoadSave
//
void Object::CopyTo(std::shared_ptr<IObject> Destination) const
{
	if (GetGUID().GetObjectClass() != Destination->GetGUID().GetObjectClass() || GetGUID().GetObjectType() != Destination->GetGUID().GetObjectType())
		throw CException(("Unable to copy object with different type and class. Source: " + GetGUID().Str() + ", Destination: " + Destination->GetGUID().Str()).c_str());
	Destination->SetName(m_Name);
}

void Object::Load(const std::shared_ptr<IByteBuffer>& Buffer)
{
	throw CException("Not implemented");
}

void Object::Save(const std::shared_ptr<IByteBuffer>& Buffer) const
{
	throw CException("Not implemented");
}

void Object::Load(const std::shared_ptr<IXMLReader>& Reader)
{
	if (false == Reader->IsAttributeExists("Name"))
		return;

	auto name = Reader->GetStrAttribute("Name");
	if (name.empty())
		throw CException("XML node '%s' contains 'Name' attribute, but this attribute is empty.", Reader->GetName().c_str());

	auto clearName = GetClearName(name).first;
	if (clearName.empty())
		throw CException("XML node '%s' contains 'Name' attribute with value '%s', but this value is not correct name.", Reader->GetName().c_str(), name.c_str());

	SetName(clearName);
}

void Object::Save(const std::shared_ptr<IXMLWriter>& Writer) const
{
	auto clearName = GetClearName(m_Name).first;
	if (clearName.empty())
		return;

	if (clearName == GetObjectClassName())
		return;

	Writer->SetStrAttribute(clearName, "Name");
}



//
// IObjectPrivate
//
void Object::SetGUID(const Guid& NewGuid)
{
	//if (GetGUID().IsEmpty() == false)
	//	throw std::exception(("Object " + GetGUID().Str() + " already has Guid.").c_str());

	if (NewGuid.IsEmpty())
		throw CException("Cannot assign empty GUID for object.");

	if (NewGuid.GetCounter() == 0)
		throw CException(("Guid " + NewGuid.Str() + " not fully initialized.").c_str());

	m_Guid = Guid(NewGuid.GetRawValue());

	if (m_Name.empty())
		m_Name = GetObjectClassName() + "#" + std::to_string(GetGUID().GetCounter());
}



Object::Object()
	: m_Guid(0ull)
	, m_BaseManager(nullptr)
{}

Object::Object(const IBaseManager& BaseManager)
	: m_Guid(0ull)
	, m_BaseManager(&BaseManager)
{
}

Object::~Object()
{}