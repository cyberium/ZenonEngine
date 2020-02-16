#include "stdafx.h"

// General
#include "RenderObjectsFactoryDX11.h"

// Additional
#include "RenderWindowDX11.h"
#include "BufferDX11.h"
#include "ConstantBufferDX11.h"
#include "StructuredBufferDX11.h"
#include "RenderTargetDX11.h"
#include "MaterialDX11.h"
#include "GeometryDX11.h"
#include "ModelDX11.h"
#include "ShaderDX11.h"
#include "TextureDX11.h"
#include "SamplerStateDX11.h"
#include "BlendStateDX11.h"
#include "DepthStencilStateDX11.h"
#include "RasterizerStateDX11.h"
#include "PipelineStateDX11.h"
#include "QueryDX11.h"

namespace
{
	std::string ShaderMacrosToString(const IShader::ShaderMacros& _shaderMacros)
	{
		std::string value = "";
		for (const auto& it : _shaderMacros)
			value += it.first + "_" + it.second;
		return value;
	}
}



CRenderObjectsFactoryDX11::CRenderObjectsFactoryDX11(IRenderDeviceDX11& RenderDeviceDX11)
	: CRenderObjectsFactory()
	, m_RenderDeviceDX11(RenderDeviceDX11)
{
	std::lock_guard<std::recursive_mutex> locker(m_LockMutex);
}

CRenderObjectsFactoryDX11::~CRenderObjectsFactoryDX11()
{
	m_Geometries.clear();
	m_Models.clear();
	m_Shaders.clear();
	m_Textures.clear();
	m_Buffers.clear();
	m_Samplers.clear();
	m_Pipelines.clear();
	m_Queries.clear();
}



//
// IRenderObjectsFactory
//
std::shared_ptr<IRenderWindow> CRenderObjectsFactoryDX11::CreateRenderWindow(INativeWindow& WindowObject, bool vSync)
{
	std::shared_ptr<IRenderWindow> renderWindow = std::make_shared<RenderWindowDX11>(m_RenderDeviceDX11, WindowObject, vSync);
	m_RenderWindows.insert(std::make_pair(GenerateRenderObjectID(), renderWindow));
	return renderWindow;
}

std::shared_ptr<IShader> CRenderObjectsFactoryDX11::CreateShader(EShaderType type, const std::string& fileName, const std::string& entryPoint, const IShader::ShaderMacros& shaderMacros, const std::string& profile, IShaderInputLayout* _customLayout)
{
	std::string fullName = fileName + ShaderMacrosToString(shaderMacros) + entryPoint + profile;

	const auto& iter = m_ShadersByName.find(fullName);
	if (iter != m_ShadersByName.end())
	{
		if (std::shared_ptr<IShader> shader = iter->second.lock())
			return shader;
	}

	std::shared_ptr<IShader> object = std::make_shared<ShaderDX11>(m_RenderDeviceDX11);
	object->LoadShaderFromFile(type, fileName, shaderMacros, entryPoint, profile, _customLayout);

	m_Shaders.insert(std::make_pair(GenerateRenderObjectID(), object));
	m_ShadersByName.insert(std::make_pair(fullName, object));

	return object;
}

std::shared_ptr<IGeometry> CRenderObjectsFactoryDX11::CreateGeometry()
{
	std::lock_guard<std::recursive_mutex> locker(m_LockMutex);

	std::shared_ptr<IGeometry> object = std::make_shared<GeometryDX11>(m_RenderDeviceDX11);
	m_Geometries.insert(std::make_pair(GenerateRenderObjectID(), object));
	return object;
}

std::shared_ptr<IModel> CRenderObjectsFactoryDX11::CreateModel()
{
	std::lock_guard<std::recursive_mutex> locker(m_LockMutex);

	std::shared_ptr<IModel> object = std::make_shared<ModelDX11>(m_RenderDeviceDX11);
	m_Models.insert(std::make_pair(GenerateRenderObjectID(), object));
	return object;
}

std::shared_ptr<ITexture> CRenderObjectsFactoryDX11::CreateEmptyTexture()
{
	std::lock_guard<std::recursive_mutex> locker(m_LockMutex);

	std::shared_ptr<ITexture> object = std::make_shared<TextureDX11>(m_RenderDeviceDX11);
	m_Textures.insert(std::make_pair(GenerateRenderObjectID(), object));
	return object;
}

