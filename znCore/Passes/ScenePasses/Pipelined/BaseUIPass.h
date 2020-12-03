#pragma once

#include "ScenePassPipelined.h"

class ZN_API BaseUIPass 
	: public ScenePassPipelined
{
public:
	BaseUIPass(IScene& Scene);
	virtual ~BaseUIPass();

	// IRenderPassPipelined
	virtual std::shared_ptr<IRenderPassPipelined> ConfigurePipeline(std::shared_ptr<IRenderTarget> RenderTarget) override;

	// IVisitor
	virtual EVisitResult Visit(const std::shared_ptr<IUIControl>& node) override;
	virtual EVisitResult Visit(const std::shared_ptr<IModel>& Model) override;
	virtual EVisitResult Visit(const std::shared_ptr<IGeometry>& Geometry, const std::shared_ptr<IMaterial>& Material, SGeometryDrawArgs GeometryDrawArgs = SGeometryDrawArgs()) override;

protected:
	PerFrame GetPerFrameData() const override final;
};