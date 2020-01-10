#include "stdafx.h"

// General
#include "CollectLightPass.h"

CCollectLightPass::CCollectLightPass(std::shared_ptr<IRenderDevice> RenderDevice, std::shared_ptr<IScene> Scene)
	: CBaseScenePass(RenderDevice, Scene)
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

void CCollectLightPass::PreRender(RenderEventArgs & e)
{
	CBaseScenePass::PreRender(e);
	
	m_CollectedLights.clear();
}

void CCollectLightPass::PostRender(RenderEventArgs & e)
{
	if (m_LightsBuffer != nullptr)
		GetRenderDevice()->DestroyStructuredBuffer(m_LightsBuffer);

	if (!m_CollectedLights.empty())
	{
		glm::mat4 viewMatrix = e.Camera->GetViewMatrix();

		std::vector<Light> lights;
		lights.resize(m_CollectedLights.size());

		// Update the viewspace vectors of the light.
		for (unsigned int i = 0; i < m_CollectedLights.size(); i++)
		{
			// Update the lights so that their position and direction are in view space.
			Light& light = m_CollectedLights[i]->getLight();
			light.m_PositionVS = viewMatrix * glm::vec4(light.m_PositionWS.xyz(), 1);
			light.m_DirectionVS = glm::normalize(viewMatrix *  glm::vec4(light.m_DirectionWS.xyz(), 0));

			lights[i] = light;
		}

		m_LightsBuffer = GetRenderDevice()->CreateStructuredBuffer(lights, CPUAccess::Write, false);
	}

	CBaseScenePass::PostRender(e);
}

bool CCollectLightPass::Visit(CLight3D * light)
{
	m_CollectedLights.push_back(light);

	return true;
}
