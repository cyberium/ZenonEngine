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
	return Base3DPass::Visit(Mesh, GeometryPartParams);
}

bool CDebugMaterialPass::Visit(IGeometry * Geometry, const IConstantBuffer* PerObject, const std::unordered_map<SShaderType, std::shared_ptr<IShader>>& ShadersMap, const IMaterial* Material, const SGeometryPartParams& GeometryPartParams)
{
	const MaterialDebug* objMaterial = dynamic_cast<const MaterialDebug*>(Material);
	if (objMaterial == nullptr)
		return false;

	return Base3DPass::Visit(Geometry, PerObject, ShadersMap, Material, GeometryPartParams);
}