#pragma once

//#define MAXD3DDECLUSAGEINDEX    15
//#define MAXD3DDECLLENGTH        64 // does not include "end" marker vertex element

enum class ZN_API ECustomVertexElementType : uint8_t
{
	FLOAT1 = 0,
	FLOAT2,
	FLOAT3,
	FLOAT4,

	UINT1,
	UINT2,
	UINT3,
	UINT4,

	BYTE4,
	BYTE4N,
	UBYTE4,
	UBYTE4N,

	SHORT2,
	SHORT2N,
	SHORT4,
	SHORT4N,

	USHORT2,
	USHORT2N,
	USHORT4,
	USHORT4N,

	FLOAT16_2,
	FLOAT16_4,	
};

enum class ZN_API ECustomVertexElementUsage : uint8_t
{
	POSITION = 0,
	BLENDWEIGHT = 1,
	BLENDINDICES = 2,
	NORMAL = 3,
	PSIZE = 4,
	TEXCOORD = 5,
	TANGENT = 6,
	BINORMAL = 7,
	TESSFACTOR = 8,
	POSITIONT = 9,
	COLOR = 10
};

struct ZN_API SCustomInputElement
{
	UINT                      Slot;
	UINT                      Offset;
	ECustomVertexElementType  Type;       // Data type
	ECustomVertexElementUsage Usage;      // Semantics
	BYTE                      UsageIndex; // Semantic index
};

//size_t ZN_API CustomElementsLength(SCustomInputElement * elements);
//size_t ZN_API CustomElementsVertexSize(SCustomInputElement * elements, size_t Cnt, DWORD Stream);
