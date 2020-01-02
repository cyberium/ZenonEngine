#include "stdafx.h"

// General
#include "Base3DPass.h"

Base3DPass::Base3DPass(std::shared_ptr<IRenderDevice> RenderDevice, std::shared_ptr<IScene> scene, std::shared_ptr<IPipelineState> pipeline)
    : CBaseScenePass(RenderDevice, scene, pipeline)
{
	m_PerObjectData = (PerObject3D*)_aligned_malloc(sizeof(PerObject3D), 16);
	m_PerObjectConstantBuffer = GetRenderDevice()->CreateConstantBuffer(PerObject3D());
}

Base3DPass::~Base3DPass()
{
	_aligned_free(m_PerObjectData);
	GetRenderDevice()->DestroyConstantBuffer(m_PerObjectConstantBuffer);
}


//
// Inherited from Visitor
//
bool Base3DPass::Visit3D(ISceneNode* sceneNode)
{
	CBaseScenePass::VisitBase(sceneNode);

	const ICamera* camera = GetRenderEventArgs()->Camera;
	//const Viewport* viewport = GetRenderEventArgs()->PipelineState->GetRasterizerState()->GetViewports()[0];

	if (camera)
	{
		m_PerObjectData->Model               = sceneNode->GetWorldTransfom();
		m_PerObjectData->View                = camera->GetViewMatrix();
		m_PerObjectData->Projection          = camera->GetProjectionMatrix();

		SetPerObjectConstantBufferData();

		return true;
	}

	return false;
}

bool Base3DPass::Visit(IMesh * Mesh, SGeometryPartParams GeometryPartParams)
{
	GetRenderEventArgs()->Caller = this;

	return Mesh->Render(GetRenderEventArgs(), m_PerObjectConstantBuffer.get(), GeometryPartParams);
}

bool Base3DPass::Visit(IGeometry * Geometry, const IShader* VertexShader, const SRenderGeometryArgs& RenderGeometryArgs)
{
	GetRenderEventArgs()->Caller = this;

	return Geometry->Render(GetRenderEventArgs(), VertexShader, RenderGeometryArgs.GeometryPartParams);
}



//
// PerObject functional
//
void Base3DPass::SetPerObjectConstantBufferData()
{
	m_PerObjectConstantBuffer->Set(m_PerObjectData, sizeof(PerObject3D));
}

std::shared_ptr<IConstantBuffer> Base3DPass::GetPerObjectConstantBuffer() const
{
	return m_PerObjectConstantBuffer;
}

void Base3DPass::BindPerObjectConstantBuffer(std::shared_ptr<IShader> shader)
{
	_ASSERT_EXPR(shader != nullptr, "Base3DPass::BindPerObjectConstantBuffer: Shader parameter must not be null!");
	shader->GetShaderParameterByName("PerObject")->Set(m_PerObjectConstantBuffer.get());
}
