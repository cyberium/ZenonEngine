#pragma once

#include "ScenePassPipelined.h"

__declspec(align(16)) struct PerObjectUI
{
	glm::mat4 Model;
	glm::mat4 Projection;
};

class ZN_API BaseUIPass 
	: public ScenePassPipelined
{
public:
	BaseUIPass(std::shared_ptr<IRenderDevice> RenderDevice, std::shared_ptr<IScene> Scene);
	virtual ~BaseUIPass();

	// IRenderPassPipelined
	virtual void CreatePipeline(std::shared_ptr<IRenderTarget> RenderTarget, const Viewport* Viewport) override;

	// IVisitor
	virtual bool VisitUI(ISceneNode* node) override;
	virtual bool Visit(IMesh* Mesh, SGeometryPartParams GeometryPartParams = SGeometryPartParams()) override;
	virtual bool Visit(IGeometry* Geometry, const IMaterial* Material, SGeometryPartParams GeometryPartParams = SGeometryPartParams()) override;

protected:
	PerObjectUI* m_PerObjectData;
	std::shared_ptr<IConstantBuffer> m_PerObjectConstantBuffer;
};