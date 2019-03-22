#include <stdafx.h>

#include "BufferDX11.h"
#include "ConstantBufferDX11.h"
#include "StructuredBufferDX11.h"
#include "RenderTargetDX11.h"
#include "MaterialDX11.h"
#include "MeshDX11.h"
#include "ShaderDX11.h"
#include "TextureDX11.h"
#include "SamplerStateDX11.h"
#include "PipelineStateDX11.h"
#include "QueryDX11.h"

// General
#include "RenderDeviceDX11.h"



class D3DMultithreadLocker
{
public:
	D3DMultithreadLocker(ID3D10Multithread * locker)
		: m_Locker(locker)
	{
		//m_Locker->Enter();
	}

	~D3DMultithreadLocker()
	{
		//m_Locker->Leave();
	}

private:
	ATL::CComPtr<ID3D10Multithread> m_Locker;
};



std::string ShaderMacrosToString(const Shader::ShaderMacros& _shaderMacros)
{
	std::string value = "";
	for (const auto& it : _shaderMacros)
		value += it.first + "_" + it.second;
	return value;
}

RenderDeviceDX11::RenderDeviceDX11()
{
	CreateDevice();
	LoadDefaultResources();
}

RenderDeviceDX11::~RenderDeviceDX11()
{
	m_Meshes.clear();
	m_Buffers.clear();
	m_Shaders.clear();
	m_Textures.clear();
	m_Samplers.clear();
	m_Pipelines.clear();
	m_Queries.clear();

#if defined(_DEBUG)
	if (m_pDebugLayer)
	{
		//        m_pDebugLayer->ReportLiveDeviceObjects( D3D11_RLDO_SUMMARY | D3D11_RLDO_DETAIL );
	}
#endif
}

void RenderDeviceDX11::CreateDevice()
{
	const D3D_FEATURE_LEVEL featureLevels[] = {
		D3D_FEATURE_LEVEL_11_1, D3D_FEATURE_LEVEL_11_0, D3D_FEATURE_LEVEL_10_1, D3D_FEATURE_LEVEL_10_0
	};

	UINT createDeviceFlags = 0;
#if defined(_DEBUG)
	createDeviceFlags = D3D11_CREATE_DEVICE_DEBUG;
#endif

	// This will be the feature level that 
	// is used to create our device and swap chain.
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

	// Now query for the ID3D11Device2 interface.
	if (FAILED(pDevice->QueryInterface<ID3D11Device2>(&m_pDevice)))
	{
		Log::Error("Failed to create DirectX 11.2 device");
	}

	// Now get the immediate device context.
	m_pDevice->GetImmediateContext2(&m_pDeviceContext);

	// Need to explitly set the multithreaded mode for this device
	if (FAILED(m_pDeviceContext->QueryInterface(__uuidof(ID3D10Multithread), (void**)&m_pMultiThread)))
	{
		Log::Error("Failed to create DirectX 11.2 device");
	}

	//m_pMultiThread->SetMultithreadProtected(FALSE);

	if (SUCCEEDED(m_pDevice->QueryInterface<ID3D11Debug>(&m_pDebugLayer)))
	{
		ATL::CComPtr<ID3D11InfoQueue> d3dInfoQueue;
		if (SUCCEEDED(m_pDebugLayer->QueryInterface<ID3D11InfoQueue>(&d3dInfoQueue)))
		{
#if defined(_DEBUG)
			d3dInfoQueue->SetBreakOnSeverity(D3D11_MESSAGE_SEVERITY_CORRUPTION, TRUE);
			d3dInfoQueue->SetBreakOnSeverity(D3D11_MESSAGE_SEVERITY_ERROR, TRUE);
			//d3dInfoQueue->SetBreakOnSeverity(D3D11_MESSAGE_SEVERITY_WARNING, TRUE);
#endif 
			D3D11_MESSAGE_ID hide[] =
			{
				D3D11_MESSAGE_ID_SETPRIVATEDATA_CHANGINGPARAMS,
				// Add more message IDs here as needed
			};

			D3D11_INFO_QUEUE_FILTER filter;
			memset(&filter, 0, sizeof(filter));
			filter.DenyList.NumIDs = _countof(hide);
			filter.DenyList.pIDList = hide;
			d3dInfoQueue->AddStorageFilterEntries(&filter);
		}
	}

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

	m_DeviceName = ConvertString(adapterDescription.Description);
}

