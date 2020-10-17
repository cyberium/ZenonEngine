#include "stdafx.h"

// General
#include "RenderDeviceDX11.h"

// Additional
#include "RenderObjectsFactoryDX11.h"

RenderDeviceDX11::RenderDeviceDX11(IBaseManager& BaseManager)
	: m_BaseManager(BaseManager)
{
	m_RenderObjectsFactory = std::make_unique<CRenderObjectsFactoryDX11>(*this);

	m_MainThreadID = std::this_thread::get_id();
}

RenderDeviceDX11::~RenderDeviceDX11()
{
	Log::Info("RenderDeviceDX11 destroyed.");
}



//
// IRenderDevice
//
bool RenderDeviceDX11::Initialize()
{
	InitializeD3D11();

	LoadDefaultResources();

	return true;
}

void RenderDeviceDX11::Finalize()
{
#if defined(_DEBUG)
	if (m_DebugD3D11)
	{
		m_DebugD3D11->ReportLiveDeviceObjects( D3D11_RLDO_SUMMARY | D3D11_RLDO_DETAIL );
	}
#endif
}

std::string RenderDeviceDX11::GetDeviceName() const
{
    return "DirectX device";
}

const RenderDeviceType RenderDeviceDX11::GetDeviceType() const
{
    return RenderDeviceType::RenderDeviceType_DirectX11;
}

IBaseManager& RenderDeviceDX11::GetBaseManager() const
{
	return m_BaseManager;
}

IRenderPrimitivesFactory & RenderDeviceDX11::GetPrimitivesFactory() const
{
	return __super::GetPrimitivesFactory();
}

IRenderObjectsFactory & RenderDeviceDX11::GetObjectsFactory() const
{
	return __super::GetObjectsFactory();
}

std::shared_ptr<ITexture> RenderDeviceDX11::GetDefaultTexture() const
{
	return __super::GetDefaultTexture();
}



//
// RenderDeviceDX11
//
ID3D11Device4* RenderDeviceDX11::GetDeviceD3D11()
{
    return m_DeviceD3D11;
}

ID3D11DeviceContext3* RenderDeviceDX11::GetDeviceContextD3D11()
{
	//_ASSERT(m_MainThreadID == std::this_thread::get_id());
	return m_DeviceImmediateContext;
}



//
// Protected
//
void RenderDeviceDX11::InitializeD3D11()
{
    const D3D_FEATURE_LEVEL featureLevels[] = 
	{
        D3D_FEATURE_LEVEL_11_1, 
		D3D_FEATURE_LEVEL_11_0, 
		D3D_FEATURE_LEVEL_10_1, 
		D3D_FEATURE_LEVEL_10_0
    };

    UINT createDeviceFlags = 0;
#if defined(_DEBUG) 
    createDeviceFlags |= D3D11_CREATE_DEVICE_DEBUG;
#endif

    // This will be the feature level that is used to create our device and swap chain.
    ATL::CComPtr<ID3D11Device> device;
	D3D_FEATURE_LEVEL featureLevel;
    ATL::CComPtr<ID3D11DeviceContext> deviceContext;

	for (auto i = 0; i < _countof(featureLevels); i++)
	{
		HRESULT hr = D3D11CreateDevice(
			nullptr,
			D3D_DRIVER_TYPE_HARDWARE,
			nullptr,
			createDeviceFlags,
			&featureLevels[i],
			_countof(featureLevels) - i,
			D3D11_SDK_VERSION,
			&device,
			&featureLevel,
			&deviceContext
		);

		if SUCCEEDED(hr)
		{
			break;
		}
		else if (hr == E_INVALIDARG)
		{
			continue;
		}
		CHECK_HR_MSG(hr, L"Failed to created DirectX 11 Device");
	}
	CHECK_HR_MSG(device->QueryInterface<ID3D11Device4>(&m_DeviceD3D11), L"Failed to create D3D device.");

    m_DeviceD3D11->GetImmediateContext3(&m_DeviceImmediateContext);

	//hr = m_DeviceD3D11->CreateDeferredContext2(0, &m_DeviceDefferedContext);

	CHECK_HR_MSG(m_DeviceImmediateContext->QueryInterface(__uuidof(ID3D11Multithread), (void**)&m_pMultiThread), L"Failed to query D3DMultithread.");

    m_pMultiThread->SetMultithreadProtected(FALSE);


#if defined(_DEBUG)
    if (SUCCEEDED(m_DeviceD3D11->QueryInterface<ID3D11Debug>(&m_DebugD3D11)))
    {
        ATL::CComPtr<ID3D11InfoQueue> d3dInfoQueue;
        if (SUCCEEDED(m_DebugD3D11->QueryInterface<ID3D11InfoQueue>(&d3dInfoQueue)))
        {

            d3dInfoQueue->SetBreakOnSeverity(D3D11_MESSAGE_SEVERITY_CORRUPTION, TRUE);
            d3dInfoQueue->SetBreakOnSeverity(D3D11_MESSAGE_SEVERITY_ERROR, TRUE);
            //d3dInfoQueue->SetBreakOnSeverity(D3D11_MESSAGE_SEVERITY_WARNING, TRUE);

            D3D11_MESSAGE_ID hide[] =
            {
                D3D11_MESSAGE_ID_SETPRIVATEDATA_CHANGINGPARAMS,
                // Add more message IDs here as needed
            };

			D3D11_INFO_QUEUE_FILTER filter = { 0 };
            filter.DenyList.NumIDs = _countof(hide);
            filter.DenyList.pIDList = hide;
            CHECK_HR(d3dInfoQueue->AddStorageFilterEntries(&filter));
        }
    }
#endif

    // Query the adapter information.
    ATL::CComPtr<IDXGIFactory> factory;
	CHECK_HR_MSG(CreateDXGIFactory(__uuidof(IDXGIFactory), (void**)&factory), L"Failed to create DXGIFactory.");

    ATL::CComPtr<IDXGIAdapter> adapter;
	CHECK_HR_MSG(factory->EnumAdapters(0, &adapter), L"Failed to enumerate adapter.");

    DXGI_ADAPTER_DESC adapterDescription = {};
	CHECK_HR_MSG(adapter->GetDesc(&adapterDescription), L"Failed to query adapter description.");
}
