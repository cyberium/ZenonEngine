#pragma once

#include "AbstractPass.h"

class OW_ENGINE_API BaseUIPass : public AbstractPass
{
    typedef AbstractPass base;
public:
	                                                BaseUIPass(std::shared_ptr<IScene> uiScene, std::shared_ptr<IPipelineState> pipeline);
	virtual                                         ~BaseUIPass();

    virtual void                                    Render(RenderEventArgs& e) override;

	// IVisitor
	virtual bool                                    Visit(CUIBaseNode* nodeUI) override;

private:
	std::shared_ptr<IScene>                         m_UIScene;
};