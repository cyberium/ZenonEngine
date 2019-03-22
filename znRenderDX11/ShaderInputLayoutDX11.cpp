#include "stdafx.h"

// General
#include "ShaderInputLayoutDX11.h"

// Additional
#include "D3D9_To_D3D11.h"

static InputSemantic gs_InvalidShaderSemantic;
// FORWARD BEGIN
DXGI_FORMAT GetDXGIFormat(const D3D11_SIGNATURE_PARAMETER_DESC& paramDesc);
// FORWARD END

ShaderInputLayoutDX11::ShaderInputLayoutDX11(ID3D11Device2 * pDevice)
	: m_pDevice(pDevice)
{
	m_pDevice->GetImmediateContext2(&m_pDeviceContext);
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
	for (auto& it : m_InputSemantics)
	{
		if (it.first.Name == binding.Name && it.first.Index == binding.Index)
		{
			return true;
		}
	}

	return false;
}

const InputSemantic& ShaderInputLayoutDX11::GetSemantic(const BufferBinding& binding) const
{
	for (auto& it : m_InputSemantics)
	{
		if (it.first.Name == binding.Name && it.first.Index == binding.Index)
		{
			return it.first;
		}
	}

	assert1(false);
	return gs_InvalidShaderSemantic;
}

UINT ShaderInputLayoutDX11::GetSemanticSlot(const BufferBinding& binding) const
{
	for (auto& it : m_InputSemantics)
	{
		if (it.first.Name == binding.Name && it.first.Index == binding.Index)
		{
			return it.second;
		}
	}

	assert1(false);
	return UINT_MAX;
}



bool ShaderInputLayoutDX11::LoadFromReflector(ID3DBlob * pShaderBlob, ID3D11ShaderReflection * pReflector)
{
	HRESULT hr = S_OK;

	// Query input parameters and build the input layout
	D3D11_SHADER_DESC shaderDescription;
	if (FAILED(hr = pReflector->GetDesc(&shaderDescription)))
	{
		Log::Error("Failed to get shader description from shader reflector.");
		return false;
	}

	std::vector<D3D11_INPUT_ELEMENT_DESC> inputElements;
	for (UINT i = 0; i < shaderDescription.InputParameters; ++i)
	{
		D3D11_SIGNATURE_PARAMETER_DESC parameterSignature;
		pReflector->GetInputParameterDesc(i, &parameterSignature);

		D3D11_INPUT_ELEMENT_DESC inputElement;
		inputElement.SemanticName = parameterSignature.SemanticName;
		inputElement.SemanticIndex = parameterSignature.SemanticIndex;
		inputElement.InputSlot = i; // TODO: If using interleaved arrays, then the input slot should be 0.  If using packed arrays, the input slot will vary.
		inputElement.AlignedByteOffset = D3D11_APPEND_ALIGNED_ELEMENT;
		inputElement.InputSlotClass = D3D11_INPUT_PER_VERTEX_DATA; // TODO: Figure out how to deal with per-instance data? .. Don't. Just use structured buffers to store per-instance data and use the SV_InstanceID as an index in the structured buffer.
		inputElement.InstanceDataStepRate = 0;
		inputElement.Format = GetDXGIFormat(parameterSignature);
		inputElements.push_back(inputElement);
		
		assert(inputElement.Format != DXGI_FORMAT_UNKNOWN);

		m_InputSemantics.insert(SemanticMap::value_type(InputSemantic(inputElement.SemanticName, inputElement.SemanticIndex), i));
	}

	if (inputElements.size() > 0)
	{
		if (FAILED(hr = m_pDevice->CreateInputLayout(inputElements.data(), (UINT)inputElements.size(), pShaderBlob->GetBufferPointer(), pShaderBlob->GetBufferSize(), &m_pInputLayout)))
		{
			Log::Error("Failed to create input layout.");
			return false;
		}
	}

	return true;
}

bool ShaderInputLayoutDX11::LoadFromD3D9(ID3DBlob * pShaderBlob, const std::vector<D3DVERTEXELEMENT9>& declIn)
{
	std::vector<D3D11_INPUT_ELEMENT_DESC> inputElements;
	ConvertVertexDeclaration(declIn, inputElements);

	for (uint32 i = 0; i < inputElements.size() - 1; i++)
	{
		m_InputSemantics.insert(SemanticMap::value_type(InputSemantic(inputElements[i].SemanticName, inputElements[i].SemanticIndex), i));
	}

	if (inputElements.size() > 0)
	{
		HRESULT hr = S_OK;
		if (FAILED(hr = m_pDevice->CreateInputLayout(inputElements.data(), (UINT)inputElements.size() - 1, pShaderBlob->GetBufferPointer(), pShaderBlob->GetBufferSize(), &m_pInputLayout)))
		{
			Log::Error("Failed to create input layout.");
			return false;
		}
	}

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
DXGI_FORMAT GetDXGIFormat(const D3D11_SIGNATURE_PARAMETER_DESC& paramDesc) // Determine DXGI format
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
