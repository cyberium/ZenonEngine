#pragma once

#include "AbstractPass.h"
#include "../SceneFunctional/UI/SceneUI.h"

class BaseUIPass : public AbstractPass
{
    typedef AbstractPass base;
public:
	                                                BaseUIPass(std::shared_ptr<SceneUI> uiScene, std::shared_ptr<PipelineState> pipeline);
	virtual                                         ~BaseUIPass();

    virtual void                                    PreRender(RenderEventArgs& e) override;
    virtual void                                    Render(RenderEventArgs& e) override;
    virtual void                                    PostRender(RenderEventArgs& e) override;

	// IVisitor
	virtual bool                                    Visit(std::shared_ptr<CUIBaseNode> nodeUI) override;

	virtual void                                    UpdateViewport(Viewport _viewport) override final;

private:
	std::shared_ptr<SceneUI>                        m_UIScene;
};