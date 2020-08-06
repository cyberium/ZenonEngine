#pragma once

#include "../RenderPass.h"

class ZN_API ScenePass
	: public RenderPass
{
public:
	ScenePass(IRenderDevice& RenderDevice, const std::shared_ptr<IScene>& Scene);
	virtual ~ScenePass();

	// IRenderPass
	virtual void Render(RenderEventArgs& e) override;

protected:
	std::shared_ptr<IScene> GetScene() const;
	
private:
	std::weak_ptr<IScene> m_Scene;
};