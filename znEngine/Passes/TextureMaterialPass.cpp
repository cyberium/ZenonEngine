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
bool CTexturedMaterialPass::Visit(IMesh * Mesh, UINT IndexStartLocation, UINT IndexCnt, UINT VertexStartLocation, UINT VertexCnt)
{
	std::shared_ptr<const IMaterial> material = Mesh->GetMaterial();
	const MaterialTextured* materialTextured = dynamic_cast<const MaterialTextured*>(material.get());
	if (materialTextured == nullptr)
		return false;

	return Base3DPass::Visit(Mesh, IndexStartLocation, IndexCnt, VertexStartLocation, VertexCnt);
}
