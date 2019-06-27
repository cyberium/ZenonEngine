#pragma once

#include "AbstractPass.h"
#include "../SceneFunctional/UI/SceneUI.h"

class BaseUIPass : public AbstractPass
{
    typedef AbstractPass base;
public:
	                                                BaseUIPass(std::shared_ptr<SceneUI> uiScene, std::shared_ptr<PipelineState> pipeline);
	virtual                                         ~BaseUIPass();

    virtual void                                    Render(RenderEventArgs& e) override;

	// IVisitor
	virtual bool                                    Visit(std::shared_ptr<CUIBaseNode> nodeUI) override;

private:
	std::shared_ptr<SceneUI>                        m_UIScene;
};