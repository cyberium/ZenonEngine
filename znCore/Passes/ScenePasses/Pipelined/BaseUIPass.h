#pragma once

#include "ScenePassPipelined.h"

__declspec(align(16)) struct ZN_API PerObjectUI
{
	glm::mat4 Model;
};

class ZN_API BaseUIPass 
	: public ScenePassPipelined
{
public:
	BaseUIPass(IRenderDevice& RenderDevice, const std::shared_ptr<IScene>& Scene);
	virtual ~BaseUIPass();

	// IRenderPassPipelined
	virtual std::shared_ptr<IRenderPassPipelined> CreatePipeline(std::shared_ptr<IRenderTarget> RenderTarget, const Viewport* Viewport) override;

	// IVisitor
	virtual EVisitResult Visit(const ISceneNodeUI* node) override;
	virtual EVisitResult Visit(const IModel* Model) override;
	virtual EVisitResult Visit(const IGeometry* Geometry, const IMaterial* Material, SGeometryDrawArgs GeometryDrawArgs = SGeometryDrawArgs()) override;

protected:
	virtual void FillPerFrameData() override;

protected:
	std::shared_ptr<IConstantBuffer> m_PerObjectConstantBuffer;
};