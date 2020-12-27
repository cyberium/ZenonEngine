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

/*namespace
{
	std::string ShaderMacrosToString(const IShader::ShaderMacros& _shaderMacros)
	{
		std::string value = "";
		for (const auto& it : _shaderMacros)
			value += it.first + "_" + it.second;
		return value;
	}
}*/



CRenderObjectsFactoryDX11::CRenderObjectsFactoryDX11(IRenderDeviceDX11& RenderDeviceDX11)
	: CRenderObjectsFactory(RenderDeviceDX11)
	, m_RenderDeviceDX11(RenderDeviceDX11)
{
	std::lock_guard<std::recursive_mutex> locker(m_LockMutex);
}

CRenderObjectsFactoryDX11::~CRenderObjectsFactoryDX11()
{
}



//
// IRenderObjectsFactory
//
std::shared_ptr<IRenderWindow> CRenderObjectsFactoryDX11::CreateRenderWindow(std::unique_ptr<IznNativeWindow> WindowObject, bool vSync)
{
	std::shared_ptr<IRenderWindow> renderWindow = MakeShared(RenderWindowDX11, m_RenderDeviceDX11, std::move(WindowObject), vSync);
	m_RenderWindows.insert(std::make_pair(GenerateRenderObjectID(), renderWindow));
	return renderWindow;
}

std::shared_ptr<IGeometry> CRenderObjectsFactoryDX11::CreateGeometry()
{
	std::lock_guard<std::recursive_mutex> locker(m_LockMutex);

	std::shared_ptr<IGeometry> object = MakeShared(GeometryDX11, m_RenderDeviceDX11);
	//m_Geometries.insert(std::make_pair(GenerateRenderObjectID(), object));
	return object;
}

std::shared_ptr<IModel> CRenderObjectsFactoryDX11::CreateModel()
{
	std::lock_guard<std::recursive_mutex> locker(m_LockMutex);

	std::shared_ptr<IModel> object = MakeShared(ModelDX11, m_RenderDeviceDX11);
	//m_Models.insert(std::make_pair(GenerateRenderObjectID(), object));
	return object;
}

std::shared_ptr<ITexture> CRenderObjectsFactoryDX11::CreateEmptyTexture()
{
	std::lock_guard<std::recursive_mutex> locker(m_LockMutex);

	std::shared_ptr<ITexture> object = MakeShared(TextureDX11, m_RenderDeviceDX11);
	//m_Textures.insert(std::make_pair(GenerateRenderObjectID(), object));
	return object;
}

std::shared_ptr<ITexture> CRenderObjectsFactoryDX11::CreateTexture2D(size_t width, size_t height, size_t slices, const ITexture::TextureFormat& format, EAccess cpuAccess)
{
	std::lock_guard<std::recursive_mutex> locker(m_LockMutex);

	std::shared_ptr<ITexture> object = MakeShared(TextureDX11, m_RenderDeviceDX11, width, height, slices, format, cpuAccess);
	//m_Textures.insert(std::make_pair(GenerateRenderObjectID(), object));
	return object;
}

std::shared_ptr<ITexture> CRenderObjectsFactoryDX11::CreateTextureCube(size_t size, const ITexture::TextureFormat& format, EAccess cpuAccess)
{
	std::lock_guard<std::recursive_mutex> locker(m_LockMutex);

	std::shared_ptr<ITexture> object = MakeShared(TextureDX11, m_RenderDeviceDX11, size, format, cpuAccess);
	//m_Textures.insert(std::make_pair(GenerateRenderObjectID(), object));
	return object;
}

std::shared_ptr<IMaterial> CRenderObjectsFactoryDX11::CreateMaterial(const std::string& MaterialName)
{
	std::lock_guard<std::recursive_mutex> locker(m_LockMutex);

	//const auto& iter = m_MaterialsByName.find(MaterialName);
	//if (iter != m_MaterialsByName.end())
	//{
	//	if (std::shared_ptr<IMaterial> material = iter->second.lock())
	//	{
	//		return material;
	//	}
	//	else
	//	{
	//		m_MaterialsByName.erase(iter);
	//	}
	//}

	std::shared_ptr<IMaterial> object = MakeShared(MaterialDX11, m_RenderDeviceDX11);
	//m_Materials.insert(std::make_pair(GenerateRenderObjectID(), object));
	return object;
}

std::shared_ptr<IRenderTarget> CRenderObjectsFactoryDX11::CreateRenderTarget()
{
	std::lock_guard<std::recursive_mutex> locker(m_LockMutex);

	std::shared_ptr<IRenderTarget> object = MakeShared(RenderTargetDX11, m_RenderDeviceDX11);
	//m_RenderTargets.insert(std::make_pair(GenerateRenderObjectID(), object));
	return object;
}

