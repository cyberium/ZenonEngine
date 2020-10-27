#include "stdafx.h"

// General
#include "ShaderInputLayoutDX11.h"

// Additional
#include "CustomElementsToDX11.h"


static InputSemantic gs_InvalidShaderSemantic;


// FORWARD BEGIN
DXGI_FORMAT GetDXGIFormat(const D3D11_SIGNATURE_PARAMETER_DESC& paramDesc);
// FORWARD END


ShaderInputLayoutDX11::ShaderInputLayoutDX11(IRenderDeviceDX11& RenderDeviceDX11)
	: m_RenderDeviceDX11(RenderDeviceDX11)
{
}

ShaderInputLayoutDX11::~ShaderInputLayoutDX11()
{
	m_pInputLayout.Release();
}



//
// IShaderInputLayout
//
bool ShaderInputLayoutDX11::HasSemantic(const BufferBinding& binding) const
{
	for (const auto& it : m_InputSemanticsDX11)
		if (it.second.Name == binding.Name && it.second.Index == binding.Index)
			return true;

	return false;
}

const InputSemantic& ShaderInputLayoutDX11::GetSemantic(const BufferBinding & binding) const
{
	for (auto& it : m_InputSemanticsDX11)
		if (it.second.Name == binding.Name && it.second.Index == binding.Index)
			return it.second;
	return gs_InvalidShaderSemantic;
}

UINT ShaderInputLayoutDX11::GetSemanticSlot(const BufferBinding & binding) const
{
	for (auto& it : m_InputSemanticsDX11)
		if ((::strcmp(it.second.Name.c_str(), binding.Name.c_str()) == 0) && it.second.Index == binding.Index)
			return it.first;
	return UINT_MAX;
}

size_t ShaderInputLayoutDX11::GetSemanticsCount() const
{
	return m_InputSemanticsDX11.size();
}

const InputSemantic & ShaderInputLayoutDX11::GetSemantic(size_t Index) const
{
	auto iter = m_InputSemanticsDX11.begin();
	std::advance(iter, Index);
	if (iter != m_InputSemanticsDX11.end())
		return iter->second;
	return gs_InvalidShaderSemantic;
}

UINT ShaderInputLayoutDX11::GetSemanticSlot(size_t Index) const
{
	auto iter = m_InputSemanticsDX11.begin();
	std::advance(iter, Index);
	if (iter != m_InputSemanticsDX11.end())
		return iter->first;
	return UINT_MAX;
}



//
// ShaderInputLayoutDX11
//
bool ShaderInputLayoutDX11::LoadFromReflector(ID3DBlob * pShaderBlob, ID3D11ShaderReflection * pReflector)
{
	// Query input parameters and build the input layout
	D3D11_SHADER_DESC shaderDescription = {};
	CHECK_HR_MSG(pReflector->GetDesc(&shaderDescription), L"Failed to get shader description from shader reflector.");

	std::vector<D3D11_INPUT_ELEMENT_DESC> inputElements;
	for (UINT i = 0; i < shaderDescription.InputParameters; ++i)
	{
		D3D11_SIGNATURE_PARAMETER_DESC parameterSignature = {};
		CHECK_HR(pReflector->GetInputParameterDesc(i, &parameterSignature));

		D3D11_INPUT_ELEMENT_DESC inputElement = {};
		inputElement.SemanticName = parameterSignature.SemanticName;
		inputElement.SemanticIndex = parameterSignature.SemanticIndex;
		inputElement.InputSlot = i; // TODO: If using interleaved arrays, then the input slot should be 0.  If using packed arrays, the input slot will vary.
		inputElement.AlignedByteOffset = D3D11_APPEND_ALIGNED_ELEMENT;
		inputElement.InputSlotClass = D3D11_INPUT_PER_VERTEX_DATA; // TODO: Figure out how to deal with per-instance data? .. Don't. Just use structured buffers to store per-instance data and use the SV_InstanceID as an index in the structured buffer.
		inputElement.InstanceDataStepRate = 0;
		inputElement.Format = GetDXGIFormat(parameterSignature);
		inputElements.push_back(inputElement);

		_ASSERT(inputElement.Format != DXGI_FORMAT_UNKNOWN);

		m_InputSemanticsDX11.insert(std::make_pair(i, InputSemanticDX11(inputElement.SemanticName, inputElement.SemanticIndex, inputElement.Format)));
	}

	_ASSERT(inputElements.size() > 0);


	//std::vector<SCustomInputElement> customInputElements = CustomElements::DX11::DX11ToCustom(inputElements);
	//CustomElements::DX11::MergeCustom(customInputElements);
	//std::vector<D3D11_INPUT_ELEMENT_DESC> dx11InputElements = CustomElements::DX11::CustomToDX11(customInputElements);

	CHECK_HR_MSG(m_RenderDeviceDX11.GetDeviceD3D11()->CreateInputLayout(inputElements.data(), (UINT)inputElements.size(), pShaderBlob->GetBufferPointer(), pShaderBlob->GetBufferSize(), &m_pInputLayout), L"Failed to create input layout.");

	return true;
}

