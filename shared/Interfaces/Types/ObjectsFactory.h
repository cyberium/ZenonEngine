#pragma once

ZN_INTERFACE ZN_API IObjectCreationArgs
{
	virtual ~IObjectCreationArgs() {}
};

ZN_INTERFACE ZN_API IObjectClassCreator
{
	virtual ~IObjectClassCreator() {}

	virtual size_t GetSupportedClassCount() const = 0;
	virtual ObjectClass GetSupportedClassKey(size_t Index) const = 0;
	virtual std::string GetSupportedClassName(size_t Index) const = 0;
	virtual std::shared_ptr<IObject> CreateObject(size_t Index, const Guid& AssignedGuid, const IObjectCreationArgs* ObjectCreationArgs) = 0;
};

ZN_INTERFACE ZN_API IObjectClassFactory
{
	ZN_OBJECTCLASS(cObjectClassFactory)

	virtual ~IObjectClassFactory() {}

	virtual std::unordered_map<ObjectClass, std::shared_ptr<IObjectClassCreator>> GetClassCreators() const = 0;
	virtual std::shared_ptr<IObjectClassCreator> GetClassCreator(ObjectClass ObjectClassKey) const = 0;
	virtual void AddClassCreator(std::shared_ptr<IObjectClassCreator> Creator) = 0;
	virtual void RemoveClassCreator(std::shared_ptr<IObjectClassCreator> Creator) = 0;

	virtual std::string GetTypeName() const = 0;
	virtual ObjectType GetType() const = 0;
	virtual Guid GenerateGuid(ObjectClass ObjectClassKey) = 0;
	virtual std::shared_ptr<IObject> CreateObject(ObjectClass ObjectClassKey, const IObjectCreationArgs* ObjectCreationArgs) = 0;
};

ZN_INTERFACE ZN_API IObjectsFactory 
	: public IManager
{
	ZN_OBJECTCLASS(cObjectsFactory)

	virtual ~IObjectsFactory() {}

	virtual const std::unordered_map<ObjectType, std::shared_ptr<IObjectClassFactory>>& GetClassFactories() const = 0;
	virtual std::shared_ptr<IObjectClassFactory> GetClassFactory(ObjectType ObjectFactoryKey) const = 0;
	virtual void AddClassFactory(std::shared_ptr<IObjectClassFactory> Creator) = 0;
	virtual void RemoveClassFactory(std::shared_ptr<IObjectClassFactory> Creator) = 0;
	virtual std::shared_ptr<IObject> CreateObject(ObjectType ObjectFactoryKey, ObjectClass ObjectClassKey, const IObjectCreationArgs* ObjectCreationArgs) = 0;

	virtual ObjectType GetObjectTypeByObjectClass(ObjectClass Class) = 0;

	virtual ObjectType GetObjectTypeByObjectTypeName(std::string TypeName) = 0;
	virtual std::string GetObjectTypeNameByObjectType(ObjectType ObjectType) = 0;

	virtual std::string GetObjectClassNameByObjectClass(ObjectClass Class) = 0;
	virtual ObjectClass GetObjectClassByObjectClassName(const std::string& ClassName) = 0;

	template<class T>
	inline std::shared_ptr<T> GetClassFactoryCast() const
	{
		return std::dynamic_pointer_cast<T>(GetClassFactory(T::GetSupportedObjectType()));
	}
};
