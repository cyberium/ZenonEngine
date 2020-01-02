#pragma once

#include "GeometryDX11.h"

class ZN_API MeshDX11 
	: public MeshBase
{
public:
	                                                MeshDX11(ID3D11Device2 * pDevice);
	virtual                                         ~MeshDX11();

	virtual void                                    SetPrimitiveTopology(PrimitiveTopology _topology);

	virtual bool                                    Render(const RenderEventArgs* renderArgs, const IConstantBuffer* perObject, SGeometryPartParams GeometryPartParams);

private: // DirectX
	ATL::CComPtr<ID3D11Device2>                     m_pDevice;
	ATL::CComPtr<ID3D11DeviceContext2>              m_pDeviceContext;
};