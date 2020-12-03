#pragma once

#include "SceneCreateTypelessListPass.h"

/**
  * This pass convert scene tree into arrays of nodes sorted by SceneNodeType
*/
class ZN_API CSceneCreateTypedListsPass
	: public ScenePass
{
public:
	CSceneCreateTypedListsPass(IRenderDevice& RenderDevice, IScene& Scene);
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
	EVisitResult Visit(const std::shared_ptr<ISceneNode>& SceneNode) override;
	EVisitResult Visit(const std::shared_ptr<IModel>& Model) override;
	EVisitResult Visit(const std::shared_ptr<IGeometry>& Geometry, const std::shared_ptr<IMaterial>& Material, SGeometryDrawArgs GeometryDrawArgs = SGeometryDrawArgs()) override;
	EVisitResult Visit(const std::shared_ptr<ILight3D>& light) override;
	EVisitResult Visit(const std::shared_ptr<IParticleSystem>& ParticleSystem) override;

private:
	std::unordered_map<ObjectClass, std::vector<CSceneCreateTypelessListPass::SNodeElement>>      m_NodesList;
	std::unordered_map<ObjectClass, std::vector<CSceneCreateTypelessListPass::SModelElement>>     m_ModelsList;
	std::unordered_map<ObjectClass, std::vector<CSceneCreateTypelessListPass::SGeometryElement>>  m_GeometryList;
	std::unordered_map<ObjectClass, std::vector<CSceneCreateTypelessListPass::SLightElement>>     m_LightList;
	std::unordered_map<ObjectClass, std::vector<CSceneCreateTypelessListPass::SParticleSystemElement>>     m_ParticleSystemList;

	std::shared_ptr<ISceneNode>         m_LastSceneNode;
	std::shared_ptr<IModel>             m_LastModel;
	std::shared_ptr<IGeometry>          m_LastGeometry;
	std::shared_ptr<ILight3D>           m_LastLight;
	std::shared_ptr<IParticleSystem>    m_LastParticleSystem;
};