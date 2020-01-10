#pragma once

#include "../ComponentBase.h"
#include "../Light3D.h"

class __declspec(UUID_LightComponent) ZN_API CLightComponent3D
	: public ILightComponent3D
	, public CComponentBase
{
public:
    CLightComponent3D(std::shared_ptr<ISceneNode> OwnerNode);
    virtual ~CLightComponent3D();

    /**
     * Add a light to this scene node.
     */
    virtual void AddLight(std::shared_ptr<CLight3D> Light) override;
    virtual void RemoveLight(std::shared_ptr<CLight3D> Light) override;
    virtual const LightList& GetLights() override;
    
    // ISceneNodeComponent
    virtual bool Accept(IVisitor* visitor) override;

private:
    LightList m_Lights;
};