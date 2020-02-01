#pragma once

//#define MAXD3DDECLUSAGEINDEX    15
//#define MAXD3DDECLLENGTH        64 // does not include "end" marker vertex element

enum class ZN_API ECustomVertexElementType : uint8_t
{
	FLOAT1 = 0,
	FLOAT2 = 1,
	FLOAT3 = 2,
	FLOAT4 = 3,
	D3DCOLOR = 4,
	UBYTE4 = 5,
	SHORT2 = 6,
	SHORT4 = 7,
	UBYTE4N = 8,
	SHORT2N = 9,
	SHORT4N = 10,
	USHORT2N = 11,
	USHORT4N = 12,
	UDEC3 = 13,
	DEC3N = 14,
	FLOAT16_2 = 15,
	FLOAT16_4 = 16,
	UNUSED = 17
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
	COLOR = 10,
	FOG = 11,
	DEPTH = 12,
	SAMPLE = 13
};

struct ZN_API SCustomVertexElement
{
	WORD                      Stream;     // Stream index
	WORD                      Offset;     // Offset in the stream in bytes
	ECustomVertexElementType  Type;       // Data type
	ECustomVertexElementUsage Usage;      // Semantics
	BYTE                      UsageIndex; // Semantic index
};

//size_t ZN_API CustomElementsLength(SCustomVertexElement * elements);
//size_t ZN_API CustomElementsVertexSize(SCustomVertexElement * elements, size_t Cnt, DWORD Stream);
