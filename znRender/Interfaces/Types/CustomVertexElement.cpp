#include "stdafx.h"

// General
#include "CustomVertexElement.h"

uint32 CustomElementToElementSize(const SCustomVertexElement& _elem)
{
	switch (_elem.Type)
	{
	case ECustomVertexElementType::FLOAT1:
		return 4;
	case ECustomVertexElementType::FLOAT2:
		return 8;
	case ECustomVertexElementType::FLOAT3:
		return 12;
	case ECustomVertexElementType::FLOAT4:
		return 16;
	case ECustomVertexElementType::D3DCOLOR:
		return 4;
	case ECustomVertexElementType::UBYTE4:
		return 4;
	case ECustomVertexElementType::SHORT2:
		return 4;
	case ECustomVertexElementType::SHORT4:
		return 8;
	case ECustomVertexElementType::UBYTE4N:
		return 4;
	case ECustomVertexElementType::SHORT2N:
		return 4;
	case ECustomVertexElementType::SHORT4N:
		return 8;
	case ECustomVertexElementType::USHORT2N:
		return 4;
	case ECustomVertexElementType::USHORT4N:
		return 8;
	case ECustomVertexElementType::UDEC3:
		_ASSERT(false);
		return UINT32_MAX;
	case ECustomVertexElementType::DEC3N:
		_ASSERT(false);
		return UINT32_MAX;
	case ECustomVertexElementType::FLOAT16_2:
		return 4;
	case ECustomVertexElementType::FLOAT16_4:
		return 8;
	case ECustomVertexElementType::UNUSED:
		_ASSERT(false);
		return UINT32_MAX;
	}

	_ASSERT(false);
	return UINT32_MAX;
}

size_t CustomElementsLength(SCustomVertexElement * elements)
{
	for (uint32 k = 0; k < MAXD3DDECLLENGTH + 1; k++)
	{
		const SCustomVertexElement& e = elements[k];
		if (e.Stream == 255)
			return k;

		//Log::Info("%d %d %d %d %d %d ", e.Stream, e.Offset, e.Type, e.Method, e.Usage, e.UsageIndex);
	}

	_ASSERT(false);
	return UINT32_MAX;
}

size_t CustomElementsVertexSize(SCustomVertexElement * elements, size_t Cnt, DWORD Stream)
{
	uint32 size = 0;

	for (uint32 k = 0; k < Cnt; k++)
	{
		const SCustomVertexElement& e = elements[k];
		if (e.Stream == Stream)
			size += CustomElementToElementSize(e);
	}

	return size;
}