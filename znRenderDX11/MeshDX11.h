#pragma once

#include "GeometryDX11.h"

class ZN_API MeshDX11 
	: public MeshBase
{
public:
	MeshDX11(IRenderDeviceDX11* RenderDeviceD3D11);
	virtual	~MeshDX11();

	virtual void SetPrimitiveTopology(PrimitiveTopology _topology);

	virtual bool Render(const RenderEventArgs* renderArgs, const IConstantBuffer* perObject, SGeometryPartParams GeometryPartParams);
};