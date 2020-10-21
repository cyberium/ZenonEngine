#include "stdafx.h"

// General
#include "CustomElementsToDX11.h"

namespace
{
	struct VertexFormatPairs
	{
		ECustomVertexElementType  CustomElementType;
		UINT                      DX11ElementSize;
		DXGI_FORMAT	              DX11Format;
	};

	const VertexFormatPairs VertexFormatList[] =
	{
		{ ECustomVertexElementType::FLOAT1,	   4, DXGI_FORMAT_R32_FLOAT },
		{ ECustomVertexElementType::FLOAT2,	   8, DXGI_FORMAT_R32G32_FLOAT },
		{ ECustomVertexElementType::FLOAT3,	  12, DXGI_FORMAT_R32G32B32_FLOAT },
		{ ECustomVertexElementType::FLOAT4,	  16, DXGI_FORMAT_R32G32B32A32_FLOAT },

		{ ECustomVertexElementType::BYTE4,	   4, DXGI_FORMAT_R8G8B8A8_SINT },
		{ ECustomVertexElementType::BYTE4N,    4, DXGI_FORMAT_R8G8B8A8_SNORM },

		{ ECustomVertexElementType::UBYTE4,	   4, DXGI_FORMAT_R8G8B8A8_UINT },
		{ ECustomVertexElementType::UBYTE4N,   4, DXGI_FORMAT_R8G8B8A8_UNORM },
		{ ECustomVertexElementType::D3DCOLOR,  4, DXGI_FORMAT_R8G8B8A8_UNORM },

		{ ECustomVertexElementType::SHORT2,	   4, DXGI_FORMAT_R16G16_SINT },
		{ ECustomVertexElementType::SHORT2N,   4, DXGI_FORMAT_R16G16_SNORM },
		{ ECustomVertexElementType::SHORT4,	   8, DXGI_FORMAT_R16G16B16A16_SINT },
		{ ECustomVertexElementType::SHORT4N,   4, DXGI_FORMAT_R16G16B16A16_SNORM },

		{ ECustomVertexElementType::USHORT2,   4, DXGI_FORMAT_R16G16_UINT },
		{ ECustomVertexElementType::USHORT2N,  4, DXGI_FORMAT_R16G16_UNORM },
		{ ECustomVertexElementType::USHORT4,   8, DXGI_FORMAT_R16G16B16A16_UINT },
		{ ECustomVertexElementType::USHORT4N,  8, DXGI_FORMAT_R16G16B16A16_UNORM },

		{ ECustomVertexElementType::FLOAT16_2, 4, DXGI_FORMAT_R16G16_FLOAT },
		{ ECustomVertexElementType::FLOAT16_4, 8, DXGI_FORMAT_R16G16B16A16_FLOAT }
	};

	const auto cVertexFormatListLength = sizeof(VertexFormatList) / sizeof(VertexFormatList[0]);

	ECustomVertexElementType DX11ToCustom_InputElementFormat(DXGI_FORMAT DX11ElementType)
	{
		for (auto i = 0; i < cVertexFormatListLength; ++i)
			if (VertexFormatList[i].DX11Format == DX11ElementType)
				return VertexFormatList[i].CustomElementType;
		throw CznRenderException("ConvertVertexFormat didn't find appropriate Custom Element!");
	}

	UINT DX11ToCustom_InputElementSize(DXGI_FORMAT DX11ElementType)
	{
		for (auto i = 0; i < cVertexFormatListLength; ++i)
			if (VertexFormatList[i].DX11Format == DX11ElementType)
				return VertexFormatList[i].DX11ElementSize;
		throw CznRenderException("ConvertVertexFormat didn't find appropriate Custom Element!");
	}

	DXGI_FORMAT	CustomToDX11_InputElementFormat(ECustomVertexElementType CustomVertexElementType)
	{
		for (auto i = 0; i < cVertexFormatListLength; ++i)
			if (VertexFormatList[i].CustomElementType == CustomVertexElementType)
				return VertexFormatList[i].DX11Format;
		throw CznRenderException("ConvertVertexFormat didn't find appropriate DX11 Element!");
	}