cstring RenderDeviceDX11::GetDeviceName() const
{
	return m_DeviceName;
}

ATL::CComPtr<ID3D11Device2> RenderDeviceDX11::GetDevice() const
{
	return m_pDevice;
}

ATL::CComPtr<ID3D11DeviceContext2> RenderDeviceDX11::GetDeviceContext() const
{
	return m_pDeviceContext;
}

std::shared_ptr<IBuffer> RenderDeviceDX11::CreateVoidVertexBuffer(const void * data, uint32 count, uint32 offset, uint32 stride)
{
	D3DMultithreadLocker locker(m_pMultiThread);

	std::shared_ptr<IBuffer> buffer = std::make_shared<BufferDX11>(m_pDevice, D3D11_BIND_VERTEX_BUFFER, data, count, offset, stride);
	m_Buffers.push_back(buffer);

	return buffer;
}

std::shared_ptr<IBuffer> RenderDeviceDX11::CreateUInt16IndexBuffer(const uint16* data, uint32 count)
{
	D3DMultithreadLocker locker(m_pMultiThread);

	std::shared_ptr <IBuffer> buffer = std::make_shared<BufferDX11>(m_pDevice, D3D11_BIND_INDEX_BUFFER, data, count, 0, (UINT)sizeof(uint16));
	m_Buffers.push_back(buffer);

	return buffer;
}

std::shared_ptr<IBuffer> RenderDeviceDX11::CreateUInt32IndexBuffer(const uint32* data, uint32 count)
{
	D3DMultithreadLocker locker(m_pMultiThread);

	std::shared_ptr <IBuffer> buffer = std::make_shared<BufferDX11>(m_pDevice, D3D11_BIND_INDEX_BUFFER, data, count, 0, (UINT)sizeof(uint32));
	m_Buffers.push_back(buffer);

	return buffer;
}

void RenderDeviceDX11::DestroyBuffer(std::shared_ptr<IBuffer> buffer)
{
	BufferList::iterator iter = std::find(m_Buffers.begin(), m_Buffers.end(), buffer);
	if (iter != m_Buffers.end())
	{
		m_Buffers.erase(iter);
	}
}

void RenderDeviceDX11::DestroyVertexBuffer(std::shared_ptr<IBuffer> buffer)
{
	DestroyBuffer(buffer);
}

void RenderDeviceDX11::DestroyIndexBuffer(std::shared_ptr<IBuffer> buffer)
{
	DestroyBuffer(buffer);
}

//--

std::shared_ptr<ConstantBuffer> RenderDeviceDX11::CreateConstantBuffer(const void* data, size_t size)
{
	D3DMultithreadLocker locker(m_pMultiThread);

	std::shared_ptr<ConstantBuffer> buffer = std::make_shared<ConstantBufferDX11>(m_pDevice, size);

	if (data)
	{
		buffer->Set(data, size);
	}

	m_Buffers.push_back(buffer);

	return buffer;
}

void RenderDeviceDX11::DestroyConstantBuffer(std::shared_ptr<ConstantBuffer> buffer)
{
	DestroyBuffer(buffer);
}

//--

std::shared_ptr<StructuredBuffer> RenderDeviceDX11::CreateStructuredBuffer(void* data, uint32 count, uint32 stride, CPUAccess cpuAccess, bool gpuWrite)
{
	D3DMultithreadLocker locker(m_pMultiThread);

	std::shared_ptr<StructuredBuffer> buffer = std::make_shared<StructuredBufferDX11>(m_pDevice, 0, data, count, stride, cpuAccess, gpuWrite);
	m_Buffers.push_back(buffer);

	return buffer;
}

void RenderDeviceDX11::DestroyStructuredBuffer(std::shared_ptr<StructuredBuffer> buffer)
{
	DestroyBuffer(buffer);
}

void RenderDeviceDX11::Lock()
{
	//m_pMultiThread->Enter();
}

void RenderDeviceDX11::Unlock()
{
	//m_pMultiThread->Leave();
}

//--

std::shared_ptr<IMesh> RenderDeviceDX11::CreateMesh()
{
	D3DMultithreadLocker locker(m_pMultiThread);

	std::shared_ptr<IMesh> mesh = std::make_shared<MeshDX11>(m_pDevice);
	m_Meshes.push_back(mesh);

	return mesh;
}

