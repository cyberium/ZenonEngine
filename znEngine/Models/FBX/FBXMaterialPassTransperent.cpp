#include "stdafx.h"

// General
#include "FBXMaterialPassTransperent.h"

// Additional
#include "FBXMaterial.h"

CFBXMaterialPassTransperent::CFBXMaterialPassTransperent(std::shared_ptr<IRenderDevice> RenderDevice, std::shared_ptr<IScene> Scene, std::shared_ptr<IPipelineState> Pipeline)
	: Base3DPass(RenderDevice, Scene, Pipeline)
{}

CFBXMaterialPassTransperent::~CFBXMaterialPassTransperent()
{}



//
// IVisitor
//
bool CFBXMaterialPassTransperent::Visit(IMesh * Mesh, SGeometryPartParams GeometryPartParams)
{
	return Base3DPass::Visit(Mesh, GeometryPartParams);
}

bool CFBXMaterialPassTransperent::Visit(IGeometry * Geometry, const IConstantBuffer* PerObject, const std::unordered_map<SShaderType, std::shared_ptr<IShader>>& ShadersMap, const IMaterial* Material, const SGeometryPartParams& GeometryPartParams)
{
	const CFBXMaterial* objMaterial = dynamic_cast<const CFBXMaterial*>(Material);
	if (objMaterial == nullptr)
		return false;

	//if (objMaterial->GetTexture(0) == nullptr || objMaterial->GetTexture(0)->IsTransparent() == false)
		return false;

	return Base3DPass::Visit(Geometry, PerObject, ShadersMap, Material, GeometryPartParams);
}