std::shared_ptr<ITexture> CRenderObjectsFactoryDX11::CreateTexture2D(size_t width, size_t height, size_t slices, const ITexture::TextureFormat& format, CPUAccess cpuAccess, bool gpuWrite)
{
	std::lock_guard<std::recursive_mutex> locker(m_LockMutex);

	std::shared_ptr<ITexture> object = std::make_shared<TextureDX11>(m_RenderDeviceDX11, width, height, slices, format, cpuAccess, gpuWrite);
	m_Textures.insert(std::make_pair(GenerateRenderObjectID(), object));
	return object;
}

std::shared_ptr<ITexture> CRenderObjectsFactoryDX11::CreateTextureCube(size_t size, size_t numCubes, const ITexture::TextureFormat& format, CPUAccess cpuAccess, bool gpuWrite)
{
	std::lock_guard<std::recursive_mutex> locker(m_LockMutex);

	std::shared_ptr<ITexture> object = std::make_shared<TextureDX11>(m_RenderDeviceDX11, size, numCubes, format, cpuAccess, gpuWrite);
	m_Textures.insert(std::make_pair(GenerateRenderObjectID(), object));
	return object;
}

std::shared_ptr<ITexture> CRenderObjectsFactoryDX11::LoadTexture2D(const std::string& fileName)
{
	std::lock_guard<std::recursive_mutex> locker(m_LockMutex);

	const auto& iter = m_TexturesByName.find(fileName);
	if (iter != m_TexturesByName.end())
	{
		if (std::shared_ptr<ITexture> texture = iter->second.lock())
			return texture;
	}

	std::shared_ptr<ITexture> object = std::make_shared<TextureDX11>(m_RenderDeviceDX11);
	if (!object->LoadTexture2D(fileName))
		return m_RenderDeviceDX11.GetDefaultTexture();

	m_Textures.insert(std::make_pair(GenerateRenderObjectID(), object));
	m_TexturesByName.insert(std::make_pair(fileName, object));

	return object;
}

std::shared_ptr<ITexture> CRenderObjectsFactoryDX11::LoadTextureCube(const std::string& fileName)
{
	std::lock_guard<std::recursive_mutex> locker(m_LockMutex);

	const auto& iter = m_TexturesByName.find(fileName);
	if (iter != m_TexturesByName.end())
	{
		if (std::shared_ptr<ITexture> texture = iter->second.lock())
			return texture;
	}

	std::shared_ptr<ITexture> object = std::make_shared<TextureDX11>(m_RenderDeviceDX11);
	object->LoadTextureCube(fileName);

	m_Textures.insert(std::make_pair(GenerateRenderObjectID(), object));
	m_TexturesByName.insert(std::make_pair(fileName, object));

	return object;
}

std::shared_ptr<IMaterial> CRenderObjectsFactoryDX11::CreateMaterial(size_t Size)
{
	std::lock_guard<std::recursive_mutex> locker(m_LockMutex);

	std::shared_ptr<IMaterial> object = std::make_shared<MaterialDX11>(m_RenderDeviceDX11, Size);
	m_Materials.insert(std::make_pair(GenerateRenderObjectID(), object));
	return object;
}

std::shared_ptr<IRenderTarget> CRenderObjectsFactoryDX11::CreateRenderTarget()
{
	std::lock_guard<std::recursive_mutex> locker(m_LockMutex);

	std::shared_ptr<IRenderTarget> object = std::make_shared<RenderTargetDX11>(m_RenderDeviceDX11);
	m_RenderTargets.insert(std::make_pair(GenerateRenderObjectID(), object));
	return object;
}

std::shared_ptr<IQuery> CRenderObjectsFactoryDX11::CreateQuery(IQuery::QueryType queryType, size_t numBuffers)
{
	std::lock_guard<std::recursive_mutex> locker(m_LockMutex);

	std::shared_ptr<IQuery> object = std::make_shared<QueryDX11>(m_RenderDeviceDX11, queryType, numBuffers);
	m_Queries.insert(std::make_pair(GenerateRenderObjectID(), object));
	return object;
}

