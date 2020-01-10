#include "stdafx.h"

// General
#include "MaterialPassOpaque.h"

// Additional
#include "Materials/MaterialModel.h"

CMaterialPassOpaque::CMaterialPassOpaque(std::shared_ptr<IRenderDevice> RenderDevice, std::shared_ptr<IScene> Scene, std::shared_ptr<IPipelineState> Pipeline)
	: BuildRenderListPass(RenderDevice, Scene, Pipeline)
{}

CMaterialPassOpaque::~CMaterialPassOpaque()
{}



//
// IVisitor
//
bool CMaterialPassOpaque::Visit(IMesh * Mesh, SGeometryPartParams GeometryPartParams)
{
	return BuildRenderListPass::Visit(Mesh, GeometryPartParams);
}

bool CMaterialPassOpaque::Visit(IGeometry * Geometry, const IMaterial* Material, SGeometryPartParams GeometryPartParams)
{
	const MaterialModel* objMaterial = dynamic_cast<const MaterialModel*>(Material);
	if (objMaterial == nullptr)
		return false;

	//if (objMaterial->GetTexture(0) == nullptr || objMaterial->GetTexture(0)->IsTransparent() == true)
	//	return false;

	return BuildRenderListPass::Visit(Geometry, Material, GeometryPartParams);
}