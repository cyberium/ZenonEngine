#pragma once

class ZN_API GeometryDX11 
	: public GeometryBase
	, public IGeometryInternal
{
public:
	GeometryDX11(IRenderDeviceDX11& RenderDeviceDX11);
	virtual	~GeometryDX11();

	// IGeometry
	void                      SetPrimitiveTopology(PrimitiveTopology Topology) override;

	void                      Render(         const RenderEventArgs& RenderEventArgs, const IShader* VertexShader, const SGeometryDrawArgs GeometryDrawArgs) const override;

	// IGeometryInternal
	void                      Render_BindAllBuffers(const RenderEventArgs& RenderEventArgs, const IShader* VertexShader) const override;
	void                      Render_Draw(const SGeometryDrawArgs GeometryDrawArgs) const override;
	void                      Render_UnbindAllBuffers(const RenderEventArgs& RenderEventArgs, const IShader* VertexShader) const override;

private:
	D3D11_PRIMITIVE_TOPOLOGY  m_PrimitiveTopology;

private: // Link to parent d3d11 device
	IRenderDeviceDX11& m_RenderDeviceDX11;
};