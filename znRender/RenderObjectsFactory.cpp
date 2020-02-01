#include "stdafx.h"

// General
#include "RenderObjectsFactory.h"

CRenderObjectsFactory::CRenderObjectsFactory()
	: m_RenderObjectIDCntr(0)
{
}

CRenderObjectsFactory::~CRenderObjectsFactory()
{
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



//
// Protected
//
RenderObjectID CRenderObjectsFactory::GenerateRenderObjectID()
{
	return (m_RenderObjectIDCntr++);
}
