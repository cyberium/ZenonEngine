#pragma once

template <typename T>
class CProperty
	: public IPropertyT<T>
{
public:
	CProperty()
		: m_Name("CPropertyName")
		, m_Description("CPropertyDescription")
	{}
	CProperty(std::string Name, std::string Description)
		: m_Name(Name)
		, m_Description(Description)
	{}
	CProperty(std::string Name, std::string Description, T Value)
		: m_Name(Name)
		, m_Description(Description)
		, m_Value(Value)
	{}
	virtual ~CProperty()
	{}


	// IProperty
	std::string GetName() const override
	{
		return m_Name;
	}
	void SetName(const std::string& Name) override
	{
		m_Name = Name;
	}
	std::string GetDescription() const override
	{
		return m_Description;
	}
	void SetDescription(const std::string& Description) override
	{
		m_Description = Description;
	}


	// IPropertyT
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

private:
	std::string m_Name;
	std::string m_Description;
	T           m_Value;

	std::function<void(const T&)> m_ValueChangedCallback;
};


// ------------------------------------------------------------------------------


template <typename T>
class CPropertyWrapped
	: public IPropertyT<T>
{
public:
	CPropertyWrapped()
		: m_Name("CPropertyWrappedName")
		, m_Description("CPropertyWrappedDescription")
	{}
	CPropertyWrapped(std::string Name, std::string Description = "")
		: m_Name(Name)
		, m_Description(Description)
	{}
	virtual ~CPropertyWrapped()
	{}


	// IProperty
	std::string GetName() const override
	{
		return m_Name;
	}
	void SetName(const std::string& Name) override
	{
		m_Name = Name;
	}
	std::string GetDescription() const override
	{
		return m_Description;
	}
	void SetDescription(const std::string& Description) override
	{
		m_Description = Description;
	}


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
	void SetValueChangedCallback(std::function<void(const T&)> ValueChangedCallback) override
	{
		m_ValueChangedCallback = ValueChangedCallback;
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
	std::string m_Name;
	std::string m_Description;
	std::function<void(const T&)> m_ValueChangedCallback;

	std::function<void(const T&)> m_FuncSetter;
	std::function<T(void)> m_FuncGetter;
};


// ------------------------------------------------------------------------------


class CPropertiesGroup
	: public IPropertiesGroup
{
public:
	CPropertiesGroup();
	CPropertiesGroup(std::string Name, std::string Description);
	virtual ~CPropertiesGroup();


	// IProperty
	std::string GetName() const;
	void SetName(const std::string& Name);
	std::string GetDescription() const;
	void SetDescription(const std::string& Description);


	// IPropertiesGroup
	void AddProperty(std::shared_ptr<IProperty> Property) override;
	std::shared_ptr<IProperty> GetProperty(const std::string& PropertyName) override;
	const std::unordered_map<std::string, std::shared_ptr<IProperty>>& GetProperties() const override;

private:
	std::string m_Name;
	std::string m_Description;
	std::unordered_map<std::string, std::shared_ptr<IProperty>> m_Properties;
};