#include "stdafx.h"

// General
#include "OBJMaterialPass.h"

// Additional
#include "Materials/AssimpMaterial.h"
#include "Materials/OBJMaterial.h"

COBJMaterialPass::COBJMaterialPass(std::shared_ptr<IRenderDevice> RenderDevice, std::shared_ptr<IScene> Scene, std::shared_ptr<IPipelineState> Pipeline)
	: Base3DPass(RenderDevice, Scene, Pipeline)
{}

COBJMaterialPass::~COBJMaterialPass()
{}



//
// IVisitor
//
bool COBJMaterialPass::Visit(IMesh * Mesh, UINT IndexStartLocation, UINT IndexCnt, UINT VertexStartLocation, UINT VertexCnt)
{
	std::shared_ptr<const IMaterial> material = Mesh->GetMaterial();

	const CAssimpMaterial* objMaterial = dynamic_cast<const CAssimpMaterial*>(material.get());
	if (objMaterial == nullptr)
		return false;

	return Base3DPass::Visit(Mesh, IndexStartLocation, IndexCnt, VertexStartLocation, VertexCnt);
}
