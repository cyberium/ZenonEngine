#pragma once

#include "BaseScenePass.h"

// FORWARD BEGIN
class SceneNode3D;
// FORWARD END

class OW_ENGINE_API Base3DPass : public CBaseScenePass
{
public:
	Base3DPass(std::shared_ptr<IRenderDevice> RenderDevice, std::shared_ptr<IScene> scene, std::shared_ptr<IPipelineState> pipeline);
	virtual ~Base3DPass();

	// IVisitor
	virtual bool                                    Visit3D(ISceneNode* node) override;
	virtual bool                                    Visit(IMesh* Mesh, UINT IndexStartLocation = 0, UINT IndexCnt = 0, UINT VertexStartLocation = 0, UINT VertexCnt = 0) override;
};