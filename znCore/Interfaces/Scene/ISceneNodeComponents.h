#pragma once

typedef uint32 ComponentMessageType;

ZN_INTERFACE OW_ENGINE_API ISceneNodeComponent : public std::enable_shared_from_this<ISceneNodeComponent>
{
	virtual ~ISceneNodeComponent() {}

    // Callbacks
    virtual void OnParentChanged() = 0;
    virtual void OnMessage(std::shared_ptr<ISceneNodeComponent> Component, ComponentMessageType Message) = 0;

    virtual bool Accept(IVisitor* visitor) = 0;
};
typedef std::map<GUID, std::shared_ptr<ISceneNodeComponent>> ComponentsMap;