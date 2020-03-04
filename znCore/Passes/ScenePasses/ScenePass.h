#pragma once

#include "../RenderPass.h"

class ZN_API ScenePass
	: public RenderPass
{
public:
	ScenePass(IRenderDevice& RenderDevice, const std::shared_ptr<IScene>& Scene);
	virtual ~ScenePass();

	// IRenderPass
	void Render(RenderEventArgs& e) override;

protected:
	const std::shared_ptr<IScene>& GetScene() const;
	
private:
	std::shared_ptr<IScene> m_Scene;
};