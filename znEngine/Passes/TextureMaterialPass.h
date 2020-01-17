#pragma once

class CTexturedMaterialPass 
	: public Base3DPass
{
public:
	CTexturedMaterialPass(std::shared_ptr<IRenderDevice> RenderDevice, std::shared_ptr<IScene> Scene);
	virtual ~CTexturedMaterialPass();

	// IRenderPassPipelined
	void CreatePipeline(std::shared_ptr<IRenderTarget> RenderTarget, const Viewport* Viewport) override final;

	// IVisitor
	bool Visit(IMesh* Mesh, SGeometryPartParams GeometryPartParams) override;
	bool Visit(IGeometry* Geometry, const IMaterial* Material, SGeometryPartParams GeometryPartParams = SGeometryPartParams()) override;
};