void RenderDeviceDX11::DestroyMesh(std::shared_ptr<IMesh> mesh)
{
	MeshList::iterator iter = std::find(m_Meshes.begin(), m_Meshes.end(), mesh);
	if (iter != m_Meshes.end())
	{
		m_Meshes.erase(iter);
	}
}

std::shared_ptr<Shader> RenderDeviceDX11::CreateShader(Shader::ShaderType type, cstring fileName, const Shader::ShaderMacros& shaderMacros, cstring entryPoint, cstring profile, std::shared_ptr<IShaderInputLayout> _customLayout)
{
	D3DMultithreadLocker locker(m_pMultiThread);

	std::string fullName = fileName + ShaderMacrosToString(shaderMacros) + entryPoint + profile;

	ShaderMap::iterator iter = m_ShadersByName.find(fullName);
	if (iter != m_ShadersByName.end())
		return iter->second;

	std::shared_ptr<Shader> pShader = std::make_shared<ShaderDX11>(m_pDevice);
	pShader->LoadShaderFromFile(type, fileName, shaderMacros, entryPoint, profile, _customLayout);
	
	m_Shaders.push_back(pShader);
	m_ShadersByName.insert(ShaderMap::value_type(fullName, pShader));

	return pShader;
}

void RenderDeviceDX11::DestroyShader(std::shared_ptr<Shader> shader)
{
	ShaderList::iterator iter = std::find(m_Shaders.begin(), m_Shaders.end(), shader);
	if (iter != m_Shaders.end())
	{
		m_Shaders.erase(iter);
	}
}


std::shared_ptr<Texture> RenderDeviceDX11::CreateTexture2D(cstring fileName)
{
	D3DMultithreadLocker locker(m_pMultiThread);

	TextureMap::iterator iter = m_TexturesByName.find(fileName);
	if (iter != m_TexturesByName.end())
	{
		return iter->second;
	}

	std::shared_ptr<Texture> texture = std::make_shared<TextureDX11>(m_pDevice);
	if (!texture->LoadTexture2D(fileName))
		return m_pDefaultTexture;

	m_Textures.push_back(texture);
	m_TexturesByName.insert(TextureMap::value_type(fileName, texture));

	return texture;
}

std::shared_ptr<Texture> RenderDeviceDX11::CreateTextureCube(cstring fileName)
{
	D3DMultithreadLocker locker(m_pMultiThread);

	TextureMap::iterator iter = m_TexturesByName.find(fileName);
	if (iter != m_TexturesByName.end())
	{
		return iter->second;
	}

	std::shared_ptr<Texture> texture = std::make_shared<TextureDX11>(m_pDevice);
	texture->LoadTextureCube(fileName);

	m_Textures.push_back(texture);
	m_TexturesByName.insert(TextureMap::value_type(fileName, texture));

	return texture;

}

std::shared_ptr<Texture> RenderDeviceDX11::CreateTexture2D(uint16_t width, uint16_t height, uint16_t slices, const Texture::TextureFormat& format, CPUAccess cpuAccess, bool gpuWrite)
{
	D3DMultithreadLocker locker(m_pMultiThread);

	std::shared_ptr<Texture> texture = std::make_shared<TextureDX11>(m_pDevice, width, height, slices, format, cpuAccess, gpuWrite);
	m_Textures.push_back(texture);

	return texture;
}

std::shared_ptr<Texture> RenderDeviceDX11::CreateTextureCube(uint16_t size, uint16_t numCubes, const Texture::TextureFormat& format, CPUAccess cpuAccess, bool gpuWrite)
{
	D3DMultithreadLocker locker(m_pMultiThread);

	std::shared_ptr<Texture> texture = std::make_shared<TextureDX11>(m_pDevice, size, numCubes, format, cpuAccess, gpuWrite);
	m_Textures.push_back(texture);

	return texture;
}

std::shared_ptr<Texture> RenderDeviceDX11::CreateTexture()
{
	D3DMultithreadLocker locker(m_pMultiThread);

	std::shared_ptr<Texture> texture = std::make_shared<TextureDX11>(m_pDevice);
	m_Textures.push_back(texture);

	return texture;
}