std::shared_ptr<IQuery> CRenderObjectsFactoryDX11::CreateQuery(IQuery::QueryType queryType, size_t numBuffers)
{
	std::lock_guard<std::recursive_mutex> locker(m_LockMutex);

	std::shared_ptr<IQuery> object = MakeShared(QueryDX11, m_RenderDeviceDX11, queryType, numBuffers);
	//m_Queries.insert(std::make_pair(GenerateRenderObjectID(), object));
	return object;
}

std::shared_ptr<ISamplerState> CRenderObjectsFactoryDX11::CreateSamplerState()
{
	std::lock_guard<std::recursive_mutex> locker(m_LockMutex);

	std::shared_ptr<ISamplerState> object = MakeShared(SamplerStateDX11, m_RenderDeviceDX11);
	//m_Samplers.insert(std::make_pair(GenerateRenderObjectID(), object));
	return object;
}

std::shared_ptr<IBlendState> CRenderObjectsFactoryDX11::CreateBlendState()
{
	std::lock_guard<std::recursive_mutex> locker(m_LockMutex);

	std::shared_ptr<IBlendState> object = MakeShared(BlendStateDX11, m_RenderDeviceDX11);
	//m_BlendStates.insert(std::make_pair(GenerateRenderObjectID(), object));
	return object;
}

std::shared_ptr<IDepthStencilState> CRenderObjectsFactoryDX11::CreateDepthStencilState()
{
	std::lock_guard<std::recursive_mutex> locker(m_LockMutex);

	std::shared_ptr<IDepthStencilState> object = MakeShared(DepthStencilStateDX11, m_RenderDeviceDX11);
	//m_DepthStencilStates.insert(std::make_pair(GenerateRenderObjectID(), object));
	return object;
}

std::shared_ptr<IRasterizerState> CRenderObjectsFactoryDX11::CreateRasterizerState()
{
	std::lock_guard<std::recursive_mutex> locker(m_LockMutex);

	std::shared_ptr<IRasterizerState> object = MakeShared(RasterizerStateDX11, m_RenderDeviceDX11);
	//m_RasterizerStates.insert(std::make_pair(GenerateRenderObjectID(), object));
	return object;
}

std::shared_ptr<IPipelineState> CRenderObjectsFactoryDX11::CreatePipelineState()
{
	std::lock_guard<std::recursive_mutex> locker(m_LockMutex);

	std::shared_ptr<IPipelineState> object = MakeShared(PipelineStateDX11, m_RenderDeviceDX11);
	//m_Pipelines.insert(std::make_pair(GenerateRenderObjectID(), object));
	return object;
}

std::shared_ptr<IBuffer> CRenderObjectsFactoryDX11::CreateVoidVertexBuffer(const void * data, size_t count, size_t offset, size_t stride)
{
	std::lock_guard<std::recursive_mutex> locker(m_LockMutex);

	std::shared_ptr<IBuffer> object = MakeShared(BufferDX11, m_RenderDeviceDX11, IBuffer::BufferType::VertexBuffer);
	std::dynamic_pointer_cast<IBufferPrivate>(object)->InitializeBufferBase(data, count, offset, stride);
	//m_Buffers.insert(std::make_pair(GenerateRenderObjectID(), object));
	return object;
}

std::shared_ptr<IBuffer> CRenderObjectsFactoryDX11::CreateVoidIndexBuffer(const void * data, size_t count, size_t offset, size_t stride)
{
	std::lock_guard<std::recursive_mutex> locker(m_LockMutex);

	std::shared_ptr<IBuffer> object = MakeShared(BufferDX11, m_RenderDeviceDX11, IBuffer::BufferType::IndexBuffer);
	std::dynamic_pointer_cast<IBufferPrivate>(object)->InitializeBufferBase(data, count, offset, stride);
	//m_Buffers.insert(std::make_pair(GenerateRenderObjectID(), object));
	return object;
}

std::shared_ptr<IConstantBuffer> CRenderObjectsFactoryDX11::CreateConstantBuffer(const void* data, size_t size)
{
	std::lock_guard<std::recursive_mutex> locker(m_LockMutex);

	std::shared_ptr<IConstantBuffer> object = MakeShared(ConstantBufferDX11, m_RenderDeviceDX11);
	std::dynamic_pointer_cast<IBufferPrivate>(object)->InitializeBufferBase(data, size, 0, 0);
	if (data)
		object->Set(data, size);

	//m_Buffers.insert(std::make_pair(GenerateRenderObjectID(), object));

	return object;
}

