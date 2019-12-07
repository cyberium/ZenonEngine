#pragma once

#include "BasePass.h"

class CTexturedMaterialPass : public BasePass
{
	typedef BasePass base;
public:
	CTexturedMaterialPass(std::shared_ptr<IScene> scene, std::shared_ptr<IPipelineState> pipeline);
	virtual ~CTexturedMaterialPass();

	// IVisitor
	bool Visit(IMesh* Mesh, UINT IndexStartLocation = 0, UINT IndexCnt = 0, UINT VertexStartLocation = 0, UINT VertexCnt = 0) override;
};