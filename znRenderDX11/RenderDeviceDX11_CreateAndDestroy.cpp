#include "stdafx.h"

// General
#include "RenderDeviceDX11.h"

// Additional
#include "RenderWindowDX11.h"
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

IRenderWindow* RenderDeviceDX11::CreateRenderWindow(INativeWindow * WindowObject, bool vSync)
{
	IRenderWindow* renderWindow = new RenderWindowDX11(this, WindowObject, vSync);
	m_RenderWindows.push_back(std::unique_ptr<IRenderWindow>(renderWindow));
	return renderWindow;
}

void RenderDeviceDX11::DestroyRenderWindow(IRenderWindow * RenderWindow)
{
}

IShader* RenderDeviceDX11::CreateShader(EShaderType type, const std::string& fileName, const IShader::ShaderMacros& shaderMacros, const std::string& entryPoint, const std::string& profile, IShaderInputLayout* _customLayout)
{
	std::string fullName = fileName + ShaderMacrosToString(shaderMacros) + entryPoint + profile;

	auto& iter = m_ShadersByName.find(fullName);
	if (iter != m_ShadersByName.end())
		return iter->second.get();

	std::shared_ptr<IShader> pShader = std::make_shared<ShaderDX11>(this);
	pShader->LoadShaderFromFile(type, fileName, shaderMacros, entryPoint, profile, _customLayout);

	m_Shaders.push_back(pShader);
	m_ShadersByName.insert(std::make_pair(fullName, pShader));

	return pShader.get();
}

void RenderDeviceDX11::DestroyShader(IShader* shader)
{
	/*auto& iter = std::find(m_Shaders.begin(), m_Shaders.end(), shader);
	if (iter != m_Shaders.end())
	{
		m_Shaders.erase(iter);
	}

	auto& iter2 = std::find_if(m_ShadersByName.begin(), m_ShadersByName.end(), [=](std::unordered_map<std::string, std::shared_ptr<IShader>>::value_type val) { return (val.second.get() == shader); });
	if (iter2 != m_ShadersByName.end())
	{
		m_ShadersByName.erase(iter2);
	}*/
}

IMesh* RenderDeviceDX11::CreateMesh()
{
	std::shared_ptr<IMesh> mesh = std::make_shared<MeshDX11>(this);
	m_Meshes.push_back(mesh);

	return mesh.get();
}

void RenderDeviceDX11::DestroyMesh(IMesh* mesh)
{
	//auto& iter = std::find(m_Meshes.begin(), m_Meshes.end(), mesh);
	//if (iter != m_Meshes.end())
	//{
	//	m_Meshes.erase(iter);
	//}
}

ITexture* RenderDeviceDX11::CreateTexture2D(const std::string& fileName)
{
	auto& iter = m_TexturesByName.find(fileName);
	if (iter != m_TexturesByName.end())
	{
		return iter->second.get();
	}

	std::shared_ptr<ITexture> texture = std::make_shared<TextureDX11>(this);
	if (!texture->LoadTexture2D(fileName))
		return m_pDefaultTexture;

	m_Textures.push_back(texture);
	m_TexturesByName.insert(std::make_pair(fileName, texture));

	return texture.get();
}

ITexture* RenderDeviceDX11::CreateTextureCube(const std::string& fileName)
{
	auto& iter = m_TexturesByName.find(fileName);
	if (iter != m_TexturesByName.end())
	{
		return iter->second.get();
	}

	std::shared_ptr<ITexture> texture = std::make_shared<TextureDX11>(this);
	texture->LoadTextureCube(fileName);

	m_Textures.push_back(texture);
	m_TexturesByName.insert(std::make_pair(fileName, texture));

	return texture.get();

}

ITexture* RenderDeviceDX11::CreateTexture2D(uint16_t width, uint16_t height, uint16_t slices, const ITexture::TextureFormat& format, CPUAccess cpuAccess, bool gpuWrite)
{
	std::shared_ptr<ITexture> texture = std::make_shared<TextureDX11>(this, width, height, slices, format, cpuAccess, gpuWrite);
	m_Textures.push_back(texture);

	return texture.get();
}

ITexture* RenderDeviceDX11::CreateTextureCube(uint16_t size, uint16_t numCubes, const ITexture::TextureFormat& format, CPUAccess cpuAccess, bool gpuWrite)
{
	std::shared_ptr<ITexture> texture = std::make_shared<TextureDX11>(this, size, numCubes, format, cpuAccess, gpuWrite);
	m_Textures.push_back(texture);

	return texture.get();
}

ITexture* RenderDeviceDX11::GetDefaultTexture() const
{
	return m_pDefaultTexture;
}

ITexture* RenderDeviceDX11::CreateTexture()
{
	std::shared_ptr<ITexture> texture = std::make_shared<TextureDX11>(this);
	m_Textures.push_back(texture);

	return texture.get();
}

void RenderDeviceDX11::DestroyTexture(ITexture* texture)
{
	/*auto& iter = std::find(m_Textures.begin(), m_Textures.end(), texture);
	if (iter != m_Textures.end())
	{
		m_Textures.erase(iter);
	}

	auto& iter2 = std::find_if(m_TexturesByName.begin(), m_TexturesByName.end(), [=](std::unordered_map<std::string, std::shared_ptr<ITexture>>::value_type val) { return (val.second.get() == texture); });
	if (iter2 != m_TexturesByName.end())
	{
		m_TexturesByName.erase(iter2);
	}*/
}

