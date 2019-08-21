#pragma once

// Forward BEGIN
struct IManager;
// Forward END

struct OW_ENGINE_API ISettingGroup
{
	virtual void InitDefault() = 0;
};

struct OW_ENGINE_API
	__declspec(uuid("9FB1EEC3-16B7-4983-84C5-DDF47B203C6D"))
	ISettings : public IManager
{
	virtual void RegisterBool(const std::string& _name, bool _defaultValue) = 0;
	virtual bool& GetBool(const std::string& _name) = 0;

	virtual void RegisterUint32(const std::string& _name, uint32 _defaultValue) = 0;
	virtual uint32& GetUint32(const std::string& _name) = 0;

	virtual void RegisterString(const std::string& _name, std::string _defaultValue) = 0;
	virtual std::string& GetString(const std::string& _name) = 0;

	virtual void RegisterGroup(GUID _guid, ISettingGroup* _group) = 0;
	virtual ISettingGroup* GetGroup(GUID _guid) = 0;
};