#pragma once

#include <znCore.h>
#pragma comment(lib, "znCore.lib")

#include <znRender.h>
#pragma comment(lib, "znRender.lib")

// COM
#include <comdef.h>
#include <atlbase.h>

// DIRECTX
#include <d3d11_2.h>
#pragma comment(lib, "d3d11.lib")

#include <d3dcompiler.h>
#pragma comment(lib, "d3dcompiler.lib")

#include <dxgi1_3.h>
#pragma comment(lib, "dxgi.lib")
#pragma comment(lib, "dxguid.lib")

ZN_INTERFACE IRenderDeviceDX11
{
	virtual ATL::CComPtr<ID3D11Device2>                     GetDevice() const = 0;
	virtual ATL::CComPtr<ID3D11DeviceContext2>              GetDeviceContext() const = 0;
};