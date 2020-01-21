#include "stdafx.h"

// General
#include "RenderDeviceDX11.h"

// Additional
#include "RenderWindowDX11.h"


class D3DMultithreadLocker
{
public:
    D3DMultithreadLocker(ID3D11Multithread * locker)
        : m_Locker(locker)
    {
        //m_Locker->Enter();
    }

    ~D3DMultithreadLocker()
    {
        //m_Locker->Leave();
    }

private:
    ATL::CComPtr<ID3D11Multithread> m_Locker;
};





RenderDeviceDX11::RenderDeviceDX11(IBaseManager* BaseManager)
	: m_BaseManager(BaseManager)
{
}

RenderDeviceDX11::~RenderDeviceDX11()
{}

bool RenderDeviceDX11::Initialize()
{
	InitializeD3D11();

	LoadDefaultResources();

	return true;
}

void RenderDeviceDX11::Finalize()
{
	m_Meshes.clear();
	m_Shaders.clear();
	m_Textures.clear();
	m_Buffers.clear();
	m_Samplers.clear();
	m_Pipelines.clear();
	m_Queries.clear();

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
    return RenderDeviceType::RenderDeviceType_DirectX;
}

IBaseManager* RenderDeviceDX11::GetBaseManager() const
{
	return m_BaseManager;
}

std::shared_ptr<IRenderWindow> RenderDeviceDX11::CreateRenderWindow(IWindowObject * WindowObject, bool vSync)
{
	return std::make_shared<RenderWindowDX11>(this, WindowObject, vSync);
}

std::shared_ptr<IRenderDevice> RenderDeviceDX11::GetRenderDevice()
{
	return shared_from_this();
}

ID3D11Device4* RenderDeviceDX11::GetDeviceD3D11()
{
    return m_DeviceD3D11;
}

ID3D11DeviceContext3 * RenderDeviceDX11::GetDeviceContextD3D11()
{
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
    D3D_FEATURE_LEVEL featureLevel;

    ATL::CComPtr<ID3D11Device> pDevice;
    ATL::CComPtr<ID3D11DeviceContext> pDeviceContext;

    // First create a ID3D11Device and ID3D11DeviceContext
    HRESULT hr = D3D11CreateDevice
    (
        nullptr,
        D3D_DRIVER_TYPE_HARDWARE,
        nullptr,
        createDeviceFlags,
        featureLevels,
        _countof(featureLevels),
        D3D11_SDK_VERSION,
        &pDevice,
        &featureLevel,
        &pDeviceContext
    );

    if (hr == E_INVALIDARG)
    {
        hr = D3D11CreateDevice
        (
            nullptr,
            D3D_DRIVER_TYPE_HARDWARE,
            nullptr, createDeviceFlags,
            &featureLevels[1],
            _countof(featureLevels) - 1,
            D3D11_SDK_VERSION,
            &pDevice,
            &featureLevel,
            &pDeviceContext
        );
    }

    if (FAILED(hr))
    {
        Log::Error("Failed to created DirectX 11 Device");
        return;
    }

    if (FAILED(pDevice->QueryInterface<ID3D11Device4>(&m_DeviceD3D11)))
    {
        Log::Error("Failed to create DirectX 11.2 device");
    }

    // Now get the immediate device context.
    m_DeviceD3D11->GetImmediateContext3(&m_DeviceImmediateContext);

	//hr = m_DeviceD3D11->CreateDeferredContext2(0, &m_DeviceDefferedContext);

    // Need to explitly set the multithreaded mode for this device
    if (FAILED(m_DeviceImmediateContext->QueryInterface(__uuidof(ID3D11Multithread), (void**)&m_pMultiThread)))
    {
        Log::Error("Failed to create DirectX 11.2 device");
    }
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
            d3dInfoQueue->AddStorageFilterEntries(&filter);
        }
    }
#endif

    // Query the adapter information.
    ATL::CComPtr<IDXGIFactory> factory;
    ATL::CComPtr<IDXGIAdapter> adapter;
    DXGI_ADAPTER_DESC adapterDescription = {};

    if (FAILED(CreateDXGIFactory(__uuidof(IDXGIFactory), (void**)&factory)))
    {
        Log::Error("Failed to create DXGIFactory.");
    }
    if (FAILED(factory->EnumAdapters(0, &adapter)))
    {
        Log::Error("Failed to enumerate adapter.");
    }
    if (FAILED(adapter->GetDesc(&adapterDescription)))
    {
        Log::Error("Failed to qauery adapter description.");
    }
}

void RenderDeviceDX11::DestroyBufferD3D11(std::shared_ptr<IBuffer> buffer)
{
	BufferList::iterator iter = std::find(m_Buffers.begin(), m_Buffers.end(), buffer);
	if (iter != m_Buffers.end())
	{
		m_Buffers.erase(iter);
	}
}


//
// Private
//
void RenderDeviceDX11::LoadDefaultResources()
{
    m_pDefaultTexture = CreateTexture2D("IDB_DEFAULT_TEXTURE");
}
