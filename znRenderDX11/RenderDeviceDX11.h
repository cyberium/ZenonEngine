#pragma once

class ZN_API RenderDeviceDX11 
	: public CRenderDeviceBase
	, public IRenderDeviceDX11
{
public:
	RenderDeviceDX11(IBaseManager* BaseManager);
	virtual ~RenderDeviceDX11();

	// IRenderDevice
	bool                                            Initialize() override;
	void                                            Finalize() override;
	std::string                                     GetDeviceName() const;
    const RenderDeviceType                          GetDeviceType() const;
	IBaseManager*                                   GetBaseManager() const;
	IRenderPrimitivesFactory&                       GetPrimitivesFactory() const override;
	IRenderObjectsFactory&                          GetObjectsFactory() const override;
	std::shared_ptr<ITexture>                       GetDefaultTexture() const override;
	

	// RenderDeviceDX11
	ID3D11Device4*                                  GetDeviceD3D11() override final;
	ID3D11DeviceContext3*                           GetDeviceContextD3D11() override final;

protected:
	void                                            InitializeD3D11();

private:
	IBaseManager*                                   m_BaseManager;

	// DirectX
	ATL::CComPtr<ID3D11Device4>                     m_DeviceD3D11;
	ATL::CComPtr<ID3D11Debug>                       m_DebugD3D11;
	ATL::CComPtr<ID3D11DeviceContext3>              m_DeviceImmediateContext;
	ATL::CComPtr<ID3D11DeviceContext3>              m_DeviceDefferedContext;
	ATL::CComPtr<ID3D11Multithread>                 m_pMultiThread;
};