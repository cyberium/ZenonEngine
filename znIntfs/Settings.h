#pragma once

// Forward BEGIN
struct IManager;
// Forward END

struct ISettingGroup
{
	virtual void InitDefault() = 0;
};

struct 
	__declspec(uuid("9FB1EEC3-16B7-4983-84C5-DDF47B203C6D"))
	ISettings : public IManager
{
	virtual void RegisterBool(cstring _name, bool _defaultValue) = 0;
	virtual bool& GetBool(cstring _name) = 0;

	virtual void RegisterUint32(cstring _name, uint32 _defaultValue) = 0;
	virtual uint32& GetUint32(cstring _name) = 0;

	virtual void RegisterString(cstring _name, std::string _defaultValue) = 0;
	virtual std::string& GetString(cstring _name) = 0;

	virtual void RegisterGroup(GUID _guid, ISettingGroup* _group) = 0;
	virtual ISettingGroup* GetGroup(GUID _guid) = 0;
};