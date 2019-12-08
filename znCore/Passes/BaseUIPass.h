#pragma once

#include "BaseScenePass.h"

// FORWARD BEGIN
class CUIBaseNode;
// FORWARD END

class OW_ENGINE_API BaseUIPass : public CBaseScenePass
{
public:
	BaseUIPass(std::shared_ptr<IRenderDevice> RenderDevice, std::shared_ptr<IScene> Scene, std::shared_ptr<IPipelineState> pipeline);
	virtual ~BaseUIPass();

	// IVisitor
	virtual bool VisitUI(ISceneNode* node) override;
};