bool ShaderInputLayoutDX11::LoadFromCustomElements(ID3DBlob * pShaderBlob, const std::vector<SCustomInputElement>& CustomElements)
{
	std::vector<D3D11_INPUT_ELEMENT_DESC> dx11InputElements = CustomElements::DX11::CustomToDX11(CustomElements);
	for (uint32 i = 0; i < dx11InputElements.size() - 1; i++)
		m_InputSemanticsDX11.insert(std::make_pair(i, InputSemanticDX11(dx11InputElements[i].SemanticName, dx11InputElements[i].SemanticIndex, dx11InputElements[i].Format)));

	_ASSERT(dx11InputElements.size() > 0);
	CHECK_HR_MSG(m_RenderDeviceDX11.GetDeviceD3D11()->CreateInputLayout(dx11InputElements.data(), (UINT)dx11InputElements.size() - 1, pShaderBlob->GetBufferPointer(), pShaderBlob->GetBufferSize(), &m_pInputLayout), L"Failed to create input layout.");

	return true;
}

//
// ShaderInputLayoutDX11
//
ID3D11InputLayout* ShaderInputLayoutDX11::GetInputLayout() const
{
	return m_pInputLayout;
}



//
// HELPERS
//
DXGI_FORMAT GetDXGIFormat(const D3D11_SIGNATURE_PARAMETER_DESC& paramDesc)
{
	DXGI_FORMAT format = DXGI_FORMAT_UNKNOWN;
	if (paramDesc.Mask == 1) // 1 component
	{
		switch (paramDesc.ComponentType)
		{
		case D3D_REGISTER_COMPONENT_UINT32:
		{
			format = DXGI_FORMAT_R32_UINT;
		}
		break;
		case D3D_REGISTER_COMPONENT_SINT32:
		{
			format = DXGI_FORMAT_R32_SINT;
		}
		break;
		case D3D_REGISTER_COMPONENT_FLOAT32:
		{
			format = DXGI_FORMAT_R32_FLOAT;
		}
		break;
		}
	}
	else if (paramDesc.Mask <= 3) // 2 components
	{
		switch (paramDesc.ComponentType)
		{
		case D3D_REGISTER_COMPONENT_UINT32:
		{
			format = DXGI_FORMAT_R32G32_UINT;
		}
		break;
		case D3D_REGISTER_COMPONENT_SINT32:
		{
			format = DXGI_FORMAT_R32G32_SINT;
		}
		break;
		case D3D_REGISTER_COMPONENT_FLOAT32:
		{
			format = DXGI_FORMAT_R32G32_FLOAT;
		}
		break;
		}
	}
	else if (paramDesc.Mask <= 7) // 3 components
	{
		switch (paramDesc.ComponentType)
		{
		case D3D_REGISTER_COMPONENT_UINT32:
		{
			format = DXGI_FORMAT_R32G32B32_UINT;
		}
		break;
		case D3D_REGISTER_COMPONENT_SINT32:
		{
			format = DXGI_FORMAT_R32G32B32_SINT;
		}
		break;
		case D3D_REGISTER_COMPONENT_FLOAT32:
		{
			format = DXGI_FORMAT_R32G32B32_FLOAT;
		}
		break;
		}
	}
	else if (paramDesc.Mask <= 15) // 4 components
	{
		switch (paramDesc.ComponentType)
		{
		case D3D_REGISTER_COMPONENT_UINT32:
		{
			format = DXGI_FORMAT_R32G32B32A32_UINT;
		}
		break;
		case D3D_REGISTER_COMPONENT_SINT32:
		{
			format = DXGI_FORMAT_R32G32B32A32_SINT;
		}
		break;
		case D3D_REGISTER_COMPONENT_FLOAT32:
		{
			format = DXGI_FORMAT_R32G32B32A32_FLOAT;
		}
		break;
		}
	}

	return format;
}
