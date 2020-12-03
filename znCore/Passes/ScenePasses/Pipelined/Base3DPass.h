#pragma once

#include "ScenePassPipelined.h"

class ZN_API Base3DPass 
	: public ScenePassPipelined
{
public:
	Base3DPass(IScene& scene);
	virtual ~Base3DPass();

	// IRenderPassPipelined
	virtual std::shared_ptr<IRenderPassPipelined> ConfigurePipeline(std::shared_ptr<IRenderTarget> RenderTarget) override;

	// IVisitor
	virtual EVisitResult Visit(const std::shared_ptr<ISceneNode>& SceneNode) override;
	virtual EVisitResult Visit(const std::shared_ptr<IModel>& Model) override;
	virtual EVisitResult Visit(const std::shared_ptr<IGeometry>& Geometry, const std::shared_ptr<IMaterial>& Material, SGeometryDrawArgs GeometryDrawArgs = SGeometryDrawArgs()) override;
};
