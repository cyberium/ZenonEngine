#pragma once


#include "Utils/Convert.h"
#include "PropertyBase.h"
#include "PropertyGroup.h"

template <typename T>
class CProperty
	: public CPropertiesGroup
	, public IPropertyT<T>
	, public IPropertyValueChangedCallback
{
public:
	CProperty()
	{}
	CProperty(std::string Name, std::string Description = "")
		: CPropertiesGroup(Name, Description)
	{}
	CProperty(std::string Name, std::string Description, T Value)
		: CPropertiesGroup(Name, Description)
		, m_Value(Value)
	{}
	virtual ~CProperty()
	{}

	//
	// IProperty
	//
	void FromString(const std::string& String, bool BlockCallback) override
	{
		T propertyValue = StringToValue<T>(String);
		Set(propertyValue, BlockCallback);
	}

	std::string ToString() const override
	{
		T value = Get();
		return ValueToString<T>(value);
	}

	void Load(const std::shared_ptr<IXMLReader>& Reader) override
	{
		if (IsSyntetic())
			return;

		__super::Load(Reader);

		std::string propertyValueAsString = Reader->GetValue();
		FromString(propertyValueAsString, true);
	}

	void Save(const std::shared_ptr<IXMLWriter>& Writer) const override
	{
		if (IsSyntetic())
			return;

		__super::Save(Writer);

		std::string valueAsSting = ToString();
		Writer->SetValue(valueAsSting);
	}

	//
	// IPropertyT
	//
	void Set(T Value, bool BlockCallback = false) override
	{
		m_Value = Value;

		if (false == BlockCallback)
		{
			if (m_ValueChangedCallback)
				m_ValueChangedCallback(Value);
		}
	}

	T Get() const override
	{
		return m_Value;
	}

	void SetValueChangedCallback(std::function<void(const T&)> ValueChangedCallback) override
	{
		m_ValueChangedCallback = ValueChangedCallback;
	}

	//
	// IPropertyValueChangedCallback
	//
	void RaiseValueChangedCallback() override
	{
		if (m_ValueChangedCallback)
			m_ValueChangedCallback(Get());
	}
	void ResetChangedCallback() override
	{
		m_ValueChangedCallback = nullptr;
	}

protected:
	T m_Value;
	std::function<void(const T&)> m_ValueChangedCallback;
};




// ------------------------------------------------------------------------------


template <typename T>
class CPropertyWrapped
	: public CProperty<T>
{
public:
	CPropertyWrapped()
		: CProperty<T>()
	{}
	CPropertyWrapped(std::string Name, std::string Description = "")
		: CProperty<T>(Name, Description)
	{}
	virtual ~CPropertyWrapped()
	{}



	//
	// IPropertyT
	//
	void Set(T Value, bool BlockCallback = false) override
	{
		_ASSERT(m_FuncSetter);

		if (BlockCallback)
		{
			auto oldCallback = m_ValueChangedCallback;
			m_ValueChangedCallback = nullptr;
			{
				m_FuncSetter(Value);
			}
			_ASSERT_EXPR(m_ValueChangedCallback == nullptr, L"Don't change value changed callback in setter.");
			m_ValueChangedCallback = oldCallback;
		}
		else
		{
			m_FuncSetter(Value);
		}
	}

	T Get() const override
	{
		_ASSERT(m_FuncGetter);
		return m_FuncGetter();
	}

public:
	void SetValueSetter(std::function<void(const T&)> Function)
	{
		m_FuncSetter = Function;
	}

	void SetValueGetter(std::function<T(void)> Function)
	{
		m_FuncGetter = Function;
	}

private:
	std::function<void(const T&)> m_FuncSetter;
	std::function<T(void)> m_FuncGetter;
};



class CPropertyWrappedVec3
	: public CPropertyWrapped<glm::vec3>
{
public:
	CPropertyWrappedVec3(std::string Name, std::string Description = "")
		: CPropertyWrapped<glm::vec3>(Name, Description)
	{
		auto xProperty = MakeShared(CPropertyWrapped<float>, "X");
		xProperty->SetSyntetic(true);
		xProperty->SetValueSetter(std::bind(&CPropertyWrappedVec3::SetT, this, 0, std::placeholders::_1));
		xProperty->SetValueGetter(std::bind(&CPropertyWrappedVec3::GetT, this, 0));
		AddProperty(xProperty);

		auto yProperty = MakeShared(CPropertyWrapped<float>, "Y");
		yProperty->SetSyntetic(true);
		yProperty->SetValueSetter(std::bind(&CPropertyWrappedVec3::SetT, this, 1, std::placeholders::_1));
		yProperty->SetValueGetter(std::bind(&CPropertyWrappedVec3::GetT, this, 1));
		AddProperty(yProperty);

		auto zProperty = MakeShared(CPropertyWrapped<float>, "Z");
		zProperty->SetSyntetic(true);
		zProperty->SetValueSetter(std::bind(&CPropertyWrappedVec3::SetT, this, 2, std::placeholders::_1));
		zProperty->SetValueGetter(std::bind(&CPropertyWrappedVec3::GetT, this, 2));
		AddProperty(zProperty);
	}

	//
	// IPropertyValueChangedCallback
	//
	void RaiseValueChangedCallback() override
	{
		__super::RaiseValueChangedCallback();

		for (const auto& subPropIt : GetProperties())
			if (auto valueChangedCallback = std::dynamic_pointer_cast<IPropertyValueChangedCallback>(subPropIt.second))
				valueChangedCallback->RaiseValueChangedCallback();
	}

private:
	void SetT(int Index, float Value)
	{
		glm::vec3 value = Get();
		value[Index] = Value;
		Set(value);
	}
	float GetT(int Index) const
	{
		return Get()[Index];
	}
};







class CPropertyWrappedColor
	: public CPropertyWrapped<ColorRBG>
{
public:
	CPropertyWrappedColor(std::string Name, std::string Description = "")
		: CPropertyWrapped<ColorRBG>(Name, Description)
	{}
};