IMaterial* RenderDeviceDX11::CreateMaterial(size_t Size)
{
	std::shared_ptr<IMaterial> pMaterial = std::make_shared<MaterialDX11>(this, Size);
	m_Materials.push_back(pMaterial);
	return pMaterial.get();
}

void RenderDeviceDX11::DestroyMaterial(IMaterial* material)
{
	//auto& iter = std::find(m_Materials.begin(), m_Materials.end(), material);
	//if (iter != m_Materials.end())
	//{
	//	m_Materials.erase(iter);
	//}
}


IRenderTarget* RenderDeviceDX11::CreateRenderTarget()
{
	std::shared_ptr<IRenderTarget> renderTarget = std::make_shared<RenderTargetDX11>(this);
	m_RenderTargets.push_back(renderTarget);

	return renderTarget.get();
}

void RenderDeviceDX11::DestroyRenderTarget(IRenderTarget* renderTarget)
{
	//auto& iter = std::find(m_RenderTargets.begin(), m_RenderTargets.end(), renderTarget);
	//if (iter != m_RenderTargets.end())
	//{
	//	m_RenderTargets.erase(iter);
	//}
}

IQuery* RenderDeviceDX11::CreateQuery(IQuery::QueryType queryType, uint8_t numBuffers)
{
	std::shared_ptr<IQuery> query = std::make_shared<QueryDX11>(this, queryType, numBuffers);
	m_Queries.push_back(query);

	return query.get();
}

void RenderDeviceDX11::DestoryQuery(IQuery* query)
{
	//auto& iter = std::find(m_Queries.begin(), m_Queries.end(), query);
	//if (iter != m_Queries.end())
	//{
	//	m_Queries.erase(iter);
	//}
}

ISamplerState* RenderDeviceDX11::CreateSamplerState()
{
	std::shared_ptr<ISamplerState> sampler = std::make_shared<SamplerStateDX11>(this);
	m_Samplers.push_back(sampler);

	return sampler.get();
}

void RenderDeviceDX11::DestroySampler(ISamplerState* sampler)
{
	//auto& iter = std::find(m_Samplers.begin(), m_Samplers.end(), sampler);
	//if (iter != m_Samplers.end())
	//{
	//	m_Samplers.erase(iter);
	//}
}

IPipelineState* RenderDeviceDX11::CreatePipelineState()
{
	std::shared_ptr<IPipelineState> pPipeline = std::make_shared<PipelineStateDX11>(this);
	m_Pipelines.push_back(pPipeline);
	return pPipeline.get();
}

void RenderDeviceDX11::DestoryPipelineState(IPipelineState* pipeline)
{
	//auto& iter = std::find(m_Pipelines.begin(), m_Pipelines.end(), pipeline);
	//if (iter != m_Pipelines.end())
	//{
	//	m_Pipelines.erase(iter);
	//}
}

IBuffer* RenderDeviceDX11::CreateVoidVertexBuffer(const void * data, uint32 count, uint32 offset, uint32 stride)
{
	std::shared_ptr<IBuffer>buffer = std::make_shared<BufferDX11>(this, D3D11_BIND_VERTEX_BUFFER, data, count, offset, stride);
	m_Buffers.push_back(buffer);

	return buffer.get();
}

void RenderDeviceDX11::DestroyVertexBuffer(IBuffer* buffer)
{
	DestroyBufferD3D11(buffer);
}

IBuffer* RenderDeviceDX11::CreateVoidIndexBuffer(const void * data, uint32 count, uint32 offset, uint32 stride)
{
	std::shared_ptr<IBuffer> buffer = std::make_shared<BufferDX11>(this, D3D11_BIND_INDEX_BUFFER, data, count, offset, stride);
	m_Buffers.push_back(buffer);

	return buffer.get();
}

void RenderDeviceDX11::DestroyIndexBuffer(IBuffer* buffer)
{
	DestroyBufferD3D11(buffer);
}

IConstantBuffer* RenderDeviceDX11::CreateConstantBuffer(const void* data, size_t size)
{
	std::shared_ptr<IConstantBuffer> buffer = std::make_shared<ConstantBufferDX11>(this, size);
	if (data)
	{
		buffer->Set(data, size);
	}

	m_Buffers.push_back(buffer);

	return buffer.get();
}

void RenderDeviceDX11::DestroyConstantBuffer(IConstantBuffer* buffer)
{
	DestroyBufferD3D11(buffer);
}

//--

IStructuredBuffer* RenderDeviceDX11::CreateStructuredBuffer(void* data, uint32 count, uint32 stride, CPUAccess cpuAccess, bool gpuWrite)
{
	std::shared_ptr<IStructuredBuffer> buffer = std::make_shared<StructuredBufferDX11>(this, 0, data, count, stride, cpuAccess, gpuWrite);
	m_Buffers.push_back(buffer);

	return buffer.get();
}

void RenderDeviceDX11::DestroyStructuredBuffer(IStructuredBuffer* buffer)
{
	DestroyBufferD3D11(buffer);
}