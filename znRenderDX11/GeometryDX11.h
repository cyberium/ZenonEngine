#pragma once

class ZN_API GeometryDX11 
	: public GeometryBase
{
public:
	                                                GeometryDX11(ID3D11Device2 * pDevice);
	virtual                                         ~GeometryDX11();

	virtual void                                    SetPrimitiveTopology(PrimitiveTopology Topology) override;

	virtual bool                                    Render(const RenderEventArgs* renderArgs, const IShader* VertexShader, const IConstantBuffer* perObject, const SRenderGeometryParams& RenderGeometryParams) override;

private:
	D3D11_PRIMITIVE_TOPOLOGY                        m_PrimitiveTopology;

	// DirectX
	ATL::CComPtr<ID3D11Device2>                     m_pDevice;
	ATL::CComPtr<ID3D11DeviceContext2>              m_pDeviceContext;
};