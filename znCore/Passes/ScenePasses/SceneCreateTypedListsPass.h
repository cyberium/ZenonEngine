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

	bool HasNodesList(ObjectClassKey SceneNodeType) const;
	const std::vector<CSceneCreateTypelessListPass::SNodeElement>& GetNodesList(ObjectClassKey SceneNodeType) const;
	bool HasModelsList(ObjectClassKey SceneNodeType) const;
	const std::vector<CSceneCreateTypelessListPass::SModelElement>& GetModelsList(ObjectClassKey SceneNodeType) const;
	bool HasGeometriesList(ObjectClassKey SceneNodeType) const;
	const std::vector<CSceneCreateTypelessListPass::SGeometryElement>& GetGeometriesList(ObjectClassKey SceneNodeType) const;
	bool HasParticleSystemsList(ObjectClassKey SceneNodeType) const;
	const std::vector<CSceneCreateTypelessListPass::SParticleSystemElement>& GetParticleSystemList(ObjectClassKey SceneNodeType) const;

	// IRenderPass
	virtual void PreRender(RenderEventArgs& e) override;
	virtual void Render(RenderEventArgs& e) override;

	// IVisitor
	EVisitResult Visit(const ISceneNode3D* SceneNode) override;
	EVisitResult Visit(const IModel* Model) override;
	EVisitResult Visit(const IGeometry* Geometry, const IMaterial* Material, SGeometryDrawArgs GeometryDrawArgs = SGeometryDrawArgs()) override;
	EVisitResult Visit(const ILight3D* light) override;
	EVisitResult Visit(const IParticleSystem* ParticleSystem) override;

private:
	std::unordered_map<ObjectClassKey, std::vector<CSceneCreateTypelessListPass::SNodeElement>>      m_NodesList;
	std::unordered_map<ObjectClassKey, std::vector<CSceneCreateTypelessListPass::SModelElement>>     m_ModelsList;
	std::unordered_map<ObjectClassKey, std::vector<CSceneCreateTypelessListPass::SGeometryElement>>  m_GeometryList;
	std::unordered_map<ObjectClassKey, std::vector<CSceneCreateTypelessListPass::SLightElement>>     m_LightList;
	std::unordered_map<ObjectClassKey, std::vector<CSceneCreateTypelessListPass::SParticleSystemElement>>     m_ParticleSystemList;

	const ISceneNode3D*       m_LastSceneNode;
	const IModel*             m_LastModel;
	const IGeometry*          m_LastGeometry;
	const ILight3D*           m_LastLight;
	const IParticleSystem*    m_LastParticleSystem;
};