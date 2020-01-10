#include "stdafx.h"

// General
#include "FBXMaterialPassOpaque.h"

// Additional
#include "FBXMaterial.h"

CFBXMaterialPassOpaque::CFBXMaterialPassOpaque(std::shared_ptr<IRenderDevice> RenderDevice, std::shared_ptr<IScene> Scene, std::shared_ptr<IPipelineState> Pipeline)
	: Base3DPass(RenderDevice, Scene, Pipeline)
{}

CFBXMaterialPassOpaque::~CFBXMaterialPassOpaque()
{}



//
// IVisitor
//
bool CFBXMaterialPassOpaque::Visit(IMesh * Mesh, SGeometryPartParams GeometryPartParams)
{
	return Base3DPass::Visit(Mesh, GeometryPartParams);
}

bool CFBXMaterialPassOpaque::Visit(IGeometry * Geometry, const IConstantBuffer* PerObject, const std::unordered_map<SShaderType, std::shared_ptr<IShader>>& ShadersMap, const IMaterial* Material, const SGeometryPartParams& GeometryPartParams)
{
	const MaterialModel* objMaterial = dynamic_cast<const MaterialModel*>(Material);
	if (objMaterial == nullptr)
		return false;

	//if (objMaterial->GetTexture(0) == nullptr || objMaterial->GetTexture(0)->IsTransparent() == true)
	//	return false;

	return Base3DPass::Visit(Geometry, PerObject, ShadersMap, Material, GeometryPartParams);
}