#pragma once

#include "ScenePassPipelined.h"
#include "Technical/BuildRenderListPass.h"

__declspec(align(16)) struct ZN_API PerObject3D
{
	glm::mat4 Model;
};


class ZN_API Base3DPass 
	: public ScenePassPipelined
{
public:
	Base3DPass(IRenderDevice& RenderDevice, const std::shared_ptr<IScene>& scene);
	virtual ~Base3DPass();

	// IVisitor
	virtual bool Visit(const ISceneNode3D* SceneNode) override;
	virtual bool Visit(const IModel* Model) override;
	virtual bool Visit(const IGeometry* Geometry, const IMaterial* Material, SGeometryDrawArgs GeometryDrawArgs = SGeometryDrawArgs()) override;

protected:
	std::shared_ptr<IConstantBuffer> m_PerObjectConstantBuffer;
	IShaderParameter* m_PerObjectParameter;
};



//
// CBaseList3DPass
//
class ZN_API CBaseList3DPass
	: public RenderPassPipelined
{
public:
	CBaseList3DPass(IRenderDevice& RenderDevice, const std::shared_ptr<CSceneNodeListPass>& SceneNodeListPass, SceneNodeType SceneNodeType);
	CBaseList3DPass(IRenderDevice& RenderDevice, const std::shared_ptr<CSceneNodeListPass>& SceneNodeListPass, std::vector<SceneNodeType> SceneNodeTypesList);
	virtual ~CBaseList3DPass();

	// IRenderPassPipelined
	void Render(RenderEventArgs& e);

	// IVisitor
	virtual bool Visit(const ISceneNode3D* SceneNode) override;
	virtual bool Visit(const IModel* Model) override;
	virtual bool Visit(const IGeometry* Geometry, const IMaterial* Material, SGeometryDrawArgs GeometryDrawArgs = SGeometryDrawArgs()) override;

protected:
	std::shared_ptr<CSceneNodeListPass> m_SceneNodeListPass;
	std::vector<SceneNodeType> m_AcceptSceneNodeTypes;

protected:
	std::shared_ptr<IConstantBuffer> m_PerObjectConstantBuffer;
	IShaderParameter* m_PerObjectParameter;
};