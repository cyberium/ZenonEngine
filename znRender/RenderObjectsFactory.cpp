#include "stdafx.h"

// General
#include "RenderObjectsFactory.h"

CRenderObjectsFactory::CRenderObjectsFactory(IRenderDevice& RenderDevice)
	: m_RenderDevice(RenderDevice)
	, m_RenderObjectIDCntr(0)
{
}

CRenderObjectsFactory::~CRenderObjectsFactory()
{
	ClearCache();
}



//
// IRenderObjectsFactory
//
bool CRenderObjectsFactory::HoldRenderObject(RenderObjectID ID) const
{
	return false;
}

bool CRenderObjectsFactory::UnholdRenderObject(RenderObjectID ID) const
{
	return false;
}

std::shared_ptr<IRenderObject> CRenderObjectsFactory::GetRenderObject(RenderObjectID ID) const
{
	return std::shared_ptr<IRenderObject>();
}

void CRenderObjectsFactory::ClearCache()
{
	m_Shaders.clear();
	m_ShadersByName.clear();

	m_Geometries.clear();
	m_Models.clear();

	m_Textures.clear();
	m_TexturesByName.clear();

	m_Materials.clear();
	m_RenderTargets.clear();
	m_Queries.clear();
	m_Samplers.clear();
	m_BlendStates.clear();
	m_DepthStencilStates.clear();
	m_RasterizerStates.clear();
	m_Pipelines.clear();
	m_Buffers.clear();
}

std::shared_ptr<IModel> CRenderObjectsFactory::LoadModel(const std::string & fileName)
{
	return std::shared_ptr<IModel>();
}

std::shared_ptr<ITexture> CRenderObjectsFactory::LoadTexture2D(const std::string& fileName)
{
	const auto& iter = m_TexturesByName.find(fileName);
	if (iter != m_TexturesByName.end())
	{
		if (std::shared_ptr<ITexture> texture = iter->second.lock())
		{
			return texture;
		}
		else
		{
			m_TexturesByName.erase(iter);
		}
	}

	std::shared_ptr<ITexture> object = CreateEmptyTexture();
	if (!object->LoadTexture2D(fileName))
		return m_RenderDevice.GetDefaultTexture();

	//m_Textures[GenerateRenderObjectID()] = object;
	m_TexturesByName[fileName] = object;

	return object;
}

std::shared_ptr<ITexture> CRenderObjectsFactory::LoadTextureCube(const std::string& fileName)
{
	const auto& iter = m_TexturesByName.find(fileName);
	if (iter != m_TexturesByName.end())
	{
		if (std::shared_ptr<ITexture> texture = iter->second.lock())
		{
			return texture;
		}
		else
		{
			m_TexturesByName.erase(iter);
		}
	}

	std::shared_ptr<ITexture> object = CreateEmptyTexture();
	object->LoadTextureCube(fileName);

	//m_Textures.insert(std::make_pair(GenerateRenderObjectID(), object));
	m_TexturesByName[fileName] = object;

	return object;
}



//
// Protected
//
RenderObjectID CRenderObjectsFactory::GenerateRenderObjectID()
{
	return (m_RenderObjectIDCntr++);
}
