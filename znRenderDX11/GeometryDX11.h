#pragma once

class ZN_API GeometryDX11 
	: public GeometryBase
{
public:
	GeometryDX11(IRenderDeviceDX11& RenderDeviceDX11);
	virtual	~GeometryDX11();

	virtual void                                    SetPrimitiveTopology(PrimitiveTopology Topology) override;

	virtual bool                                    Render(         const RenderEventArgs& RenderEventArgs, const ShaderMap& ShadersMap, const IMaterial* Material, const SGeometryDrawArgs& GeometryDrawArgs) const override;
	virtual bool                                    RenderInstanced(const RenderEventArgs& RenderEventArgs, const ShaderMap& ShadersMap, const IMaterial* Material, const SGeometryDrawInstancedArgs& GeometryDrawInstancedArgs) const override;

private:
	D3D11_PRIMITIVE_TOPOLOGY m_PrimitiveTopology;

private: // Link to parent d3d11 device
	IRenderDeviceDX11& m_RenderDeviceDX11;
};