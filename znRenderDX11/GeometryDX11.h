#pragma once

class ZN_API GeometryDX11 
	: public GeometryBase
{
public:
	GeometryDX11(IRenderDeviceDX11& RenderDeviceDX11);
	virtual	~GeometryDX11();

	// IGeometry
	void                      SetPrimitiveTopology(PrimitiveTopology Topology) override;

	bool                      Render(         const RenderEventArgs& RenderEventArgs, const IShader* VertexShader, const SGeometryDrawArgs GeometryDrawArgs) const override;
	bool                      RenderInstanced(const RenderEventArgs& RenderEventArgs, const IShader* VertexShader, const SGeometryDrawInstancedArgs GeometryDrawInstancedArgs) const override;

private:
	D3D11_PRIMITIVE_TOPOLOGY  m_PrimitiveTopology;

private: // Link to parent d3d11 device
	IRenderDeviceDX11& m_RenderDeviceDX11;
};