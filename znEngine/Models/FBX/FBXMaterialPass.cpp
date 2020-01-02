#include "stdafx.h"

// General
#include "FBXMaterialPass.h"

// Additional
#include "FBXMaterial.h"

CFBXMaterialPass::CFBXMaterialPass(std::shared_ptr<IRenderDevice> RenderDevice, std::shared_ptr<IScene> Scene, std::shared_ptr<IPipelineState> Pipeline)
	: Base3DPass(RenderDevice, Scene, Pipeline)
{
}

CFBXMaterialPass::~CFBXMaterialPass()
{
}



//
// IVisitor
//
bool CFBXMaterialPass::Visit(IMesh * Mesh, SGeometryPartParams GeometryPartParams)
{
	std::shared_ptr<const IMaterial> material = Mesh->GetMaterials()[0].Material;

	const CFBXMaterial* objMaterial = dynamic_cast<const CFBXMaterial*>(material.get());
	if (objMaterial == nullptr)
		return false;

	return Base3DPass::Visit(Mesh, GeometryPartParams);
}

bool CFBXMaterialPass::Visit(IGeometry * Geometry, const IShader * VertexShader, const SRenderGeometryArgs& RenderGeometryArgs)
{
	std::shared_ptr<const IMaterial> material = RenderGeometryArgs.Material;

	const CFBXMaterial* objMaterial = dynamic_cast<const CFBXMaterial*>(material.get());
	if (objMaterial == nullptr)
		return false;

	if (objMaterial->GetTexture(0) == nullptr || objMaterial->GetTexture(0)->IsTransparent() == false)
		return false;

	return Base3DPass::Visit(Geometry, VertexShader, RenderGeometryArgs);
}