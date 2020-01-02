#include "stdafx.h"

// General
#include "TextureMaterialPass.h"

// Additional
#include "Materials/MaterialTextured.h"

CTexturedMaterialPass::CTexturedMaterialPass(std::shared_ptr<IRenderDevice> RenderDevice, std::shared_ptr<IScene> Scene, std::shared_ptr<IPipelineState> Pipeline)
	: Base3DPass(RenderDevice, Scene, Pipeline)
{}

CTexturedMaterialPass::~CTexturedMaterialPass()
{}



//
// IVisitor
//
bool CTexturedMaterialPass::Visit(IMesh * Mesh, SGeometryPartParams GeometryPartParams)
{
	std::shared_ptr<const IMaterial> material = Mesh->GetMaterials()[0].Material;

	const MaterialTextured* objMaterial = dynamic_cast<const MaterialTextured*>(material.get());
	if (objMaterial == nullptr)
		return false;

	return Base3DPass::Visit(Mesh, GeometryPartParams);
}

bool CTexturedMaterialPass::Visit(IGeometry * Geometry, const IShader * VertexShader, const SRenderGeometryArgs& RenderGeometryArgs)
{
	std::shared_ptr<const IMaterial> material = RenderGeometryArgs.Material;

	const MaterialTextured* objMaterial = dynamic_cast<const MaterialTextured*>(material.get());
	if (objMaterial == nullptr)
		return false;

	return Base3DPass::Visit(Geometry, VertexShader, RenderGeometryArgs);
}