#pragma once

#include "ScenePass.h"
#include "SceneCreateTypelessListPass.h"

/**
  * This pass convert scene tree into arrays of nodes sorted by SceneNodeType
*/
class ZN_API CSceneCreateTypedListsPass
	: public ScenePass
{
public:
	CSceneCreateTypedListsPass(IRenderDevice& RenderDevice, const std::shared_ptr<IScene>& Scene);
	virtual ~CSceneCreateTypedListsPass();

	bool HasNodesList(SceneNodeType SceneNodeType) const;
	const std::vector<CSceneCreateTypelessListPass::SNodeElement>& GetNodesList(SceneNodeType SceneNodeType) const;

	// IRenderPass
	virtual void PreRender(RenderEventArgs& e) override;
	virtual void Render(RenderEventArgs& e) override;

	// IVisitor
	virtual bool Visit(const ISceneNode3D* SceneNode) override;

private:
	std::unordered_map<SceneNodeType, std::vector<CSceneCreateTypelessListPass::SNodeElement>> m_NodesByType;
	std::vector<CSceneCreateTypelessListPass::SNodeElement> m_EmptyList;
};