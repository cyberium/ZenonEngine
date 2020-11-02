#pragma once

#include "ScenePassPipelined.h"

class ZN_API Base3DPass 
	: public ScenePassPipelined
{
public:
	Base3DPass(IRenderDevice& RenderDevice, IScene& scene);
	virtual ~Base3DPass();

	// IVisitor
	virtual EVisitResult Visit(const ISceneNode* SceneNode) override;
	virtual EVisitResult Visit(const IModel* Model) override;
	virtual EVisitResult Visit(const IGeometry* Geometry, const IMaterial* Material, SGeometryDrawArgs GeometryDrawArgs = SGeometryDrawArgs()) override;

protected:
	std::shared_ptr<IConstantBuffer> m_PerObjectConstantBuffer;
	IShaderParameter* m_PerObjectParameter;
};
