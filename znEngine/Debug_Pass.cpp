#include "stdafx.h"

// General
#include "Debug_Pass.h"

Debug_Pass::Debug_Pass(std::shared_ptr<Scene3D> scene, std::shared_ptr<PipelineState> pipeline)
	: BasePass(scene, pipeline)
{}

Debug_Pass::~Debug_Pass()
{}

bool Debug_Pass::Visit(IMesh& mesh, UINT IndexStartLocation, UINT IndexCnt, UINT VertexStartLocation, UINT VertexCnt)
{
	if (mesh.GetType() == SN_TYPE_DEBUG)
	{
		return mesh.Render(GetRenderEventArgs(), GetPerObjectConstantBuffer(), IndexStartLocation, IndexCnt, VertexStartLocation, VertexCnt);
	}

	return false;
}
