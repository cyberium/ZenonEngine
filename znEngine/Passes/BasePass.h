#pragma once

#include "AbstractPass.h"

class OW_ENGINE_API BasePass : public AbstractPass
{
    typedef AbstractPass base;
public:
	                                                BasePass(std::shared_ptr<IScene> scene, std::shared_ptr<IPipelineState> pipeline);
	virtual                                         ~BasePass();

	virtual void                                    Render(RenderEventArgs& e) override;

	// IVisitor
	virtual bool                                    Visit(SceneNode3D* node) override;
	virtual bool                                    Visit(IMesh* Mesh, UINT IndexStartLocation = 0, UINT IndexCnt = 0, UINT VertexStartLocation = 0, UINT VertexCnt = 0) override;
private:
	std::shared_ptr<IScene>                         m_Scene;
};