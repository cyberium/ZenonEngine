#pragma once

#include "../RenderPass.h"

class ZN_API ScenePass
	: public RenderPass
{
public:
	ScenePass(IRenderDevice& RenderDevice, IScene& Scene);
	virtual ~ScenePass();

	// IRenderPass
	virtual void Render(RenderEventArgs& e) override;

protected:
	IScene& GetScene() const;
	
private:
	IScene& m_Scene;
};