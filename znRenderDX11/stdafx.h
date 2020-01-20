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

ZN_INTERFACE IRenderDeviceDX11
{
	virtual std::shared_ptr<IRenderDevice> GetDevice() = 0;
	virtual ID3D11Device2* GetDeviceD3D11() = 0;
	virtual ID3D11DeviceContext2* GetDeviceContextD3D11() = 0;
};