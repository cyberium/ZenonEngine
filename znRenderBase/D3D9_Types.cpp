#include "stdafx.h"

// General
#include "D3D9_Types.h"

uint32 D3DVERTEXELEMENT9ToElementSize2(D3DVERTEXELEMENT9 _elem)
{
	switch (_elem.Type)
	{
	case D3DDECLTYPE_FLOAT1:
		return 4;
	case D3DDECLTYPE_FLOAT2:
		return 8;
	case D3DDECLTYPE_FLOAT3:
		return 12;
	case D3DDECLTYPE_FLOAT4:
		return 16;
	case D3DDECLTYPE_D3DCOLOR:
		return 4;
	case D3DDECLTYPE_UBYTE4:
		return 4;
	case D3DDECLTYPE_SHORT2:
		return 4;
	case D3DDECLTYPE_SHORT4:
		return 8;
	case D3DDECLTYPE_UBYTE4N:
		return 4;
	case D3DDECLTYPE_SHORT2N:
		return 4;
	case D3DDECLTYPE_SHORT4N:
		return 8;
	case D3DDECLTYPE_USHORT2N:
		return 4;
	case D3DDECLTYPE_USHORT4N:
		return 8;
	case D3DDECLTYPE_UDEC3:
		fail1();
		return UINT32_MAX;
	case D3DDECLTYPE_DEC3N:
		fail1();
		return UINT32_MAX;
	case D3DDECLTYPE_FLOAT16_2:
		return 4;
	case D3DDECLTYPE_FLOAT16_4:
		return 8;
	case D3DDECLTYPE_UNUSED:
		fail1();
		return UINT32_MAX;
	}

	fail1();
	return UINT32_MAX;
}

uint32 D3DXGetDeclLength(D3DVERTEXELEMENT9 * dcl)
{
	for (uint32 k = 0; k < MAXD3DDECLLENGTH + 1; k++)
	{
		D3DVERTEXELEMENT9 e = dcl[k];
		if (e.Stream == 255)
			return k;

		Log::Info("%d %d %d %d %d %d ", e.Stream, e.Offset, e.Type, e.Method, e.Usage, e.UsageIndex);
	}

	fail1();
	return UINT32_MAX;
}

uint32 D3DXGetDeclVertexSize(D3DVERTEXELEMENT9 * dcl, DWORD Cnt, DWORD Stream)
{
	uint32 size = 0;

	for (uint32 k = 0; k < Cnt; k++)
	{
		D3DVERTEXELEMENT9 e = dcl[k];
		if (e.Stream == Stream)
			size += D3DVERTEXELEMENT9ToElementSize2(e);
	}

	return size;
}