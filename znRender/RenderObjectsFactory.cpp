#include "stdafx.h"

// General
#include "RenderObjectsFactory.h"

CRenderObjectsFactory::CRenderObjectsFactory()
	: m_RenderObjectIDCntr(0)
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



//
// Protected
//
RenderObjectID CRenderObjectsFactory::GenerateRenderObjectID()
{
	return (m_RenderObjectIDCntr++);
}
