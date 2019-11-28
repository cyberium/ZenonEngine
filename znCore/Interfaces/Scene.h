#pragma once

#include "Visitor.h"

typedef uint32 ComponentMessageType;

struct OW_ENGINE_API ISceneNodeComponent : public std::enable_shared_from_this<ISceneNodeComponent>
{
    // Callbacks
    virtual void OnParentChanged() = 0;
    virtual void OnMessage(std::shared_ptr<ISceneNodeComponent> Component, ComponentMessageType Message) = 0;


    virtual bool Accept(IVisitor* visitor) = 0;
};