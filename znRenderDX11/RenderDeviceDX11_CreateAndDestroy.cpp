#include "stdafx.h"

// General
#include "RenderDeviceDX11.h"

// Additional
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

std::shared_ptr<IShader> RenderDeviceDX11::CreateShader(EShaderType type, const std::string& fileName, const IShader::ShaderMacros& shaderMacros, const std::string& entryPoint, const std::string& profile, std::shared_ptr<IShaderInputLayout> _customLayout)
{
	std::string fullName = fileName + ShaderMacrosToString(shaderMacros) + entryPoint + profile;

	ShaderNameMap::iterator iter = m_ShadersByName.find(fullName);
	if (iter != m_ShadersByName.end())
		return iter->second;

	std::shared_ptr<IShader> pShader = std::make_shared<ShaderDX11>(this);
	pShader->LoadShaderFromFile(type, fileName, shaderMacros, entryPoint, profile, _customLayout);

	m_Shaders.push_back(pShader);
	m_ShadersByName.insert(ShaderNameMap::value_type(fullName, pShader));

	return pShader;
}

void RenderDeviceDX11::DestroyShader(std::shared_ptr<IShader> shader)
{
	ShaderList::iterator iter = std::find(m_Shaders.begin(), m_Shaders.end(), shader);
	if (iter != m_Shaders.end())
	{
		m_Shaders.erase(iter);
	}
}

