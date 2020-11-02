#pragma once

#include "../../RenderPassPipelined.h"

class ZN_API ScenePassPipelined
	: public RenderPassPipelined
{
public:
	ScenePassPipelined(IRenderDevice& RenderDevice, IScene& Scene);
	virtual ~ScenePassPipelined();

	// IRenderPass
	void Render(RenderEventArgs& e) override;

protected:
	IScene& GetScene() const;
	
private:
	IScene& m_Scene;
};