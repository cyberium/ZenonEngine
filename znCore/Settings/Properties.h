#pragma once


#include "Utils/Convert.h"
#include "PropertyBase.h"


template <typename T>
class CProperty
	: public CPropertyBase
	, public IPropertyT<T>
{
public:
	CProperty()
	{}
	CProperty(std::string Name, std::string Description)
		: CPropertyBase(Name, Description)
	{}
	CProperty(std::string Name, std::string Description, T Value)
		: CPropertyBase(Name, Description)
		, m_Value(Value)
	{}
	virtual ~CProperty()
	{}



	//
	// IProperty
	//
	void Load(const std::shared_ptr<IXMLReader>& Reader)
	{
		if (IsSyntetic())
			return;

		std::string propertyName = Reader->GetName();
		SetName(Reader->GetName());

		std::string propertyValueAsString = Reader->GetValue();
		T propertyValue = StringToValue<T>(propertyValueAsString);
		Set(propertyValue);
	}

	void Save(const std::shared_ptr<IXMLWriter>& Writer) const 
	{
		if (IsSyntetic())
			return;

		std::string propertyName = GetName();
		Writer->SetName(propertyName);

		std::string valueTypeName2 = typeid(T).name();
		Writer->SetStrAttribute(valueTypeName2, "Type2");

		std::string valueAsSting = ValueToString<T>(Get());
		Writer->SetValue(valueAsSting);
	}



	//
	// IPropertyT
	//
	void Set(T Value, bool BlockCallback = false) override
	{
		m_Value = Value;

		if (BlockCallback)
			return;

		if (m_ValueChangedCallback)
			m_ValueChangedCallback(Value);
	}

	T Get() const override
	{
		return m_Value;
	}

	void SetValueChangedCallback(std::function<void(const T&)> ValueChangedCallback) override
	{
		m_ValueChangedCallback = ValueChangedCallback;
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

	// IPropertyT
	void Set(T Value, bool BlockCallback = false) override
	{
		_ASSERT(m_FuncSetter);

		if (BlockCallback)
		{
			auto oldCallback = m_ValueChangedCallback;
			m_ValueChangedCallback = nullptr;
			m_FuncSetter(Value);
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
	void RaiseValueChangedCallback()
	{
		if (m_ValueChangedCallback)
			m_ValueChangedCallback(Get());
	}


private:
	std::function<void(const T&)> m_FuncSetter;
	std::function<T(void)> m_FuncGetter;
};


class CPropertyWrappedColor
	: public CPropertyWrapped<ColorRBG>
{
public:
	CPropertyWrappedColor(std::string Name, std::string Description = "")
		: CPropertyWrapped<ColorRBG>(Name, Description)
	{}
};
