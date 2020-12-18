#pragma once

//#define MAXD3DDECLUSAGEINDEX    15
//#define MAXD3DDECLLENGTH        64 // does not include "end" marker vertex element

enum class ZN_API ECustomVertexElementType : uint8
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

enum class ZN_API ECustomVertexElementUsage : uint8
{
	POSITION = 0,
	BLENDWEIGHT,
	BLENDINDICES,
	NORMAL,
	PSIZE,
	TEXCOORD,
	TANGENT,
	BINORMAL,
	TESSFACTOR,
	POSITIONT,
	COLOR ,

	InstanceID
};

struct ZN_API SCustomInputElement
{
	uint32                    Slot;
	uint32                    Offset;
	ECustomVertexElementType  Type;       // Data type
	ECustomVertexElementUsage Usage;      // Semantics
	uint8                     UsageIndex; // Semantic index
};

//size_t ZN_API CustomElementsLength(SCustomInputElement * elements);
//size_t ZN_API CustomElementsVertexSize(SCustomInputElement * elements, size_t Cnt, DWORD Stream);
