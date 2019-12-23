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
bool CFBXMaterialPass::Visit(IMesh * Mesh, UINT IndexStartLocation, UINT IndexCnt, UINT VertexStartLocation, UINT VertexCnt)
{
	std::shared_ptr<const IMaterial> material = Mesh->GetMaterial();
	const CFBXMaterial* materialFBX = dynamic_cast<const CFBXMaterial*>(material.get());
	if (materialFBX == nullptr)
		return false;

	return Base3DPass::Visit(Mesh, IndexStartLocation, IndexCnt, VertexStartLocation, VertexCnt);
}
