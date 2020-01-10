#pragma once

#include "AbstractPass.h"

class ZN_API CBaseScenePass
	: public AbstractPass
{
public:
	CBaseScenePass(std::shared_ptr<IRenderDevice> RenderDevice, std::shared_ptr<IScene> Scene);
	CBaseScenePass(std::shared_ptr<IRenderDevice> RenderDevice, std::shared_ptr<IScene> Scene, std::shared_ptr<IPipelineState> Pipeline);
	virtual ~CBaseScenePass();

	// IRenderPass
	void Render(RenderEventArgs& e) override final;

protected:
	std::shared_ptr<IScene>                         GetScene() const;
	IBaseManager*                                   GetBaseManager() const;

private:
	std::shared_ptr<IScene>                         m_Scene;
};