std::shared_ptr<ISamplerState> CRenderObjectsFactoryDX11::CreateSamplerState()
{
	std::lock_guard<std::recursive_mutex> locker(m_LockMutex);

	std::shared_ptr<ISamplerState> object = std::make_shared<SamplerStateDX11>(m_RenderDeviceDX11);
	m_Samplers.insert(std::make_pair(GenerateRenderObjectID(), object));
	return object;
}

std::shared_ptr<IBlendState> CRenderObjectsFactoryDX11::CreateBlendState()
{
	std::lock_guard<std::recursive_mutex> locker(m_LockMutex);

	std::shared_ptr<IBlendState> object = std::make_shared<BlendStateDX11>(m_RenderDeviceDX11);
	m_BlendStates.insert(std::make_pair(GenerateRenderObjectID(), object));
	return object;
}

std::shared_ptr<IDepthStencilState> CRenderObjectsFactoryDX11::CreateDepthStencilState()
{
	std::lock_guard<std::recursive_mutex> locker(m_LockMutex);

	std::shared_ptr<IDepthStencilState> object = std::make_shared<DepthStencilStateDX11>(m_RenderDeviceDX11);
	m_DepthStencilStates.insert(std::make_pair(GenerateRenderObjectID(), object));
	return object;
}

std::shared_ptr<IRasterizerState> CRenderObjectsFactoryDX11::CreateRasterizerState()
{
	std::lock_guard<std::recursive_mutex> locker(m_LockMutex);

	std::shared_ptr<IRasterizerState> object = std::make_shared<RasterizerStateDX11>(m_RenderDeviceDX11);
	m_RasterizerStates.insert(std::make_pair(GenerateRenderObjectID(), object));
	return object;
}

std::shared_ptr<IPipelineState> CRenderObjectsFactoryDX11::CreatePipelineState()
{
	std::lock_guard<std::recursive_mutex> locker(m_LockMutex);

	std::shared_ptr<IPipelineState> object = std::make_shared<PipelineStateDX11>(m_RenderDeviceDX11);
	m_Pipelines.insert(std::make_pair(GenerateRenderObjectID(), object));
	return object;
}

std::shared_ptr<IBuffer> CRenderObjectsFactoryDX11::CreateVoidVertexBuffer(const void * data, size_t count, size_t offset, size_t stride)
{
	std::lock_guard<std::recursive_mutex> locker(m_LockMutex);

	std::shared_ptr<IBuffer> object = std::make_shared<BufferDX11>(m_RenderDeviceDX11, D3D11_BIND_VERTEX_BUFFER, data, count, offset, stride);
	m_Buffers.insert(std::make_pair(GenerateRenderObjectID(), object));
	return object;
}

std::shared_ptr<IBuffer> CRenderObjectsFactoryDX11::CreateVoidIndexBuffer(const void * data, size_t count, size_t offset, size_t stride)
{
	std::lock_guard<std::recursive_mutex> locker(m_LockMutex);

	std::shared_ptr<IBuffer> object = std::make_shared<BufferDX11>(m_RenderDeviceDX11, D3D11_BIND_INDEX_BUFFER, data, count, offset, stride);
	m_Buffers.insert(std::make_pair(GenerateRenderObjectID(), object));
	return object;
}

std::shared_ptr<IConstantBuffer> CRenderObjectsFactoryDX11::CreateConstantBuffer(const void* data, size_t size)
{
	std::lock_guard<std::recursive_mutex> locker(m_LockMutex);

	std::shared_ptr<IConstantBuffer> object = std::make_shared<ConstantBufferDX11>(m_RenderDeviceDX11, size);
	if (data)
	{
		object->Set(data, size);
	}

	m_Buffers.insert(std::make_pair(GenerateRenderObjectID(), object));

	return object;
}

std::shared_ptr<IStructuredBuffer> CRenderObjectsFactoryDX11::CreateStructuredBuffer(void* data, size_t count, size_t stride, CPUAccess cpuAccess, bool gpuWrite)
{
	std::lock_guard<std::recursive_mutex> locker(m_LockMutex);

	std::shared_ptr<IStructuredBuffer> object = std::make_shared<StructuredBufferDX11>(m_RenderDeviceDX11, 0, data, count, stride, cpuAccess, gpuWrite);
	m_Buffers.insert(std::make_pair(GenerateRenderObjectID(), object));
	return object;
}
