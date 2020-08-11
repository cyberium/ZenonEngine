#pragma once

#include "Properties.h"

template<typename T>
class CSettingBase
	: public CProperty<T>
	, public ISetting
{
public:
	CSettingBase(std::string Name, std::string Description, T Value)
		: CProperty<T>(Name, Description, Value)
		, m_DefaultValue(Value)
	{}
	virtual ~CSettingBase()
	{}


	// ISetting
	inline bool IsDefault() const  override
	{
		return (m_Value == m_DefaultValue);
	}
	inline void ResetToDefault() override
	{
		Set(m_DefaultValue);
	}

private:
	T m_DefaultValue;
};