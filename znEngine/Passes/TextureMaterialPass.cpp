#include "stdafx.h"

// General
#include "TextureMaterialPass.h"

// Additional
#include "Materials/MaterialTextured.h"

CTexturedMaterialPass::CTexturedMaterialPass(std::shared_ptr<IScene> scene, std::shared_ptr<IPipelineState> pipeline)
	: BasePass(scene, pipeline)
{
}

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

	return base::Visit(Mesh, IndexStartLocation, IndexCnt, VertexStartLocation, VertexCnt);
}
