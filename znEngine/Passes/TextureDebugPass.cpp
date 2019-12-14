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
bool CDebugMaterialPass::Visit(IMesh * Mesh, UINT IndexStartLocation, UINT IndexCnt, UINT VertexStartLocation, UINT VertexCnt)
{
	std::shared_ptr<const IMaterial> material = Mesh->GetMaterial();
	const MaterialDebug* materialDebug = dynamic_cast<const MaterialDebug*>(material.get());
	if (materialDebug == nullptr)
		return false;

	return Base3DPass::Visit(Mesh, IndexStartLocation, IndexCnt, VertexStartLocation, VertexCnt);
}
