#include "stdafx.h"

// Include
#include "SceneFunctional/Base/SceneNode.h"

// General
#include "LightComponent3D.h"

CLightComponent3D::CLightComponent3D(std::shared_ptr<SceneNode> OwnerNode)
    : CComponentBase(OwnerNode)
{
}

CLightComponent3D::~CLightComponent3D()
{
}

void CLightComponent3D::AddLight(std::shared_ptr<CLight3D> _light)
{
    LightList::iterator iter = std::find(m_Lights.begin(), m_Lights.end(), _light);
    if (iter == m_Lights.end())
        m_Lights.push_back(_light);
}

void CLightComponent3D::RemoveLight(std::shared_ptr<CLight3D> _light)
{
    LightList::iterator iter = std::find(m_Lights.begin(), m_Lights.end(), _light);
    if (iter != m_Lights.end())
        m_Lights.erase(iter);
}

const CLightComponent3D::LightList& CLightComponent3D::GetLights()
{
    return m_Lights;
}



//
// ISceneNodeComponent
//
bool CLightComponent3D::Accept(IVisitor* visitor)
{
    bool acceptResult = true;

	// Visit lights
	for (auto light : GetLights())
	{
		light->Accept(visitor);
	}

    return acceptResult;
}
