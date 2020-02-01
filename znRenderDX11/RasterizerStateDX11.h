#pragma once

class ZN_API RasterizerStateDX11 : public RasterizerStateBase
{
public:
	RasterizerStateDX11(IRenderDeviceDX11& RenderDeviceDX11);
	RasterizerStateDX11(const RasterizerStateDX11& copy);
	virtual	~RasterizerStateDX11();

	const RasterizerStateDX11&                      operator=(const RasterizerStateDX11& other);

    // RasterizerState
	void                                            Bind() override final;
	void                                            Unbind() override final;

private:
	ATL::CComPtr<ID3D11RasterizerState1>            m_pRasterizerState;

	std::vector<D3D11_RECT>                         m_d3dRects;
	std::vector<D3D11_VIEWPORT>                     m_d3dViewports;

private: // Link to parent d3d11 device
	IRenderDeviceDX11&                              m_RenderDeviceDX11;
};