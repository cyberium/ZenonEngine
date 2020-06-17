#pragma once

#include "Interfaces/__Interfaces.h"

#include <znCore.h>
#pragma comment(lib, "znCore.lib")

#include <znRender.h>
#pragma comment(lib, "znRender.lib")

// COM
#define _ATL_DISABLE_NOTHROW_NEW
#include <comdef.h>
#include <atlbase.h>

// DIRECTX
#include <d3d11_4.h>
#pragma comment(lib, "d3d11.lib")

#include <d3dcompiler.h>
#pragma comment(lib, "d3dcompiler.lib")

#include <dxgi1_6.h>
#pragma comment(lib, "dxgi.lib")
#pragma comment(lib, "dxguid.lib")

ZN_INTERFACE ZN_API IRenderDeviceDX11
	: public IRenderDevice
{
	virtual ~IRenderDeviceDX11() {}

	virtual ID3D11Device4* GetDeviceD3D11() = 0;
	virtual ID3D11DeviceContext3* GetDeviceContextD3D11() = 0;
};

void DoCheckHR(HRESULT hr, ATL::CComBSTR Message = L"");

#define CHECK_HR(x) \
	{ \
		HRESULT __hr = (x); \
		if (FAILED(__hr)) \
			DoCheckHR(__hr); \
	}

#define CHECK_HR_MSG(x, y) \
	{ \
		HRESULT __hr = (x); \
		if (FAILED(__hr)) \
			DoCheckHR(__hr, y); \
	}