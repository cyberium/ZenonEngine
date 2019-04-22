#pragma once

#include "Visitor.h"

struct ISceneNodeComponent
{
    // Callbacks
    virtual void OnParentChanged() = 0;

    virtual bool Accept(IVisitor& visitor) = 0;
};