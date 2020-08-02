#pragma once

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

	bool HasNodesList(ObjectClass SceneNodeType) const;
	const std::vector<CSceneCreateTypelessListPass::SNodeElement>& GetNodesList(ObjectClass SceneNodeType) const;
	bool HasModelsList(ObjectClass SceneNodeType) const;
	const std::vector<CSceneCreateTypelessListPass::SModelElement>& GetModelsList(ObjectClass SceneNodeType) const;
	bool HasGeometriesList(ObjectClass SceneNodeType) const;
	const std::vector<CSceneCreateTypelessListPass::SGeometryElement>& GetGeometriesList(ObjectClass SceneNodeType) const;
	bool HasParticleSystemsList(ObjectClass SceneNodeType) const;
	const std::vector<CSceneCreateTypelessListPass::SParticleSystemElement>& GetParticleSystemList(ObjectClass SceneNodeType) const;

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
	std::unordered_map<ObjectClass, std::vector<CSceneCreateTypelessListPass::SNodeElement>>      m_NodesList;
	std::unordered_map<ObjectClass, std::vector<CSceneCreateTypelessListPass::SModelElement>>     m_ModelsList;
	std::unordered_map<ObjectClass, std::vector<CSceneCreateTypelessListPass::SGeometryElement>>  m_GeometryList;
	std::unordered_map<ObjectClass, std::vector<CSceneCreateTypelessListPass::SLightElement>>     m_LightList;
	std::unordered_map<ObjectClass, std::vector<CSceneCreateTypelessListPass::SParticleSystemElement>>     m_ParticleSystemList;

	const ISceneNode3D*       m_LastSceneNode;
	const IModel*             m_LastModel;
	const IGeometry*          m_LastGeometry;
	const ILight3D*           m_LastLight;
	const IParticleSystem*    m_LastParticleSystem;
};