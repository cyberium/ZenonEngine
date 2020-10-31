#pragma once

#include "../Scene/SceneCreateTypedListsPass.h"


class ZN_API CBaseList3DPass
	: public RenderPassPipelined
{
public:
	CBaseList3DPass(IRenderDevice& RenderDevice, const std::shared_ptr<CSceneCreateTypedListsPass>& SceneCreateTypedListsPass, ObjectClass SceneNodeType);
	CBaseList3DPass(IRenderDevice& RenderDevice, const std::shared_ptr<CSceneCreateTypedListsPass>& SceneCreateTypedListsPass, std::vector<ObjectClass> SceneNodeTypesList);
	virtual ~CBaseList3DPass();

	// IRenderPassPipelined
	void Render(RenderEventArgs& e);

	// IVisitor
	virtual EVisitResult Visit(const ISceneNode3D* SceneNode) override;
	virtual EVisitResult Visit(const IModel* Model) override;
	virtual EVisitResult Visit(const IGeometry* Geometry, const IMaterial* Material, SGeometryDrawArgs GeometryDrawArgs = SGeometryDrawArgs()) override;

protected:
	const std::shared_ptr<CSceneCreateTypedListsPass>& GetSceneNodeListPass() const;
	const std::vector<ObjectClass>& GetAcceptableNodeTypes() const;

protected:
	std::shared_ptr<CSceneCreateTypedListsPass> m_SceneNodeListPass;
	std::vector<ObjectClass> m_AcceptSceneNodeTypes;

private:
	std::shared_ptr<IConstantBuffer> m_PerObjectConstantBuffer;
	IShaderParameter* m_PerObjectParameter;
};