#include "stdafx.h"

// General
#include "Base3DPass.h"

Base3DPass::Base3DPass(IRenderDevice& RenderDevice, std::shared_ptr<IScene> scene)
    : ScenePassPipelined(RenderDevice, scene)
{
	m_PerObjectData = (PerObject3D*)_aligned_malloc(sizeof(PerObject3D), 16);
	m_PerObjectConstantBuffer = GetRenderDevice().GetObjectsFactory().CreateConstantBuffer(PerObject3D());
}

Base3DPass::~Base3DPass()
{
	_aligned_free(m_PerObjectData);
}


//
// IVisitor
//
bool Base3DPass::Visit(ISceneNode3D* sceneNode)
{
	const ICameraComponent3D* camera = GetRenderEventArgs().Camera;
	if (camera)
		sceneNode->UpdateCamera(camera);

	const Viewport* viewport = GetRenderEventArgs().PipelineState->GetRasterizerState()->GetViewports()[0];
	if (viewport)
		sceneNode->UpdateViewport(*viewport);

	m_PerObjectData->Model = sceneNode->GetWorldTransfom();
	m_PerObjectConstantBuffer->Set(m_PerObjectData, sizeof(PerObject3D));

	auto& perObjectParameter = GetPipeline().GetShaders().at(EShaderType::VertexShader)->GetShaderParameterByName("PerObject");
	if (perObjectParameter.IsValid() && m_PerObjectConstantBuffer != nullptr)
	{
		perObjectParameter.SetConstantBuffer(m_PerObjectConstantBuffer);
		perObjectParameter.Bind();
	}

	return true;
}

bool Base3DPass::Visit(IMesh * Mesh, SGeometryPartParams GeometryPartParams)
{
	return Mesh->Render(GetRenderEventArgs(), GeometryPartParams);
}

bool Base3DPass::Visit(IGeometry* Geometry, const IMaterial* Material, SGeometryPartParams GeometryPartParams)
{
	Material->Bind(GetRenderEventArgs().PipelineState->GetShaders());
	bool result = Geometry->Render(GetRenderEventArgs(), GetRenderEventArgs().PipelineState->GetShaders(), Material, GeometryPartParams);
	Material->Unbind(GetRenderEventArgs().PipelineState->GetShaders());

	return result;
}
