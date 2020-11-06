#pragma once
	
namespace
{
	template <typename T>
	T DoLoadProperty(std::shared_ptr<IXMLReader> Reader, const std::string& Name)
	{
		_ASSERT(false);
		return T();
	}

	template <>
	std::string DoLoadProperty<std::string> (std::shared_ptr<IXMLReader> Reader, const std::string& Name)
	{
		return Reader->GetValue();
	}

	template <>
	float DoLoadProperty<float>(std::shared_ptr<IXMLReader> Reader, const std::string& Name)
	{
		return Reader->GetFloat();
	}

	template <>
	glm::vec2 DoLoadProperty<glm::vec2>(std::shared_ptr<IXMLReader> Reader, const std::string& Name)
	{
		return Reader->GetVec2();
	}

	template <>
	glm::vec3 DoLoadProperty<glm::vec3>(std::shared_ptr<IXMLReader> Reader, const std::string& Name)
	{
		return Reader->GetVec3();
	}

	template <>
	glm::vec4 DoLoadProperty<glm::vec4>(std::shared_ptr<IXMLReader> Reader, const std::string& Name)
	{
		return Reader->GetVec4();
	}


	//
	//
	//

	template <typename T>
	void DoSaveProperty(std::shared_ptr<IXMLWriter> Writer, const std::string& Name, const T& Value)
	{
		_ASSERT(false);
	}

	template <>
	void DoSaveProperty<std::string>(std::shared_ptr<IXMLWriter> Writer, const std::string& Name, const std::string&Value)
	{
		Writer->SetValue(Value);
	}

	template <>
	void DoSaveProperty<float>(std::shared_ptr<IXMLWriter> Writer, const std::string& Name, const float& Value)
	{
		Writer->SetFloat(Value);
	}

	template <>
	void DoSaveProperty<glm::vec2>(std::shared_ptr<IXMLWriter> Writer, const std::string& Name, const glm::vec2& Value)
	{
		Writer->SetVec2(Value);
	}

	template <>
	void DoSaveProperty<glm::vec3>(std::shared_ptr<IXMLWriter> Writer, const std::string& Name, const glm::vec3& Value)
	{
		Writer->SetVec3(Value);
	}

	template <>
	void DoSaveProperty<glm::vec4>(std::shared_ptr<IXMLWriter> Writer, const std::string& Name, const glm::vec4& Value)
	{
		Writer->SetVec4(Value);
	}

}



template <typename T>
class CProperty
	: public IPropertyT<T>
{
public:
	CProperty()
		: m_Name("CPropertyName")
		, m_Description("CPropertyDescription")
		, m_IsSyntetic(false)
	{}
	CProperty(std::string Name, std::string Description)
		: m_Name(Name)
		, m_Description(Description)
		, m_IsSyntetic(false)
	{}
	CProperty(std::string Name, std::string Description, T Value)
		: m_Name(Name)
		, m_Description(Description)
		, m_IsSyntetic(false)
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
	void SetSyntetic(bool Value) override
	{
		m_IsSyntetic = Value;
	}
	bool IsSyntetic() const 
	{
		return m_IsSyntetic;
	}
	void Load(const std::shared_ptr<IXMLReader>& Reader)
	{
		SetName(Reader->GetName());
		//SetDescription(Reader->GetStr("Description"));

		T value = DoLoadProperty<T>(Reader, GetName());
		Set(value);
	}
	void Save(const std::shared_ptr<IXMLWriter>& Writer) const 
	{
		if (m_IsSyntetic)
			return;

		Writer->SetName(GetName());
		//Writer->AddStr(GetDescription(), "Description");

		DoSaveProperty<T>(Writer, GetName(), Get());
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

protected:
	std::string m_Name;
	std::string m_Description;
	bool        m_IsSyntetic;
	T           m_Value;
	std::function<void(const T&)> m_ValueChangedCallback;
};


// ------------------------------------------------------------------------------


template <typename T>
class CPropertyWrapped
	: public CProperty<T>
{
public:
	CPropertyWrapped()
		: CProperty<T>("CPropertyWrappedName", "CPropertyWrappedDescription")
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


// ------------------------------------------------------------------------------


class CPropertiesGroup
	: public IPropertiesGroup
{
public:
	CPropertiesGroup();
	CPropertiesGroup(std::string Name, std::string Description);
	virtual ~CPropertiesGroup();

	// IProperty
	std::string GetName() const override;
	void SetName(const std::string& Name) override;
	std::string GetDescription() const override;
	void SetDescription(const std::string& Description) override;
	void SetSyntetic(bool Value) override;
	bool IsSyntetic() const;
	void Load(const std::shared_ptr<IXMLReader>& Reader) override;
	void Save(const std::shared_ptr<IXMLWriter>& Writer) const override;

	// IPropertiesGroup
	void AddProperty(std::shared_ptr<IProperty> Property) override;
	std::shared_ptr<IProperty> GetProperty(const std::string& PropertyName) override;
	const std::unordered_map<std::string, std::shared_ptr<IProperty>>& GetProperties() const override;

private:
	std::string GetPropertyTypeName(const IProperty* Property) const;
	std::shared_ptr<IProperty> CreatePropetyByType(std::string TypeName);

private:
	std::string m_Name;
	std::string m_Description;
	bool m_IsSyntetic;
	std::unordered_map<std::string, std::shared_ptr<IProperty>> m_Properties;
};
