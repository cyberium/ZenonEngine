#pragma once

class OW_ENGINE_API MeshDX11 : public IMesh
{
public:
	                                                MeshDX11(ID3D11Device2 * pDevice);
	virtual                                         ~MeshDX11();

	virtual void                                    SetPrimitiveTopology(PrimitiveTopology _topology);

	virtual bool                                    Render(const RenderEventArgs* renderArgs, const ConstantBuffer* perObject, UINT indexStartLocation = 0, UINT indexCnt = 0, UINT vertexStartLocation = 0, UINT vertexCnt = 0);

private:
	D3D11_PRIMITIVE_TOPOLOGY                        m_PrimitiveTopology;

	// DirectX
	ATL::CComPtr<ID3D11Device2>                     m_pDevice;
	ATL::CComPtr<ID3D11DeviceContext2>              m_pDeviceContext;
};