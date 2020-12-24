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
	CProperty(std::string Name, std::string Description, T DefaultValue)
		: CPropertiesGroup(Name, Description)
		, m_Value(DefaultValue)
		, m_DefaultValue(DefaultValue)
	{}
	virtual ~CProperty()
	{}

	//
	// IProperty
	//
	void ResetToDefault() override
	{
		Set(GetDefaultValue());
	}

	bool IsDefault() const override
	{
		return Get() == GetDefaultValue();
	}

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

	void CopyTo(const std::shared_ptr<IProperty>& Other) const override
	{
		__super::CopyTo(Other);

		if (IsNonCopyable())
			return;

		std::shared_ptr<IPropertyT<T>> otherAsPropertyT = std::dynamic_pointer_cast<IPropertyT<T>>(Other);
		if (otherAsPropertyT == nullptr)
			return;
			//throw CException("Unable copy '%s' property to '%s' property, because this is IPropertyT<%s>, but other is not.", GetName().c_str(), Other->GetName(), GetPropertyTypeName(this).c_str());

		otherAsPropertyT->Set(Get());
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
		if (IsReadOnly())
			throw CException("Property '%s' is read-only.", GetName().c_str());

		m_Value = Value;

		if (false == BlockCallback)
			RaiseValueChangedCallback();
	}

	T Get() const override
	{
		return m_Value;
	}

	T GetDefaultValue() const override
	{
		return m_DefaultValue;
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
	T m_DefaultValue;
	std::function<void(const T&)> m_ValueChangedCallback;
};




// ------------------------------------------------------------------------------


