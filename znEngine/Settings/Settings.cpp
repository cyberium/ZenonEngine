#include "stdafx.h"

// General
#include "Settings.h"

CSettings::CSettings(IBaseManager& BaseManager)
	: m_BaseManager(BaseManager)
{
	m_RootGroup = MakeShared(CPropertiesGroup, "Settings", "");
}

CSettings::~CSettings()
{
	m_BaseManager.RemoveManager<ISettings>();
}



//
// ISettings
//
void CSettings::AddGroup(std::shared_ptr<IPropertiesGroup> Group)
{
	m_RootGroup->AddProperty(Group);
}

std::shared_ptr<IPropertiesGroup> CSettings::GetGroup(const std::string& GroupName)
{
	auto property = m_RootGroup->GetProperty(GroupName);
	if (property == nullptr)
		throw CException("Setting group '%s' not found.", GroupName.c_str());
	return std::dynamic_pointer_cast<IPropertiesGroup>(property);
}

void CSettings::Load()
{
	try
	{
		std::string settingsFile = Utils::LoadLocalFile("settings.xml");

		CXMLManager xml(m_BaseManager);
		auto reader = xml.CreateReaderFromString(settingsFile);
		m_RootGroup->Load(reader->GetChild("Settings"));
	}
	catch (const CException& e)
	{
		Log::Error("Unable to load settings file.");
		Log::Error("--->%s", e.MessageCStr());
	}
}

void CSettings::Save()
{
	try
	{
		CXMLManager xml(m_BaseManager);

		auto xmlDocument = xml.CreateDocument();
		auto settingsWriter = xmlDocument->CreateChild("Settings");
		m_RootGroup->Save(settingsWriter);
		Utils::SaveToLocalFile("settings.xml", xml.SaveWriterToString(xmlDocument));

	}
	catch (const CException& e)
	{
		Log::Error("Unable to save settings file.");
		Log::Error("--->%s", e.MessageCStr());
	}
}
