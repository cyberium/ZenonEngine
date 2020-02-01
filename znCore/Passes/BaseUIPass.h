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
	BaseUIPass(IRenderDevice& RenderDevice, std::shared_ptr<IScene> Scene);
	virtual ~BaseUIPass();

	// IRenderPassPipelined
	virtual std::shared_ptr<IRenderPassPipelined> CreatePipeline(std::shared_ptr<IRenderTarget> RenderTarget, const Viewport* Viewport) override;

	// IVisitor
	virtual bool Visit(ISceneNodeUI* node) override;
	virtual bool Visit(IMesh* Mesh, SGeometryPartParams GeometryPartParams = SGeometryPartParams()) override;
	virtual bool Visit(IGeometry* Geometry, const IMaterial* Material, SGeometryPartParams GeometryPartParams = SGeometryPartParams()) override;

protected:
	virtual void FillPerFrameData() override;

protected:
	PerObjectUI* m_PerObjectData;
	std::shared_ptr<IConstantBuffer> m_PerObjectConstantBuffer;
};