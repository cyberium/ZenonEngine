#include "stdafx.h"

// Include
#include "SceneFunctional\\SceneNode3D.h"

// General
#include "LightComponent.h"

CLightComponent::CLightComponent(std::shared_ptr<SceneNode3D> OwnerNode)
    : CComponentBase(OwnerNode)
{
}

CLightComponent::~CLightComponent()
{
}

void CLightComponent::AddLight(std::shared_ptr<CLight3D> _light)
{
    LightList::iterator iter = std::find(m_Lights.begin(), m_Lights.end(), _light);
    if (iter == m_Lights.end())
        m_Lights.push_back(_light);
}

void CLightComponent::RemoveLight(std::shared_ptr<CLight3D> _light)
{
    LightList::iterator iter = std::find(m_Lights.begin(), m_Lights.end(), _light);
    if (iter != m_Lights.end())
        m_Lights.erase(iter);
}

const CLightComponent::LightList& CLightComponent::GetLights()
{
    return m_Lights;
}



//
// ISceneNodeComponent
//
bool CLightComponent::Accept(IVisitor & visitor)
{
    bool acceptResult = true;

	// Visit lights
	for (auto light : GetLights())
	{
		light->Accept(visitor);
	}

    return acceptResult;
}
