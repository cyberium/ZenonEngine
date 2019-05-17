#pragma once

#include "Passes/BasePass.h"

// A pass that renders the opaque geometry in the scene.
class Debug_Pass : public BasePass
{
public:
	Debug_Pass(std::shared_ptr<Scene3D> scene, std::shared_ptr<PipelineState> pipeline);
	virtual ~Debug_Pass();

    // IVisitor
	bool Visit(std::shared_ptr<IMesh> Mesh, UINT IndexStartLocation = 0, UINT IndexCnt = 0, UINT VertexStartLocation = 0, UINT VertexCnt = 0) override;
};