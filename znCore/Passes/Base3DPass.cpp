#include "stdafx.h"

// General
#include "Base3DPass.h"

Base3DPass::Base3DPass(IRenderDevice& RenderDevice, std::shared_ptr<IScene> scene)
    : ScenePassPipelined(RenderDevice, scene)
{
	m_PerObjectConstantBuffer = GetRenderDevice().GetObjectsFactory().CreateConstantBuffer(PerObject3D());
}

Base3DPass::~Base3DPass()
{
}


//
// IVisitor
//
bool Base3DPass::Visit(const ISceneNode3D* sceneNode)
{
	const ICameraComponent3D* camera = GetRenderEventArgs().Camera;
	//if (camera)
	//	sceneNode->UpdateCamera(camera);

	// Do check frustum

	const Viewport* viewport = GetRenderEventArgs().PipelineState->GetRasterizerState()->GetViewports()[0];
	//if (viewport)
	//	sceneNode->UpdateViewport(*viewport);

	PerObject3D perObject3D;
	perObject3D.Model = sceneNode->GetWorldTransfom();
	m_PerObjectConstantBuffer->Set(perObject3D);

	auto& perObjectParameter = GetPipeline().GetShaders().at(EShaderType::VertexShader)->GetShaderParameterByName("PerObject");
	if (perObjectParameter.IsValid() && m_PerObjectConstantBuffer != nullptr)
	{
		perObjectParameter.SetConstantBuffer(m_PerObjectConstantBuffer);
		perObjectParameter.Bind();
	}

	return true;
}

bool Base3DPass::Visit(const IModel * Model)
{
	return Model->Render(GetRenderEventArgs());
}

bool Base3DPass::Visit(const IGeometry* Geometry, const IMaterial* Material, SGeometryDrawArgs GeometryDrawArgs)
{
	Material->Bind(GetRenderEventArgs().PipelineState->GetShaders());
	Geometry->Render(GetRenderEventArgs(), GetRenderEventArgs().PipelineState->GetShaders().at(EShaderType::VertexShader).get(), GeometryDrawArgs);
	Material->Unbind(GetRenderEventArgs().PipelineState->GetShaders());

	return true;
}
