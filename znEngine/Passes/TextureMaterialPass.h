#pragma once

class CTexturedMaterialPass 
	: public Base3DPass
{
public:
	CTexturedMaterialPass(IRenderDevice& RenderDevice, std::shared_ptr<IScene> Scene);
	virtual ~CTexturedMaterialPass();

	// IRenderPassPipelined
	std::shared_ptr<IRenderPassPipelined> CreatePipeline(std::shared_ptr<IRenderTarget> RenderTarget, const Viewport* Viewport) override final;

	// IVisitor
	bool Visit(IMesh* Mesh, SGeometryDrawArgs GeometryDrawArgs) override;
	bool Visit(IGeometry* Geometry, const IMaterial* Material, SGeometryDrawArgs GeometryDrawArgs = SGeometryDrawArgs()) override;
};