std::shared_ptr<Texture> RenderDeviceDX11::GetDefaultTexture() const
{
	return m_pDefaultTexture;
}

void RenderDeviceDX11::DestroyTexture(std::shared_ptr<Texture> texture)
{
	TextureList::iterator iter = std::find(m_Textures.begin(), m_Textures.end(), texture);
	if (iter != m_Textures.end())
	{
		m_Textures.erase(iter);
	}

	TextureMap::iterator iter2 = std::find_if(m_TexturesByName.begin(), m_TexturesByName.end(), [=](TextureMap::value_type val) { return (val.second == texture); });
	if (iter2 != m_TexturesByName.end())
	{
		m_TexturesByName.erase(iter2);
	}
}

std::shared_ptr<Material> RenderDeviceDX11::CreateMaterial()
{
	D3DMultithreadLocker locker(m_pMultiThread);

	std::shared_ptr<Material> pMaterial = std::make_shared<MaterialDX11>(this);
	m_Materials.push_back(pMaterial);
	return pMaterial;
}

void RenderDeviceDX11::DestroyMaterial(std::shared_ptr<Material> material)
{
	MaterialList::iterator iter = std::find(m_Materials.begin(), m_Materials.end(), material);
	if (iter != m_Materials.end())
	{
		m_Materials.erase(iter);
	}
}


std::shared_ptr<IRenderTarget> RenderDeviceDX11::CreateRenderTarget()
{
	D3DMultithreadLocker locker(m_pMultiThread);

	std::shared_ptr<RenderTargetDX11> renderTarget = std::make_shared<RenderTargetDX11>(m_pDevice);
	m_RenderTargets.push_back(renderTarget);

	return renderTarget;
}

void RenderDeviceDX11::DestroyRenderTarget(std::shared_ptr<IRenderTarget> renderTarget)
{
	RenderTargetList::iterator iter = std::find(m_RenderTargets.begin(), m_RenderTargets.end(), renderTarget);
	if (iter != m_RenderTargets.end())
	{
		m_RenderTargets.erase(iter);
	}
}


std::shared_ptr<SamplerState> RenderDeviceDX11::CreateSamplerState()
{
	D3DMultithreadLocker locker(m_pMultiThread);

	std::shared_ptr<SamplerState> sampler = std::make_shared<SamplerStateDX11>(m_pDevice);
	m_Samplers.push_back(sampler);

	return sampler;
}

void RenderDeviceDX11::DestroySampler(std::shared_ptr<SamplerState> sampler)
{
	SamplerList::iterator iter = std::find(m_Samplers.begin(), m_Samplers.end(), sampler);
	if (iter != m_Samplers.end())
	{
		m_Samplers.erase(iter);
	}
}


std::shared_ptr<PipelineState> RenderDeviceDX11::CreatePipelineState()
{
	D3DMultithreadLocker locker(m_pMultiThread);

	std::shared_ptr<PipelineState> pPipeline = std::make_shared<PipelineStateDX11>(m_pDevice);
	m_Pipelines.push_back(pPipeline);

	return pPipeline;
}

void RenderDeviceDX11::DestoryPipelineState(std::shared_ptr<PipelineState> pipeline)
{
	PipelineList::iterator iter = std::find(m_Pipelines.begin(), m_Pipelines.end(), pipeline);
	if (iter != m_Pipelines.end())
	{
		m_Pipelines.erase(iter);
	}
}


std::shared_ptr<Query> RenderDeviceDX11::CreateQuery(Query::QueryType queryType, uint8_t numBuffers)
{
	D3DMultithreadLocker locker(m_pMultiThread);

	std::shared_ptr<Query> query = std::make_shared<QueryDX11>(m_pDevice, queryType, numBuffers);
	m_Queries.push_back(query);

	return query;
}

void RenderDeviceDX11::DestoryQuery(std::shared_ptr<Query> query)
{
	QueryList::iterator iter = std::find(m_Queries.begin(), m_Queries.end(), query);
	if (iter != m_Queries.end())
	{
		m_Queries.erase(iter);
	}
}

// FreeImage
#define FREEIMAGE_LIB // Static linking
#include <FreeImage.h>

void RenderDeviceDX11::LoadDefaultResources()
{
	FreeImage_Initialise();
	m_pDefaultTexture = CreateTexture2D("Textures\\default.png");
}