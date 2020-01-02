#include "stdafx.h"

// General
#include "TextureDebugPass.h"

// Additional
#include "Materials/MaterialDebug.h"

CDebugMaterialPass::CDebugMaterialPass(std::shared_ptr<IRenderDevice> RenderDevice, std::shared_ptr<IScene> Scene, std::shared_ptr<IPipelineState> Pipeline)
	: Base3DPass(RenderDevice, Scene, Pipeline)
{}

CDebugMaterialPass::~CDebugMaterialPass()
{}



//
// IVisitor
//
bool CDebugMaterialPass::Visit(IMesh * Mesh, SGeometryPartParams GeometryPartParams)
{
	std::shared_ptr<const IMaterial> material = Mesh->GetMaterials()[0].Material;

	const MaterialDebug* objMaterial = dynamic_cast<const MaterialDebug*>(material.get());
	if (objMaterial == nullptr)
		return false;

	return Base3DPass::Visit(Mesh, GeometryPartParams);
}

bool CDebugMaterialPass::Visit(IGeometry * Geometry, const IShader * VertexShader, const SRenderGeometryArgs& RenderGeometryArgs)
{
	std::shared_ptr<const IMaterial> material = RenderGeometryArgs.Material;

	const MaterialDebug* objMaterial = dynamic_cast<const MaterialDebug*>(material.get());
	if (objMaterial == nullptr)
		return false;

	return Base3DPass::Visit(Geometry, VertexShader, RenderGeometryArgs);
}