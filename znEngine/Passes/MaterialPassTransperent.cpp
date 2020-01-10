#include "stdafx.h"

// General
#include "MaterialPassTransperent.h"

// Additional
#include "Materials/MaterialModel.h"

CMaterialPassTransperent::CMaterialPassTransperent(std::shared_ptr<IRenderDevice> RenderDevice, std::shared_ptr<IScene> Scene, std::shared_ptr<IPipelineState> Pipeline)
	: BuildRenderListPass(RenderDevice, Scene, Pipeline)
{}

CMaterialPassTransperent::~CMaterialPassTransperent()
{}



//
// IVisitor
//
bool CMaterialPassTransperent::Visit(IMesh * Mesh, SGeometryPartParams GeometryPartParams)
{
	return BuildRenderListPass::Visit(Mesh, GeometryPartParams);
}

bool CMaterialPassTransperent::Visit(IGeometry * Geometry, const IMaterial* Material, SGeometryPartParams GeometryPartParams)
{
	const MaterialModel* objMaterial = dynamic_cast<const MaterialModel*>(Material);
	if (objMaterial == nullptr)
		return false;

	//if (objMaterial->GetTexture(0) == nullptr || objMaterial->GetTexture(0)->IsTransparent() == false)
		return false;

	return BuildRenderListPass::Visit(Geometry, Material, GeometryPartParams);
}