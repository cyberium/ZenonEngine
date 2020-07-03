#pragma once

// FORWARD BEGIN
ZN_INTERFACE ISceneNode3D;
// FORWARD END

ZN_INTERFACE ZN_API IProperty
{
	virtual ~IProperty() {}

	virtual std::string GetName() const = 0;
	virtual void        SetName(const std::string& Name) = 0;
	virtual std::string GetDescription() const = 0;
	virtual void        SetDescription(const std::string& Description) = 0;
};

ZN_INTERFACE ZN_API IPropertyAction
	: public IProperty
{
	virtual ~IPropertyAction() {}

	virtual void        SetAction(std::function<bool(void)> Action) = 0;
	virtual void        ExecuteAction() = 0;
};

template <typename T>
ZN_INTERFACE ZN_API IPropertyT
	: public IProperty
{
	virtual ~IPropertyT() {}

	virtual void Set(T Value, bool BlockCallback = false) = 0;
	virtual T Get() const = 0;

	virtual void SetValueChangedCallback(std::function<void(const T&)> ValueChangedCallback) = 0;
};

ZN_INTERFACE ZN_API IPropertiesGroup
	: public IProperty
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
