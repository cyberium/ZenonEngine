#include "stdafx.h"

// General
#include "CollectLightPass.h"

CCollectLightPass::CCollectLightPass(std::shared_ptr<IRenderDevice> RenderDevice, std::shared_ptr<IScene> Scene)
	: ScenePass(RenderDevice, Scene)
{
}

CCollectLightPass::~CCollectLightPass()
{
	if (m_LightsBuffer != nullptr)
		GetRenderDevice()->DestroyStructuredBuffer(m_LightsBuffer);
}

std::shared_ptr<IStructuredBuffer> CCollectLightPass::GetLightBuffer() const
{
	return m_LightsBuffer;
}

void CCollectLightPass::PreRender(RenderEventArgs& e)
{
	ScenePass::PreRender(e);
	
	m_CollectedLights.clear();
}

void CCollectLightPass::PostRender(RenderEventArgs& e)
{
	if (m_LightsBuffer != nullptr)
		GetRenderDevice()->DestroyStructuredBuffer(m_LightsBuffer);

	if (!m_CollectedLights.empty())
	{
		glm::mat4 viewMatrix = e.Camera->GetViewMatrix();

		// Update the viewspace vectors of the light.
		for (unsigned int i = 0; i < m_CollectedLights.size(); i++)
		{
			// Update the lights so that their position and direction are in view space.
			SLight& light = m_CollectedLights[i];
			light.PositionVS = viewMatrix * glm::vec4(light.PositionWS.xyz(), 1);
			light.DirectionVS = glm::normalize(viewMatrix *  glm::vec4(light.DirectionWS.xyz(), 0));
		}

		m_LightsBuffer = GetRenderDevice()->CreateStructuredBuffer(m_CollectedLights, CPUAccess::Write, false);
	}

	ScenePass::PostRender(e);
}



//
// IVisitor
//
bool CCollectLightPass::Visit(ILightComponent3D * light)
{
	m_CollectedLights.push_back(light->GetLightStruct());

	return true;
}
