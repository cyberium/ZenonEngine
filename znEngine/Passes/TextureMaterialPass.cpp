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
	return Base3DPass::Visit(Mesh, GeometryPartParams);
}

bool CTexturedMaterialPass::Visit(IGeometry * Geometry, const IConstantBuffer* PerObject, const std::unordered_map<SShaderType, std::shared_ptr<IShader>>& ShadersMap, const IMaterial* Material, const SGeometryPartParams& GeometryPartParams)
{
	const MaterialTextured* objMaterial = dynamic_cast<const MaterialTextured*>(Material);
	if (objMaterial == nullptr)
		return false;

	return Base3DPass::Visit(Geometry, PerObject, ShadersMap, Material, GeometryPartParams);
}