template <typename T>
class CPropertyWrapped
	: public CProperty<T>
{
public:
	CPropertyWrapped(std::string Name, std::string Description, T DefaultValue)
		: CProperty<T>(Name, Description, DefaultValue)
	{}
	virtual ~CPropertyWrapped()
	{}

	//
	// IProperty
	//
	bool IsReadOnly() const override
	{
		if (m_FuncSetter == nullptr)
			return true;
		return __super::IsReadOnly();
	}

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












class CPropertyWrappedVec2
	: public CPropertyWrapped<glm::vec2>
{
public:
	CPropertyWrappedVec2(std::string Name, std::string Description, glm::vec2 DefaultValue)
		: CPropertyWrapped<glm::vec2>(Name, Description, DefaultValue)
	{
		auto xProperty = MakeShared(CPropertyWrapped<float>, "X", "The 'X' component of Vector2", DefaultValue.x);
		xProperty->SetSyntetic(true);
		xProperty->SetValueSetter(std::bind(&CPropertyWrappedVec2::SetT, this, 0, std::placeholders::_1));
		xProperty->SetValueGetter(std::bind(&CPropertyWrappedVec2::GetT, this, 0));
		AddProperty(xProperty);

		auto yProperty = MakeShared(CPropertyWrapped<float>, "Y", "The 'Y' component of Vector2", DefaultValue.y);
		yProperty->SetSyntetic(true);
		yProperty->SetValueSetter(std::bind(&CPropertyWrappedVec2::SetT, this, 1, std::placeholders::_1));
		yProperty->SetValueGetter(std::bind(&CPropertyWrappedVec2::GetT, this, 1));
		AddProperty(yProperty);
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
	virtual void SetT(int Index, float Value)
	{
		glm::vec2 value = Get();
		value[Index] = Value;
		Set(value);
	}
	virtual float GetT(int Index) const
	{
		return Get()[Index];
	}
};












class CPropertyWrappedVec3
	: public CPropertyWrapped<glm::vec3>
{
public:
	CPropertyWrappedVec3(std::string Name, std::string Description, glm::vec3 DefaultValue)
		: CPropertyWrapped<glm::vec3>(Name, Description, DefaultValue)
	{
		auto xProperty = MakeShared(CPropertyWrapped<float>, "X", "The 'X' component of Vector3", DefaultValue.x);
		xProperty->SetSyntetic(true);
		xProperty->SetValueSetter(std::bind(&CPropertyWrappedVec3::SetT, this, 0, std::placeholders::_1));
		xProperty->SetValueGetter(std::bind(&CPropertyWrappedVec3::GetT, this, 0));
		AddProperty(xProperty);

		auto yProperty = MakeShared(CPropertyWrapped<float>, "Y", "The 'Y' component of Vector3", DefaultValue.y);
		yProperty->SetSyntetic(true);
		yProperty->SetValueSetter(std::bind(&CPropertyWrappedVec3::SetT, this, 1, std::placeholders::_1));
		yProperty->SetValueGetter(std::bind(&CPropertyWrappedVec3::GetT, this, 1));
		AddProperty(yProperty);

		auto zProperty = MakeShared(CPropertyWrapped<float>, "Z", "The 'Z' component of Vector3", DefaultValue.z);
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
	virtual void SetT(int Index, float Value)
	{
		glm::vec3 value = Get();
		value[Index] = Value;
		Set(value);
	}
	virtual float GetT(int Index) const
	{
		return Get()[Index];
	}
};
















class CPropertyWrappedVec4
	: public CPropertyWrapped<glm::vec4>
{
public:
	CPropertyWrappedVec4(std::string Name, std::string Description, glm::vec4 DefaultValue)
		: CPropertyWrapped<glm::vec4>(Name, Description, DefaultValue)
	{
		auto xProperty = MakeShared(CPropertyWrapped<float>, "X", "The 'X' component of Vector4", DefaultValue.x);
		xProperty->SetSyntetic(true);
		xProperty->SetValueSetter(std::bind(&CPropertyWrappedVec4::SetT, this, 0, std::placeholders::_1));
		xProperty->SetValueGetter(std::bind(&CPropertyWrappedVec4::GetT, this, 0));
		AddProperty(xProperty);

		auto yProperty = MakeShared(CPropertyWrapped<float>, "Y", "The 'Y' component of Vector4", DefaultValue.y);
		yProperty->SetSyntetic(true);
		yProperty->SetValueSetter(std::bind(&CPropertyWrappedVec4::SetT, this, 1, std::placeholders::_1));
		yProperty->SetValueGetter(std::bind(&CPropertyWrappedVec4::GetT, this, 1));
		AddProperty(yProperty);

		auto zProperty = MakeShared(CPropertyWrapped<float>, "Z", "The 'Z' component of Vector4", DefaultValue.z);
		zProperty->SetSyntetic(true);
		zProperty->SetValueSetter(std::bind(&CPropertyWrappedVec4::SetT, this, 2, std::placeholders::_1));
		zProperty->SetValueGetter(std::bind(&CPropertyWrappedVec4::GetT, this, 2));
		AddProperty(zProperty);

		auto wProperty = MakeShared(CPropertyWrapped<float>, "W", "The 'W' component of Vector4", DefaultValue.w);
		wProperty->SetSyntetic(true);
		wProperty->SetValueSetter(std::bind(&CPropertyWrappedVec4::SetT, this, 3, std::placeholders::_1));
		wProperty->SetValueGetter(std::bind(&CPropertyWrappedVec4::GetT, this, 3));
		AddProperty(wProperty);
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
	virtual void SetT(int Index, float Value)
	{
		glm::vec4 value = Get();
		value[Index] = Value;
		Set(value);
	}
	virtual float GetT(int Index) const
	{
		return Get()[Index];
	}
};





class CPropertyWrappedQuat
	: public CPropertyWrapped<glm::quat>
{
public:
	CPropertyWrappedQuat(std::string Name, std::string Description, glm::quat DefaultValue)
		: CPropertyWrapped<glm::quat>(Name, Description, DefaultValue)
	{
		auto xProperty = MakeShared(CPropertyWrapped<float>, "X", "The 'X' component of Quaternion", DefaultValue.x);
		xProperty->SetSyntetic(true);
		xProperty->SetValueSetter(std::bind(&CPropertyWrappedQuat::SetT, this, 0, std::placeholders::_1));
		xProperty->SetValueGetter(std::bind(&CPropertyWrappedQuat::GetT, this, 0));
		AddProperty(xProperty);

		auto yProperty = MakeShared(CPropertyWrapped<float>, "Y", "The 'Y' component of Quaternion", DefaultValue.y);
		yProperty->SetSyntetic(true);
		yProperty->SetValueSetter(std::bind(&CPropertyWrappedQuat::SetT, this, 1, std::placeholders::_1));
		yProperty->SetValueGetter(std::bind(&CPropertyWrappedQuat::GetT, this, 1));
		AddProperty(yProperty);

		auto zProperty = MakeShared(CPropertyWrapped<float>, "Z", "The 'Z' component of Quaternion", DefaultValue.z);
		zProperty->SetSyntetic(true);
		zProperty->SetValueSetter(std::bind(&CPropertyWrappedQuat::SetT, this, 2, std::placeholders::_1));
		zProperty->SetValueGetter(std::bind(&CPropertyWrappedQuat::GetT, this, 2));
		AddProperty(zProperty);

		auto wProperty = MakeShared(CPropertyWrapped<float>, "W", "The 'W' component of Quaternion", DefaultValue.w);
		wProperty->SetSyntetic(true);
		wProperty->SetValueSetter(std::bind(&CPropertyWrappedQuat::SetT, this, 3, std::placeholders::_1));
		wProperty->SetValueGetter(std::bind(&CPropertyWrappedQuat::GetT, this, 3));
		AddProperty(wProperty);
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
	virtual void SetT(int Index, float Value)
	{
		glm::quat value = Get();
		value[Index] = Value;
		Set(value);
	}
	virtual float GetT(int Index) const
	{
		return Get()[Index];
	}
};