	UINT CustomToDX11_InputElementSize(ECustomVertexElementType CustomVertexElementType)
	{
		for (auto i = 0; i < cVertexFormatListLength; ++i)
			if (VertexFormatList[i].CustomElementType == CustomVertexElementType)
				return VertexFormatList[i].DX11ElementSize;
		throw CznRenderException("ConvertVertexFormat didn't find appropriate DX11 Element!");
	}
}






struct VertexSemanticPairs
{
	ECustomVertexElementUsage CustomUsage;
	LPCSTR DX11Usage;
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

const auto cVertexSemanticListLength = sizeof(VertexFormatList) / sizeof(VertexFormatList[0]);

ECustomVertexElementUsage DX11ToCustom_InputElementUsage(LPCSTR Semantic)
{
	for (auto i = 0; i < cVertexSemanticListLength; ++i)
		if (::_stricmp(VertexSemanticList[i].DX11Usage, Semantic) == 0)
			return VertexSemanticList[i].CustomUsage;
	throw CznRenderException("ConvertVertexFormat didn't find appropriate Custom Semantic!");
}

LPCSTR CustomToDX11_InputElementUsage(ECustomVertexElementUsage Semantic)
{
	for (auto i = 0; i < cVertexSemanticListLength; ++i)
		if (VertexSemanticList[i].CustomUsage == Semantic)
			return VertexSemanticList[i].DX11Usage;
	throw CznRenderException("ConvertVertexFormat didn't find appropriate DX11 Semantic!");
}






SCustomInputElement CustomElements::DX11::DX11ToCustom(const D3D11_INPUT_ELEMENT_DESC& DX11InputElement)
{
	SCustomInputElement inputElement = {};
	inputElement.Slot = DX11InputElement.InputSlot;
	inputElement.Offset = DX11InputElement.AlignedByteOffset;
	inputElement.Type = DX11ToCustom_InputElementFormat(DX11InputElement.Format);
	inputElement.Usage = DX11ToCustom_InputElementUsage(DX11InputElement.SemanticName);
	inputElement.UsageIndex = DX11InputElement.SemanticIndex;
	return inputElement;
}

D3D11_INPUT_ELEMENT_DESC CustomElements::DX11::CustomToDX11(const SCustomInputElement& CustomInputElement)
{
	D3D11_INPUT_ELEMENT_DESC inputElement = {};
	inputElement.SemanticName = CustomToDX11_InputElementUsage(CustomInputElement.Usage);
	inputElement.SemanticIndex = CustomInputElement.UsageIndex;
	inputElement.Format = CustomToDX11_InputElementFormat(CustomInputElement.Type);
	inputElement.InputSlot = CustomInputElement.Slot;
	inputElement.AlignedByteOffset = CustomInputElement.Offset;
	inputElement.InputSlotClass = D3D11_INPUT_PER_VERTEX_DATA;
	inputElement.InstanceDataStepRate = 0;
	return inputElement;
}




void CustomElements::DX11::MergeCustom(std::vector<SCustomInputElement>& CustomInputElements)
{
	UINT offsetCnt = 0;
	for (auto& e : CustomInputElements)
	{
		e.Slot = 0;
		e.Offset = offsetCnt;
		offsetCnt += CustomToDX11_InputElementSize(e.Type);
	}
}

std::vector<SCustomInputElement> CustomElements::DX11::DX11ToCustom(const std::vector<D3D11_INPUT_ELEMENT_DESC>& CustomInputElements)
{
	std::vector<SCustomInputElement> customInputElements;
	for (const auto& e : CustomInputElements)
		customInputElements.push_back(CustomElements::DX11::DX11ToCustom(e));
	return customInputElements;
}

std::vector<D3D11_INPUT_ELEMENT_DESC> CustomElements::DX11::CustomToDX11(const std::vector<SCustomInputElement>& CustomInputElements)
{
	std::vector<D3D11_INPUT_ELEMENT_DESC> dx11InputElements;
	for (const auto& e : CustomInputElements)
		dx11InputElements.push_back(CustomElements::DX11::CustomToDX11(e));
	return dx11InputElements;
}
