#pragma once

#include "Base3DPass.h"
#include "../SceneCreateTypedListsPass.h"


class ZN_API CBaseList3DPass
	: public RenderPassPipelined
{
public:
	CBaseList3DPass(IRenderDevice& RenderDevice, const std::shared_ptr<CSceneCreateTypedListsPass>& SceneCreateTypedListsPass, SceneNodeType SceneNodeType);
	CBaseList3DPass(IRenderDevice& RenderDevice, const std::shared_ptr<CSceneCreateTypedListsPass>& SceneCreateTypedListsPass, std::vector<SceneNodeType> SceneNodeTypesList);
	virtual ~CBaseList3DPass();

	// IRenderPassPipelined
	void Render(RenderEventArgs& e);

	// IVisitor
	virtual bool Visit(const ISceneNode3D* SceneNode) override;
	virtual bool Visit(const IModel* Model) override;
	virtual bool Visit(const IGeometry* Geometry, const IMaterial* Material, SGeometryDrawArgs GeometryDrawArgs = SGeometryDrawArgs()) override;

protected:
	const std::shared_ptr<CSceneCreateTypedListsPass>& GetSceneNodeListPass() const;
	const std::vector<SceneNodeType>& GetAcceptableNodeTypes() const;

protected:
	std::shared_ptr<CSceneCreateTypedListsPass> m_SceneNodeListPass;
	std::vector<SceneNodeType> m_AcceptSceneNodeTypes;

protected:
	std::shared_ptr<IConstantBuffer> m_PerObjectConstantBuffer;
	IShaderParameter* m_PerObjectParameter;
};