#pragma once

// FORWARD BEGIN
ZN_INTERFACE ISceneNode;
// FORWARD END

ZN_INTERFACE ZN_API IProperty
{
	virtual ~IProperty() {}

	virtual std::string GetName() const = 0;
	virtual void        SetName(const std::string& Name) = 0;

	virtual std::string GetDescription() const = 0;
	virtual void        SetDescription(const std::string& Description) = 0;

	virtual void        SetSyntetic(bool Value) = 0;
	virtual bool        IsSyntetic() const = 0;

	virtual void        SetNonCopyable(bool Value) = 0;
	virtual bool        IsNonCopyable() const = 0;

	virtual void        ResetToDefault() = 0;
	virtual bool        IsDefault() const = 0;

	virtual void        FromString(const std::string& String, bool BlockCallback = false) = 0;
	virtual std::string ToString() const = 0;

	virtual void        CopyTo(const std::shared_ptr<IProperty>& Other) const = 0;
	virtual void        Load(const std::shared_ptr<IXMLReader>& Reader) = 0;
	virtual void        Save(const std::shared_ptr<IXMLWriter>& Writer) const = 0;
};

ZN_INTERFACE ZN_API IPropertyAction
	: public virtual IProperty
{
	virtual ~IPropertyAction() {}

	virtual void SetAction(std::function<bool(void)> Action) = 0;
	virtual void SetActionPrecondition(std::function<bool(void)> ActionPrecondition) = 0;
	virtual bool ExecuteAction() = 0;
	virtual bool ExecutePrecondition() = 0;
};


template <typename T>
ZN_INTERFACE ZN_API IPropertyT
	: public virtual IProperty
{
	virtual ~IPropertyT() {}

	virtual void Set(T Value, bool BlockCallback = false) = 0;
	virtual T Get() const = 0;
	virtual T GetDefaultValue() const = 0;

	virtual void SetValueChangedCallback(std::function<void(const T&)> ValueChangedCallback) = 0;
};

ZN_INTERFACE ZN_API IPropertyValueChangedCallback
{
	virtual ~IPropertyValueChangedCallback() {}

	virtual void RaiseValueChangedCallback() = 0;
	virtual void ResetChangedCallback() = 0;
};


ZN_INTERFACE ZN_API IPropertiesGroup
	: public virtual IProperty
{
	virtual ~IPropertiesGroup() {}

	virtual void AddProperty(std::shared_ptr<IProperty> Property) = 0;
	virtual std::shared_ptr<IProperty> GetProperty(const std::string& PropertyName) = 0;
	virtual const std::unordered_map<std::string, std::shared_ptr<IProperty>>& GetProperties() const = 0;

	template<typename T>
	inline std::shared_ptr<IPropertyT<T>> GetPropertyT(const std::string& PropertyName)
	{
		return std::dynamic_pointer_cast<IPropertyT<T>>(GetProperty(PropertyName));
	}
};
