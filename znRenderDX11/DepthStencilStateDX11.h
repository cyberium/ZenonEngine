#pragma once

class OW_ENGINE_API DepthStencilStateDX11 : public DepthStencilStateBase
{
public:
	DepthStencilStateDX11(ID3D11Device2* pDevice);
	DepthStencilStateDX11(const DepthStencilStateDX11& copy);

	virtual ~DepthStencilStateDX11();

	const DepthStencilStateDX11& operator=(const DepthStencilStateDX11& other);

    // DepthStencilState
    void                                            Bind() override final;

private:
	ATL::CComPtr<ID3D11Device2>                     m_pDevice;
	ATL::CComPtr<ID3D11DeviceContext2>              m_pDeviceContext;
	ATL::CComPtr<ID3D11DepthStencilState>           m_pDepthStencilState;
};