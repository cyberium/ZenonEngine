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
		: CProperty(Name, Description)
		, m_Value(Value)
	{}
	virtual ~CProperty()
	{}


	// IProperty
	std::string GetName() const
	{
		return m_Name;
	}
	void SetName(const std::string& Name)
	{
		m_Name = Name;
	}
	std::string GetDescription() const
	{
		return m_Description;
	}
	void SetDescription(const std::string& Description)
	{
		m_Description = Description;
	}


	// IPropertyT
	void Set(T Value)
	{
		m_Value = Value;
	}
	T Get() const
	{
		return m_Value;
	}

private:
	std::string m_Name;
	std::string m_Description;
	T           m_Value;
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
	CPropertyWrapped(std::string Name, std::string Description)
		: m_Name(Name)
		, m_Description(Description)
	{}
	virtual ~CPropertyWrapped()
	{}


	// IProperty
	std::string GetName() const
	{
		return m_Name;
	}
	void SetName(const std::string& Name)
	{
		m_Name = Name;
	}
	std::string GetDescription() const
	{
		return m_Description;
	}
	void SetDescription(const std::string& Description)
	{
		m_Description = Description;
	}


	// IPropertyT
	void Set(T Value)
	{
		_ASSERT(m_FuncSetter);
		m_FuncSetter(Value);
	}
	T Get() const
	{
		_ASSERT(m_FuncGetter);
		return m_FuncGetter();
	}
	void SetValueSetter(std::function<void(const T&)> Function)
	{
		m_FuncSetter = Function;
	}
	void SetValueGetter(std::function<T()> Function)
	{
		m_FuncGetter = Function;
	}

private:
	std::string m_Name;
	std::string m_Description;
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