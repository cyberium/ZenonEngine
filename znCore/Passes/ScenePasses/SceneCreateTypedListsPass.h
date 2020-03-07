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
	bool HasModelsList(SceneNodeType SceneNodeType) const;
	const std::vector<CSceneCreateTypelessListPass::SModelElement>& GetModelsList(SceneNodeType SceneNodeType) const;
	bool HasGeometriesList(SceneNodeType SceneNodeType) const;
	const std::vector<CSceneCreateTypelessListPass::SGeometryElement>& GetGeometriesList(SceneNodeType SceneNodeType) const;

	// IRenderPass
	virtual void PreRender(RenderEventArgs& e) override;
	virtual void Render(RenderEventArgs& e) override;

	// IVisitor
	EVisitResult Visit(const ISceneNode3D* SceneNode) override;
	EVisitResult Visit(const IModel* Model) override;
	EVisitResult Visit(const IGeometry* Geometry, const IMaterial* Material, SGeometryDrawArgs GeometryDrawArgs = SGeometryDrawArgs()) override;
	EVisitResult Visit(const ILight3D* light) override;

private:
	std::unordered_map<SceneNodeType, std::vector<CSceneCreateTypelessListPass::SNodeElement>>      m_NodesList;
	std::unordered_map<SceneNodeType, std::vector<CSceneCreateTypelessListPass::SModelElement>>     m_ModelsList;
	std::unordered_map<SceneNodeType, std::vector<CSceneCreateTypelessListPass::SGeometryElement>>  m_GeometryList;
	std::unordered_map<SceneNodeType, std::vector<CSceneCreateTypelessListPass::SLightElement>>     m_LightList;

	const ISceneNode3D*       m_LastSceneNode;
	const IModel*             m_LastModel;
	const IGeometry*          m_LastGeometry;
	const ILight3D*           m_LastLight;
};