#include "stdafx.h"

// General
#include "RenderDeviceBase.h"

// Additional
#include "RenderPrimitivesFactory.h"

CRenderDeviceBase::CRenderDeviceBase()
{
	m_RenderPrimitivesFactory = std::make_unique<CRenderPrimitivesFactory>(*this);
}

CRenderDeviceBase::~CRenderDeviceBase()
{
}



//
// IRenderDevice
//
IRenderPrimitivesFactory& CRenderDeviceBase::GetPrimitivesFactory() const
{
	return *m_RenderPrimitivesFactory;
}

IRenderObjectsFactory& CRenderDeviceBase::GetObjectsFactory() const
{
	return *m_RenderObjectsFactory;
}
