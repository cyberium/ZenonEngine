#include "stdafx.h"

// General
#include "D3D9_To_D3D11.h"

struct VertexFormatPairs
{
	ECustomVertexElementType	CustomVertexElementType;
	DXGI_FORMAT	m_dx10FMT;
};

VertexFormatPairs	VertexFormatList[] =
{
	{ ECustomVertexElementType::FLOAT1,	DXGI_FORMAT_R32_FLOAT },
	{ ECustomVertexElementType::FLOAT2,	DXGI_FORMAT_R32G32_FLOAT },
	{ ECustomVertexElementType::FLOAT3,	DXGI_FORMAT_R32G32B32_FLOAT },
	{ ECustomVertexElementType::FLOAT4,	DXGI_FORMAT_R32G32B32A32_FLOAT },
	{ ECustomVertexElementType::D3DCOLOR,	DXGI_FORMAT_R8G8B8A8_UNORM },	// Warning. Explicit RGB component swizzling is nesessary	//	Not available 
	{ ECustomVertexElementType::UBYTE4,	DXGI_FORMAT_R8G8B8A8_UINT },	// Note: Shader gets UINT values, but if Direct3D 9 style integral floats are needed (0.0f, 1.0f... 255.f), UINT can just be converted to float32 in shader. 
	{ ECustomVertexElementType::SHORT2,	DXGI_FORMAT_R16G16_SINT },		// Note: Shader gets SINT values, but if Direct3D 9 style integral floats are needed, SINT can just be converted to float32 in shader. 
	{ ECustomVertexElementType::SHORT4,		DXGI_FORMAT_R16G16B16A16_SINT },// Note: Shader gets SINT values, but if Direct3D 9 style integral floats are needed, SINT can just be converted to float32 in shader. 
	{ ECustomVertexElementType::UBYTE4N,  DXGI_FORMAT_R8G8B8A8_UNORM },
	{ ECustomVertexElementType::SHORT2N,  DXGI_FORMAT_R16G16_SNORM },
	{ ECustomVertexElementType::SHORT4N,  DXGI_FORMAT_R16G16B16A16_SNORM },
	{ ECustomVertexElementType::USHORT2N, DXGI_FORMAT_R16G16_UNORM },
	{ ECustomVertexElementType::USHORT4N, DXGI_FORMAT_R16G16B16A16_UNORM },
	{ ECustomVertexElementType::FLOAT16_2,DXGI_FORMAT_R16G16_FLOAT },
	{ ECustomVertexElementType::FLOAT16_4,DXGI_FORMAT_R16G16B16A16_FLOAT }
};

DXGI_FORMAT	ConvertVertexFormat(ECustomVertexElementType CustomVertexElementType)
{
	int arrayLength = sizeof(VertexFormatList) / sizeof(VertexFormatList[0]);
	for (int i = 0; i < arrayLength; ++i)
	{
		if (VertexFormatList[i].CustomVertexElementType == CustomVertexElementType)
			return VertexFormatList[i].m_dx10FMT;
	}

	_ASSERT_EXPR(false, "ConvertVertexFormat didn't find appropriate dx10 vertex format!");
	return DXGI_FORMAT_UNKNOWN;
}

struct VertexSemanticPairs
{
	ECustomVertexElementUsage	m_dx9Semantic;
	LPCSTR			m_dx10Semantic;
};

VertexSemanticPairs	VertexSemanticList[] =
{
	{ ECustomVertexElementUsage::POSITION,		"POSITION" },		// 0
	{ ECustomVertexElementUsage::BLENDWEIGHT,	"BLENDWEIGHT" },	// 1
	{ ECustomVertexElementUsage::BLENDINDICES,	"BLENDINDICES" },	// 2
	{ ECustomVertexElementUsage::NORMAL,		"NORMAL" },			// 3
	{ ECustomVertexElementUsage::PSIZE,			"PSIZE" },			// 4
	{ ECustomVertexElementUsage::TEXCOORD,		"TEXCOORD" },		// 5
	{ ECustomVertexElementUsage::TANGENT,		"TANGENT" },		// 6
	{ ECustomVertexElementUsage::BINORMAL,		"BINORMAL" },		// 7
	{ ECustomVertexElementUsage::POSITIONT,		"POSITIONT" },		// 8
	{ ECustomVertexElementUsage::COLOR,			"COLOR" },			// 9
};

LPCSTR ConvertSemantic(ECustomVertexElementUsage Semantic)
{
	int arrayLength = sizeof(VertexSemanticList) / sizeof(VertexSemanticList[0]);
	for (int i = 0; i < arrayLength; ++i)
	{
		if (VertexSemanticList[i].m_dx9Semantic == Semantic)
			return VertexSemanticList[i].m_dx10Semantic;
	}

	_ASSERT_EXPR(false, "ConvertSemantic didn't find appropriate dx10 input semantic!");
	return NULL;
}

void ConvertVertexDeclaration(const std::vector<SCustomVertexElement>& CustomElements, std::vector<D3D11_INPUT_ELEMENT_DESC>& declOut)
{
	int customElementsSize = CustomElements.size();
	declOut.resize(customElementsSize + 1);

	for (int i = 0; i < customElementsSize; ++i)
	{
		const SCustomVertexElement& customElements = CustomElements[i];
		D3D11_INPUT_ELEMENT_DESC& descOut = declOut[i];

		descOut.SemanticName = ConvertSemantic((ECustomVertexElementUsage)customElements.Usage);
		descOut.SemanticIndex = customElements.UsageIndex;
		descOut.Format = ConvertVertexFormat((ECustomVertexElementType)customElements.Type);
		descOut.InputSlot = customElements.Stream;
		descOut.AlignedByteOffset = customElements.Offset;
		descOut.InputSlotClass = D3D11_INPUT_PER_VERTEX_DATA;
		descOut.InstanceDataStepRate = 0;
	}

	//std::memset(&declOut[iDeclSize], 0, sizeof(D3D11_INPUT_ELEMENT_DESC));
}
