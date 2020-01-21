#include "stdafx.h"

// General
#include "BuildRenderListPass.h"

#if 0

// Additional
#include <omp.h>

int threadCnt = omp_get_max_threads();
Log::Print("BuildRenderListPass: Threads cnt = '%d'", threadCnt);
m_PerObjectData.resize(threadCnt);
m_PerObjectConstantBuffer.resize(threadCnt);

for (int i = 0; i < threadCnt; i++)
{
	m_PerObjectData[i] = (PerObject3D*)_aligned_malloc(sizeof(PerObject3D), 16);
	*(m_PerObjectData[i]) = PerObject3D();

	m_PerObjectConstantBuffer[i] = GetRenderDevice()->CreateConstantBuffer(PerObject3D());
}


for (int i = 0; i < m_PerObjectConstantBuffer.size(); i++)
{
	_aligned_free(m_PerObjectData[i]);
	GetRenderDevice()->DestroyConstantBuffer(m_PerObjectConstantBuffer[i]);
}

class CRenderDeviceLocker
{
public:
	CRenderDeviceLocker(const IRenderDevice*& RenderDevice)
		: m_RenderDevice(RenderDevice)
	{
		m_RenderDevice->Lock();
	}
	virtual ~CRenderDeviceLocker()
	{
		m_RenderDevice->Unlock();
	}

private:
	const IRenderDevice*& m_RenderDevice;
};


/*

#pragma omp parallel for
	for (int i = 0; i < static_cast<int>(m_RenderList.size()); i++)
	{
		const auto& it = m_RenderList[i];
		int threadNum = omp_get_thread_num();

		m_PerObjectData[threadNum]->Model = it.Node->GetWorldTransfom();
		m_PerObjectData[threadNum]->View = camera->GetViewMatrix();
		m_PerObjectData[threadNum]->Projection = camera->GetProjectionMatrix();

		ShaderMap shadersMap;
		if (it.Material)
			shadersMap = it.Material->GetShaders();
		if (shadersMap.empty())
			shadersMap = GetRenderEventArgs()->PipelineState->GetShaders();

		{
			CRenderDeviceLocker locker(GetRenderDevice());
			SetPerObjectConstantBufferData(threadNum);
		}

		{
			CRenderDeviceLocker locker(GetRenderDevice());
			it.Material->Bind(shadersMap);
			it.Geometry->Render(GetRenderEventArgs(), GetPerObjectConstantBuffer(threadNum), shadersMap, it.Material, it.GeometryPartParams);
			it.Material->Unbind(shadersMap);
		}
	}

	*/



	/*
	std::vector<PerObject3D> perObject;
	for (const auto& it : m_RenderList)
	{
		m_PerObjectData[0]->Model = it.Node->GetWorldTransfom();
		m_PerObjectData[0]->View = camera->GetViewMatrix();
		m_PerObjectData[0]->Projection = camera->GetProjectionMatrix();
		perObject.push_back(*m_PerObjectData[0]);
	}

	if (m_RenderList.size() > 0)
	{
		IStructuredBuffer* buffer = GetRenderDevice()->CreateStructuredBuffer(perObject);

		const auto& geom = m_RenderList[0];

		ShaderMap shadersMap;
		if (geom.Material)
			shadersMap = geom.Material->GetShaders();
		if (shadersMap.empty())
			shadersMap = GetRenderEventArgs()->PipelineState->GetShaders();

		geom.Material->Bind(shadersMap);
		geom.Geometry->RenderInstanced(GetRenderEventArgs(), buffer, shadersMap, geom.Material, geom.GeometryPartParams);
		geom.Material->Unbind(shadersMap);

		GetRenderDevice()->DestroyStructuredBuffer(buffer);
	}
	*/

#endif




BuildRenderListPass::BuildRenderListPass(IRenderDevice* RenderDevice, std::shared_ptr<IScene> Scene)
	: ScenePass(RenderDevice, Scene)
	, m_CurrentSceneNode(nullptr)
{
}

BuildRenderListPass::~BuildRenderListPass()
{
}



//
// BuildRenderListPass
//
const std::vector<BuildRenderListPass::SGeometryElement>& BuildRenderListPass::GetGeometryList() const
{
	return m_GeometryList;
}

const std::vector<BuildRenderListPass::SLightElement>& BuildRenderListPass::GetLightList() const
{
	return m_LightList;
}



//
// IRenderPass
//
void BuildRenderListPass::PreRender(RenderEventArgs & e)
{
	ScenePass::PreRender(e);

	m_GeometryList.clear();
	m_LightList.clear();
}

void BuildRenderListPass::Render(RenderEventArgs & e)
{
	ScenePass::Render(e);
}



//
// IVisitor
//
bool BuildRenderListPass::Visit3D(ISceneNode * node)
{
	m_CurrentSceneNode = node;

	return true;
}

bool BuildRenderListPass::Visit(IMesh * Mesh, SGeometryPartParams GeometryPartParams)
{
	return true;
}

bool BuildRenderListPass::Visit(IGeometry * Geometry, const IMaterial * Material, SGeometryPartParams GeometryPartParams)
{
	m_GeometryList.push_back(SGeometryElement(m_CurrentSceneNode, Geometry, Material, GeometryPartParams));
	return true;
}

bool BuildRenderListPass::Visit(ILightComponent3D * light)
{
	m_LightList.push_back(SLightElement(m_CurrentSceneNode, light));
	return true;
}
