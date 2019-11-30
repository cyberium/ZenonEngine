#pragma once

class OW_ENGINE_API RasterizerStateDX11 : public RasterizerStateBase
{
public:
	                                                RasterizerStateDX11(ID3D11Device2* pDevice);
	                                                RasterizerStateDX11(const RasterizerStateDX11& copy);
	virtual                                         ~RasterizerStateDX11();

	const RasterizerStateDX11&                      operator=(const RasterizerStateDX11& other);

    // RasterizerState
	void                                            Bind() override final;

private:
	ATL::CComPtr<ID3D11Device2>                     m_pDevice;
	ATL::CComPtr<ID3D11DeviceContext2>              m_pDeviceContext;
	ATL::CComPtr<ID3D11RasterizerState1>            m_pRasterizerState;

	std::vector<D3D11_RECT>                         m_d3dRects;
	std::vector<D3D11_VIEWPORT>                     m_d3dViewports;
};