std::shared_ptr<IStructuredBuffer> CRenderObjectsFactoryDX11::CreateStructuredBuffer(const void* data, size_t count, size_t stride, EAccess cpuAccess)
{
	std::lock_guard<std::recursive_mutex> locker(m_LockMutex);

	std::shared_ptr<IStructuredBuffer> object = MakeShared(StructuredBufferDX11, m_RenderDeviceDX11);
	std::dynamic_pointer_cast<IBufferPrivate>(object)->InitializeBufferBase(data, count, 0, stride);
	std::dynamic_pointer_cast<IStructuredBufferPrivate>(object)->InitializeStructuredBufferBase(cpuAccess);
	//m_Buffers.insert(std::make_pair(GenerateRenderObjectID(), object));
	return object;
}



std::shared_ptr<IShader> CRenderObjectsFactoryDX11::LoadShader(EShaderType type, const std::string& fileName, const std::string& entryPoint, const IShader::ShaderMacros& shaderMacros, IShaderInputLayout* _customLayout)
{
	std::lock_guard<std::recursive_mutex> locker(m_LockMutex);

	std::shared_ptr<IShader> object = MakeShared(ShaderDX11, m_RenderDeviceDX11);
	object->LoadFromFile(type, fileName, shaderMacros, entryPoint);
	m_Shaders.insert(std::make_pair(GenerateRenderObjectID(), object));
	return object;
}


std::shared_ptr<IBuffer> CRenderObjectsFactoryDX11::LoadVoidBuffer(const std::shared_ptr<IByteBuffer>& ByteBuffer)
{
	std::lock_guard<std::recursive_mutex> locker(m_LockMutex);

	std::shared_ptr<IBuffer> object = MakeShared(BufferDX11, m_RenderDeviceDX11, IBuffer::BufferType::Unknown);
	if (const auto& loadableFromFile = std::dynamic_pointer_cast<IObjectLoadSave>(object))
		loadableFromFile->Load(ByteBuffer);

	return object;
}

std::shared_ptr<IConstantBuffer> CRenderObjectsFactoryDX11::LoadConstantBuffer(const std::shared_ptr<IByteBuffer>& ByteBuffer)
{
	std::lock_guard<std::recursive_mutex> locker(m_LockMutex);

	std::shared_ptr<IConstantBuffer> object = MakeShared(ConstantBufferDX11, m_RenderDeviceDX11);
	if (const auto& loadableFromFile = std::dynamic_pointer_cast<IObjectLoadSave>(object))
		loadableFromFile->Load(ByteBuffer);

	return object;
}

std::shared_ptr<IStructuredBuffer> CRenderObjectsFactoryDX11::LoadStructuredBuffer(const std::shared_ptr<IByteBuffer>& ByteBuffer)
{
	std::lock_guard<std::recursive_mutex> locker(m_LockMutex);

	std::shared_ptr<IStructuredBuffer> object = MakeShared(StructuredBufferDX11, m_RenderDeviceDX11);
	if (const auto& loadableFromFile = std::dynamic_pointer_cast<IObjectLoadSave>(object))
		loadableFromFile->Load(ByteBuffer);

	return object;
}

//
// XML
//

std::shared_ptr<IBuffer> CRenderObjectsFactoryDX11::LoadVoidBuffer(const std::shared_ptr<IXMLReader>& Reader)
{
	std::lock_guard<std::recursive_mutex> locker(m_LockMutex);

	std::shared_ptr<IBuffer> object = MakeShared(BufferDX11, m_RenderDeviceDX11, IBuffer::BufferType::Unknown);
	if (const auto& loadableFromFile = std::dynamic_pointer_cast<IObjectLoadSave>(object))
		loadableFromFile->Load(Reader);

	return object;
}

std::shared_ptr<IConstantBuffer> CRenderObjectsFactoryDX11::LoadConstantBuffer(const std::shared_ptr<IXMLReader>& Reader)
{
	std::lock_guard<std::recursive_mutex> locker(m_LockMutex);

	std::shared_ptr<IConstantBuffer> object = MakeShared(ConstantBufferDX11, m_RenderDeviceDX11);
	if (const auto& loadableFromFile = std::dynamic_pointer_cast<IObjectLoadSave>(object))
		loadableFromFile->Load(Reader);

	return object;
}

std::shared_ptr<IStructuredBuffer> CRenderObjectsFactoryDX11::LoadStructuredBuffer(const std::shared_ptr<IXMLReader>& Reader)
{
	std::lock_guard<std::recursive_mutex> locker(m_LockMutex);

	std::shared_ptr<IStructuredBuffer> object = MakeShared(StructuredBufferDX11, m_RenderDeviceDX11);
	if (const auto& loadableFromFile = std::dynamic_pointer_cast<IObjectLoadSave>(object))
		loadableFromFile->Load(Reader);

	return object;
}
