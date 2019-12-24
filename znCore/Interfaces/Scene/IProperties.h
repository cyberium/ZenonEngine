#pragma once

ZN_INTERFACE OW_ENGINE_API IProperty
{
	virtual ~IProperty() {}

	virtual std::string GetName() const = 0;
	virtual void        SetName(const std::string& Name) = 0;
	virtual std::string GetDescription() const = 0;
	virtual void        SetDescription(const std::string& Description) = 0;
};

template <typename T>
ZN_INTERFACE OW_ENGINE_API IPropertyT : public IProperty
{
	virtual ~IPropertyT() {}

	virtual void Set(T Value) = 0;
	virtual T Get() const = 0;
};

ZN_INTERFACE OW_ENGINE_API IPropertiesGroup : public IProperty
{
	virtual void AddProperty(std::shared_ptr<IProperty> Property) = 0;
	virtual std::shared_ptr<IProperty> GetProperty(const std::string& PropertyName) = 0;
	virtual const std::unordered_map<std::string, std::shared_ptr<IProperty>>& GetProperties() const = 0;

	template<typename T>
	inline std::shared_ptr<IPropertyT<T>> GetPropertyT(const std::string& PropertyName)
	{
		return std::dynamic_pointer_cast<IPropertyT<T>>(GetProperty(PropertyName));
	}
};
