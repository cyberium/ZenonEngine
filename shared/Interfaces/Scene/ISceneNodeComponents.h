#pragma once

#include "Interfaces/Scene/Types/Light.h"

// FORWARD BEGIN
ZN_INTERFACE IPropertiesGroup;
ZN_INTERFACE IXMLReader;
ZN_INTERFACE IXMLWriter;
// FORWARD END

typedef uint32 ComponentMessageType;


ZN_INTERFACE ZN_API ISceneNodeComponent
	: public virtual IObject
	, public std::enable_shared_from_this<ISceneNodeComponent>
{
	ZN_OBJECTCLASS(cSceneNodeComponent);

	virtual ~ISceneNodeComponent() {}

	// Callbacks
	virtual void OnMessage(const ISceneNodeComponent* FromComponent, ComponentMessageType Message) = 0;

	// Visit functional
	virtual void Update(const UpdateEventArgs& e) = 0;
	virtual void Accept(IVisitor* visitor) = 0;
};
typedef std::unordered_map<ObjectClass, std::shared_ptr<ISceneNodeComponent>> ComponentsMap;

const ComponentMessageType UUID_OnParentChanged = 1;
const ComponentMessageType UUID_OnLocalTransformChanged = 2;
const ComponentMessageType UUID_OnWorldTransformChanged = 3;



//
// PHYSICS BODY COMPONENT 3D
//

ZN_INTERFACE ZN_API IPhysicsBody
{
	virtual ~IPhysicsBody() {}

	virtual glm::vec3 GetPosition() const = 0;
};

ZN_INTERFACE ZN_API IPhysicsComponent
{
	ZN_OBJECTCLASS(cSceneNodePhysicsComponent);

	virtual ~IPhysicsComponent() {}

	virtual glm::vec3 GetPhysicsPosition() const = 0;
};







ZN_INTERFACE ZN_API ITerrain
	: public virtual IObject
{
	virtual ~ITerrain() {}

	virtual std::shared_ptr<IGeometry> GetTerrainGeometry() const = 0;
};




//
// 
//
ZN_INTERFACE ZN_API ITerrainComponent
{
	ZN_OBJECTCLASS(cSceneNodeTerrainComponent);

	virtual ~ITerrainComponent() {}

	virtual std::shared_ptr<ITerrain> GetTerrain() const = 0;
};