std::shared_ptr<IMesh> RenderDeviceDX11::CreateMesh()
{
	std::shared_ptr<IMesh> mesh = std::make_shared<MeshDX11>(this);
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

std::shared_ptr<ITexture> RenderDeviceDX11::CreateTexture2D(const std::string& fileName)
{
	TextureNameMap::iterator iter = m_TexturesByName.find(fileName);
	if (iter != m_TexturesByName.end())
	{
		return iter->second;
	}

	std::shared_ptr<ITexture> texture = std::make_shared<TextureDX11>(this);
	if (!texture->LoadTexture2D(fileName))
		return m_pDefaultTexture;

	m_Textures.push_back(texture);
	m_TexturesByName.insert(TextureNameMap::value_type(fileName, texture));

	return texture;
}

std::shared_ptr<ITexture> RenderDeviceDX11::CreateTextureCube(const std::string& fileName)
{
	TextureNameMap::iterator iter = m_TexturesByName.find(fileName);
	if (iter != m_TexturesByName.end())
	{
		return iter->second;
	}

	std::shared_ptr<ITexture> texture = std::make_shared<TextureDX11>(this);
	texture->LoadTextureCube(fileName);

	m_Textures.push_back(texture);
	m_TexturesByName.insert(TextureNameMap::value_type(fileName, texture));

	return texture;

}

std::shared_ptr<ITexture> RenderDeviceDX11::CreateTexture2D(uint16_t width, uint16_t height, uint16_t slices, const ITexture::TextureFormat& format, CPUAccess cpuAccess, bool gpuWrite)
{
	std::shared_ptr<ITexture> texture = std::make_shared<TextureDX11>(this, width, height, slices, format, cpuAccess, gpuWrite);
	m_Textures.push_back(texture);

	return texture;
}

std::shared_ptr<ITexture> RenderDeviceDX11::CreateTextureCube(uint16_t size, uint16_t numCubes, const ITexture::TextureFormat& format, CPUAccess cpuAccess, bool gpuWrite)
{
	std::shared_ptr<ITexture> texture = std::make_shared<TextureDX11>(this, size, numCubes, format, cpuAccess, gpuWrite);
	m_Textures.push_back(texture);

	return texture;
}

std::shared_ptr<ITexture> RenderDeviceDX11::GetDefaultTexture() const
{
	return m_pDefaultTexture;
}

std::shared_ptr<ITexture> RenderDeviceDX11::CreateTexture()
{
	std::shared_ptr<ITexture> texture = std::make_shared<TextureDX11>(this);
	m_Textures.push_back(texture);

	return texture;
}

void RenderDeviceDX11::DestroyTexture(std::shared_ptr<ITexture> texture)
{
	TextureList::iterator iter = std::find(m_Textures.begin(), m_Textures.end(), texture);
	if (iter != m_Textures.end())
	{
		m_Textures.erase(iter);
	}

	TextureNameMap::iterator iter2 = std::find_if(m_TexturesByName.begin(), m_TexturesByName.end(), [=](TextureNameMap::value_type val) { return (val.second == texture); });
	if (iter2 != m_TexturesByName.end())
	{
		m_TexturesByName.erase(iter2);
	}
}

std::shared_ptr<IMaterial> RenderDeviceDX11::CreateMaterial(size_t Size)
{
	std::shared_ptr<IMaterial> pMaterial = std::make_shared<MaterialDX11>(this, Size);
	m_Materials.push_back(pMaterial);
	return pMaterial;
}

void RenderDeviceDX11::DestroyMaterial(std::shared_ptr<IMaterial> material)
{
	MaterialList::iterator iter = std::find(m_Materials.begin(), m_Materials.end(), material);
	if (iter != m_Materials.end())
	{
		m_Materials.erase(iter);
	}
}


std::shared_ptr<IRenderTarget> RenderDeviceDX11::CreateRenderTarget()
{
	std::shared_ptr<RenderTargetDX11> renderTarget = std::make_shared<RenderTargetDX11>(this);
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

std::shared_ptr<IQuery> RenderDeviceDX11::CreateQuery(IQuery::QueryType queryType, uint8_t numBuffers)
{
	std::shared_ptr<IQuery> query = std::make_shared<QueryDX11>(this, queryType, numBuffers);
	m_Queries.push_back(query);

	return query;
}

void RenderDeviceDX11::DestoryQuery(std::shared_ptr<IQuery> query)
{
	QueryList::iterator iter = std::find(m_Queries.begin(), m_Queries.end(), query);
	if (iter != m_Queries.end())
	{
		m_Queries.erase(iter);
	}
}

std::shared_ptr<ISamplerState> RenderDeviceDX11::CreateSamplerState()
{
	std::shared_ptr<ISamplerState> sampler = std::make_shared<SamplerStateDX11>(this);
	m_Samplers.push_back(sampler);

	return sampler;
}

void RenderDeviceDX11::DestroySampler(std::shared_ptr<ISamplerState> sampler)
{
	SamplerList::iterator iter = std::find(m_Samplers.begin(), m_Samplers.end(), sampler);
	if (iter != m_Samplers.end())
	{
		m_Samplers.erase(iter);
	}
}

std::shared_ptr<IPipelineState> RenderDeviceDX11::CreatePipelineState()
{
	std::shared_ptr<IPipelineState> pPipeline = std::make_shared<PipelineStateDX11>(this);
	m_Pipelines.push_back(pPipeline);

	return pPipeline;
}

void RenderDeviceDX11::DestoryPipelineState(std::shared_ptr<IPipelineState> pipeline)
{
	PipelineList::iterator iter = std::find(m_Pipelines.begin(), m_Pipelines.end(), pipeline);
	if (iter != m_Pipelines.end())
	{
		m_Pipelines.erase(iter);
	}
}

std::shared_ptr<IBuffer> RenderDeviceDX11::CreateVoidVertexBuffer(const void * data, uint32 count, uint32 offset, uint32 stride)
{
	std::shared_ptr<IBuffer> buffer = std::make_shared<BufferDX11>(this, D3D11_BIND_VERTEX_BUFFER, data, count, offset, stride);
	m_Buffers.push_back(buffer);

	return buffer;
}

void RenderDeviceDX11::DestroyVertexBuffer(std::shared_ptr<IBuffer> buffer)
{
	DestroyBufferD3D11(buffer);
}

std::shared_ptr<IBuffer> RenderDeviceDX11::CreateVoidIndexBuffer(const void * data, uint32 count, uint32 offset, uint32 stride)
{
	std::shared_ptr <IBuffer> buffer = std::make_shared<BufferDX11>(this, D3D11_BIND_INDEX_BUFFER, data, count, offset, stride);
	m_Buffers.push_back(buffer);

	return buffer;
}

void RenderDeviceDX11::DestroyIndexBuffer(std::shared_ptr<IBuffer> buffer)
{
	DestroyBufferD3D11(buffer);
}

std::shared_ptr<IConstantBuffer> RenderDeviceDX11::CreateConstantBuffer(const void* data, size_t size)
{
	std::shared_ptr<IConstantBuffer> buffer = std::make_shared<ConstantBufferDX11>(this, size);
	if (data)
	{
		buffer->Set(data, size);
	}

	m_Buffers.push_back(buffer);

	return buffer;
}

void RenderDeviceDX11::DestroyConstantBuffer(std::shared_ptr<IConstantBuffer> buffer)
{
	DestroyBufferD3D11(buffer);
}

//--

std::shared_ptr<IStructuredBuffer> RenderDeviceDX11::CreateStructuredBuffer(void* data, uint32 count, uint32 stride, CPUAccess cpuAccess, bool gpuWrite)
{
	std::shared_ptr<IStructuredBuffer> buffer = std::make_shared<StructuredBufferDX11>(this, 0, data, count, stride, cpuAccess, gpuWrite);
	m_Buffers.push_back(buffer);

	return buffer;
}

void RenderDeviceDX11::DestroyStructuredBuffer(std::shared_ptr<IStructuredBuffer> buffer)
{
	DestroyBufferD3D11(buffer);
}