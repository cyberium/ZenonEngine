#pragma once

class CDebugMaterialPass 
	: public Base3DPass
{
public:
	CDebugMaterialPass(IRenderDevice& RenderDevice, std::shared_ptr<IScene> Scene);
	virtual ~CDebugMaterialPass();

	// IRenderPassPipelined
	std::shared_ptr<IRenderPassPipelined> CreatePipeline(std::shared_ptr<IRenderTarget> RenderTarget, const Viewport* Viewport) override final;

	// IVisitor
	bool Visit(IMesh* Mesh, SGeometryPartParams GeometryPartParams) override final;
	bool Visit(IGeometry* Geometry, const IMaterial* Material, SGeometryPartParams GeometryPartParams = SGeometryPartParams()) override final;
};