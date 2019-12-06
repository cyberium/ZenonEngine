#pragma once

template<typename T>
class CSettingBase
	: public ISettingT<T>
	, public ISettingInternalT<T>
{
public:
	CSettingBase(T DefaultValue)
		: m_DefaultValue(DefaultValue)
	{}
	virtual ~CSettingBase()
	{}


	// ISetting
	inline const std::string& GetName() const override
	{
		return m_Name;
	}
	inline bool IsDefault() const  override
	{
		return (m_Value == m_DefaultValue);
	}
	inline virtual void OnUpdate() override
	{

	}

	// ISettingT<T>
	inline virtual void Set(T Value) override
	{
		m_Value = Value;

		OnUpdate();
	}
	inline virtual T Get() const  override
	{
		return m_Value;
	}


	// ISettingInternal
	inline void SetName(const std::string& Name)  override
	{
		m_Name = Name;
	}
	inline void ResetToDefault() override
	{
		m_Value = m_DefaultValue;
	}


	// ISettingInternalT<T>
	inline void SetDefaultValue(T Value) override
	{
		m_DefaultValue = Value;
	}

private:
	std::string m_Name;
	T m_Value;
	T m_DefaultValue;
};