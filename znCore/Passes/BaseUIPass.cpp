#include <stdafx.h>

// General
#include "BaseUIPass.h"

BaseUIPass::BaseUIPass(std::shared_ptr<IRenderDevice> RenderDevice, std::shared_ptr<IScene> Scene, std::shared_ptr<IPipelineState> pipeline)
	: CBaseScenePass(RenderDevice, Scene, pipeline)
{
	m_PerObjectData = (PerObjectUI*)_aligned_malloc(sizeof(PerObjectUI), 16);
	m_PerObjectConstantBuffer = GetRenderDevice()->CreateConstantBuffer(PerObjectUI());
}

BaseUIPass::~BaseUIPass()
{
	_aligned_free(m_PerObjectData);
	GetRenderDevice()->DestroyConstantBuffer(m_PerObjectConstantBuffer);
}

//
// IVisitor
//
bool BaseUIPass::VisitUI(ISceneNode* sceneNode)
{
    GetRenderEventArgs()->Node = sceneNode;

	const ICamera* camera = GetRenderEventArgs()->Camera;
	const Viewport* viewport = GetRenderEventArgs()->PipelineState->GetRasterizerState()->GetViewports()[0];
	
	if (viewport)
	{
		sceneNode->UpdateCamera(camera);
		sceneNode->UpdateViewport(viewport);

		m_PerObjectData->Model = sceneNode->GetComponent<ITransformComponent>()->GetWorldTransfom();
		m_PerObjectData->Projection = viewport->GetOrthoMatix();

		SetPerObjectConstantBufferData();

		return true;
	}

	return false;
}

bool BaseUIPass::Visit(IMesh * Mesh, UINT IndexStartLocation, UINT IndexCnt, UINT VertexStartLocation, UINT VertexCnt)
{
	GetRenderEventArgs()->Caller = this;

	return Mesh->Render(GetRenderEventArgs(), m_PerObjectConstantBuffer.get(), IndexStartLocation, IndexCnt, VertexStartLocation, VertexCnt);
}



//
// PerObject functional
//
void BaseUIPass::SetPerObjectConstantBufferData()
{
	m_PerObjectConstantBuffer->Set(m_PerObjectData, sizeof(PerObjectUI));
}

std::shared_ptr<IConstantBuffer> BaseUIPass::GetPerObjectConstantBuffer() const
{
	return m_PerObjectConstantBuffer;
}

void BaseUIPass::BindPerObjectConstantBuffer(std::shared_ptr<IShader> shader)
{
	_ASSERT_EXPR(shader != nullptr, "BaseUIPass::BindPerObjectConstantBuffer: Shader parameter must not be null!");

	shader->GetShaderParameterByName("PerObject")->Set(m_PerObjectConstantBuffer.get());
}