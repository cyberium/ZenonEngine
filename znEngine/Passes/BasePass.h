#pragma once

#include "AbstractPass.h"
#include "../SceneFunctional/3D/Scene3D.h"

class OW_ENGINE_API BasePass : public AbstractPass
{
    typedef AbstractPass base;
public:
	                                                BasePass(std::shared_ptr<Scene3D> scene, std::shared_ptr<PipelineState> pipeline);
	virtual                                         ~BasePass();

	virtual void                                    Render(RenderEventArgs& e) override;

	// IVisitor
	virtual bool                                    Visit(SceneNode3D* node) override;

private:
	std::shared_ptr<Scene3D>                        m_Scene;
};