#include <stdafx.h>

// General
#include "ShaderDX11.h"



static ShaderParameter gs_InvalidShaderParameter;

// FORWARD BEGIN
std::string GetLatestProfile(Shader::ShaderType type, const D3D_FEATURE_LEVEL& _featureLevel);
// FORWARD END

ShaderDX11::ShaderDX11(ID3D11Device2* pDevice)
	: m_pDevice(pDevice)
{
	m_pDevice->GetImmediateContext2(&m_pDeviceContext);
}

ShaderDX11::~ShaderDX11()
{
	Destroy();
}

void ShaderDX11::Destroy()
{
	m_pPixelShader.Release();
	m_pDomainShader.Release();
	m_pHullShader.Release();
	m_pGeometryShader.Release();
	m_pVertexShader.Release();
	m_pComputeShader.Release();

	m_ShaderParameters.clear();

}

bool ShaderDX11::LoadShaderFromString(ShaderType shaderType, cstring fileName, cstring source, const ShaderMacros& shaderMacros, cstring entryPoint, cstring profile, std::shared_ptr<IShaderInputLayout> _customLayout)
{
	HRESULT hr;
	{
		ATL::CComPtr<ID3DBlob> pShaderBlob;
		ATL::CComPtr<ID3DBlob> pErrorBlob;

		std::string _profile = profile;
		if (profile == "latest")
		{
			D3D_FEATURE_LEVEL featureLevel = m_pDevice->GetFeatureLevel();
			_profile = GetLatestProfile(shaderType, featureLevel);
			if (_profile.empty())
			{
				Log::Error("Invalid shader type for feature level.");
				return false;
			}
		}

		std::vector<D3D_SHADER_MACRO> macros;
		for (auto macro : shaderMacros)
		{
			// The macro definitions passed to this function only store temporary std::string objects.
			// I need to copy the temporary strings into the D3D macro type 
			// in order for it to persist outside of this for loop.
			std::string name = macro.first;
			std::string definition = macro.second;

			char* c_name = new char[name.size() + 1];
			char* c_definition = new char[definition.size() + 1];

			strncpy_s(c_name, name.size() + 1, name.c_str(), name.size());
			strncpy_s(c_definition, definition.size() + 1, definition.c_str(), definition.size());

			macros.push_back({ c_name, c_definition });
		}
		macros.push_back({ 0, 0 });


		UINT flags = D3DCOMPILE_ENABLE_STRICTNESS;
#if defined( _DEBUG )
		flags |= D3DCOMPILE_DEBUG;
#endif

		hr = D3DCompile((LPCVOID)source.c_str(), source.size(), fileName.c_str(), macros.data(), D3D_COMPILE_STANDARD_FILE_INCLUDE, entryPoint.c_str(), _profile.c_str(), flags, 0, &pShaderBlob, &pErrorBlob);

		// We're done compiling.. Delete the macro definitions.
		for (D3D_SHADER_MACRO macro : macros)
		{
			delete[] macro.Name;
			delete[] macro.Definition;
		}
		macros.clear();

		if (FAILED(hr))
		{
			if (pErrorBlob)
			{
				OutputDebugStringA(static_cast<char*>(pErrorBlob->GetBufferPointer()));
				Log::Error(static_cast<char*>(pErrorBlob->GetBufferPointer()));
			}
			return false;
		}

		m_pShaderBlob = pShaderBlob;
	}

	// After the shader recompiles, try to restore the shader parameters.
	ParameterMap shaderParameters = m_ShaderParameters;

	// Destroy the last shader as we are now loading a new one.
	Destroy();

	m_ShaderType = shaderType;

	switch (m_ShaderType)
	{
	case VertexShader:
		hr = m_pDevice->CreateVertexShader(m_pShaderBlob->GetBufferPointer(), m_pShaderBlob->GetBufferSize(), nullptr, &m_pVertexShader);
		break;
	case TessellationControlShader:
		hr = m_pDevice->CreateHullShader(m_pShaderBlob->GetBufferPointer(), m_pShaderBlob->GetBufferSize(), nullptr, &m_pHullShader);
		break;
	case TessellationEvaluationShader:
		hr = m_pDevice->CreateDomainShader(m_pShaderBlob->GetBufferPointer(), m_pShaderBlob->GetBufferSize(), nullptr, &m_pDomainShader);
		break;
	case GeometryShader:
		hr = m_pDevice->CreateGeometryShader(m_pShaderBlob->GetBufferPointer(), m_pShaderBlob->GetBufferSize(), nullptr, &m_pGeometryShader);
		break;
	case PixelShader:
		hr = m_pDevice->CreatePixelShader(m_pShaderBlob->GetBufferPointer(), m_pShaderBlob->GetBufferSize(), nullptr, &m_pPixelShader);
		break;
	case ComputeShader:
		hr = m_pDevice->CreateComputeShader(m_pShaderBlob->GetBufferPointer(), m_pShaderBlob->GetBufferSize(), nullptr, &m_pComputeShader);
		break;
	default:
		Log::Error("Invalid shader type.");
		break;
	}

	if (FAILED(hr))
	{
		Log::Error("Failed to create shader.");
		return false;
	}

	// Reflect the parameters from the shader.
	// Inspired by: http://members.gamedev.net/JasonZ/Heiroglyph/D3D11ShaderReflection.pdf
	ATL::CComPtr<ID3D11ShaderReflection> pReflector;
	if (FAILED(hr = D3DReflect(m_pShaderBlob->GetBufferPointer(), m_pShaderBlob->GetBufferSize(), IID_ID3D11ShaderReflection, (void**)&pReflector)))
	{
		Log::Error("Failed to reflect shader.");
		return false;
	}

	// Query input parameters and build the input layout
	D3D11_SHADER_DESC shaderDescription;
	if (FAILED(hr = pReflector->GetDesc(&shaderDescription)))
	{
		Log::Error("Failed to get shader description from shader reflector.");
		return false;
	}

	// LAYOUT HERE

	// Query Resources that are bound to the shader.
	for (UINT i = 0; i < shaderDescription.BoundResources; ++i)
	{
		D3D11_SHADER_INPUT_BIND_DESC bindDesc;
		pReflector->GetResourceBindingDesc(i, &bindDesc);
		std::string resourceName = bindDesc.Name;

		ShaderParameter::Type parameterType = ShaderParameter::Type::Invalid;

		switch (bindDesc.Type)
		{
		case D3D_SIT_TEXTURE:
			parameterType = ShaderParameter::Type::Texture;
			break;
		case D3D_SIT_SAMPLER:
			parameterType = ShaderParameter::Type::Sampler;
			break;
		case D3D_SIT_CBUFFER:
		case D3D_SIT_STRUCTURED:
			parameterType = ShaderParameter::Type::Buffer;
			break;
		case D3D_SIT_UAV_RWSTRUCTURED:
			parameterType = ShaderParameter::Type::RWBuffer;
			break;
		case D3D_SIT_UAV_RWTYPED:
			parameterType = ShaderParameter::Type::RWTexture;
			break;
		}

		// Create an empty shader parameter that should be filled-in by the application.
		std::shared_ptr<ShaderParameter> shaderParameter = std::make_shared<ShaderParameter>(resourceName, bindDesc.BindPoint, shared_from_this(), parameterType);
		m_ShaderParameters.insert(ParameterMap::value_type(resourceName, shaderParameter));
	}

	// Now try to restore the original shader parameters (if there were any)
	for (auto shaderParameter : shaderParameters)
	{
		ParameterMap::iterator iter = m_ShaderParameters.find(shaderParameter.first);
		if (iter != m_ShaderParameters.end())
		{
			iter->second = shaderParameter.second;
		}
	}

	return true;
}

bool ShaderDX11::LoadShaderFromFile(ShaderType shaderType, cstring fileName, const ShaderMacros& shaderMacros, cstring entryPoint, cstring profile, std::shared_ptr<IShaderInputLayout> _customLayout)
{
	std::shared_ptr<IFile> file = GetManager<IFilesManager>()->Open(fileName);

	std::string data = "";
	while (!file->isEof())
	{
		std::string line;
		file->readLine(&line);

		data += line + '\n';
	}

	return LoadShaderFromString(shaderType, fileName, data, shaderMacros, entryPoint, profile, _customLayout);
}

bool ShaderDX11::LoadInputLayoutFromReflector()
{
	if (m_InputLayout)
		return true;

	HRESULT hr = S_OK;

	// Reflect the parameters from the shader.
	// Inspired by: http://members.gamedev.net/JasonZ/Heiroglyph/D3D11ShaderReflection.pdf
	ATL::CComPtr<ID3D11ShaderReflection> pReflector;
	if (FAILED(hr = D3DReflect(m_pShaderBlob->GetBufferPointer(), m_pShaderBlob->GetBufferSize(), IID_ID3D11ShaderReflection, (void**)&pReflector)))
	{
		Log::Error("Failed to reflect shader.");
		return false;
	}

	m_InputLayout = std::make_shared<ShaderInputLayoutDX11>(m_pDevice);
	m_InputLayout->LoadFromReflector(m_pShaderBlob, pReflector);

	return true;
}

bool ShaderDX11::LoadInputLayoutFromD3DElement(const std::vector<D3DVERTEXELEMENT9>& declIn)
{
	if (m_InputLayout)
		return true;

	m_InputLayout = std::make_shared<ShaderInputLayoutDX11>(m_pDevice);
	m_InputLayout->LoadFromD3D9(m_pShaderBlob, declIn);

	return true;
}

std::shared_ptr<IShaderInputLayout> ShaderDX11::GetInputLayout() const
{
	return m_InputLayout;
}

ShaderParameter& ShaderDX11::GetShaderParameterByName(cstring name) const
{
	ParameterMap::const_iterator iter = m_ShaderParameters.find(name);
	if (iter != m_ShaderParameters.end())
	{
		return *(iter->second);
	}

	//assert1(false);
	return gs_InvalidShaderParameter;
}

void ShaderDX11::Bind()
{
	for (ParameterMap::value_type value : m_ShaderParameters)
	{
		value.second->Bind();
	}

	if (m_pVertexShader)
	{
		assert1(m_InputLayout);
		assert1(m_InputLayout->GetInputLayout());
		m_pDeviceContext->IASetInputLayout(m_InputLayout->GetInputLayout());
		m_pDeviceContext->VSSetShader(m_pVertexShader, nullptr, 0);
	}
	else if (m_pHullShader)
	{
		m_pDeviceContext->HSSetShader(m_pHullShader, nullptr, 0);
	}
	else if (m_pDomainShader)
	{
		m_pDeviceContext->DSSetShader(m_pDomainShader, nullptr, 0);
	}
	else if (m_pGeometryShader)
	{
		m_pDeviceContext->GSSetShader(m_pGeometryShader, nullptr, 0);
	}
	else if (m_pPixelShader)
	{
		m_pDeviceContext->PSSetShader(m_pPixelShader, nullptr, 0);
	}
	else if (m_pComputeShader)
	{
		m_pDeviceContext->CSSetShader(m_pComputeShader, nullptr, 0);
	}
}

void ShaderDX11::UnBind()
{
	for (ParameterMap::value_type value : m_ShaderParameters)
	{
		value.second->UnBind();
	}

	if (m_pVertexShader)
	{
		m_pDeviceContext->IASetInputLayout(nullptr);
		m_pDeviceContext->VSSetShader(nullptr, nullptr, 0);
	}
	else if (m_pHullShader)
	{
		m_pDeviceContext->HSSetShader(nullptr, nullptr, 0);
	}
	else if (m_pDomainShader)
	{
		m_pDeviceContext->DSSetShader(nullptr, nullptr, 0);
	}
	else if (m_pGeometryShader)
	{
		m_pDeviceContext->GSSetShader(nullptr, nullptr, 0);
	}
	else if (m_pPixelShader)
	{
		m_pDeviceContext->PSSetShader(nullptr, nullptr, 0);
	}
	else if (m_pComputeShader)
	{
		m_pDeviceContext->CSSetShader(nullptr, nullptr, 0);
	}
}

void ShaderDX11::Dispatch(const glm::uvec3& numGroups)
{
	if (m_pDeviceContext && m_pComputeShader)
	{
		m_pDeviceContext->Dispatch(numGroups.x, numGroups.y, numGroups.z);
	}
}

std::string GetLatestProfile(Shader::ShaderType type, const D3D_FEATURE_LEVEL& _featureLevel)
{
	switch (type)
	{
	case Shader::VertexShader:
		switch (_featureLevel)
		{
		case D3D_FEATURE_LEVEL_11_1:
		case D3D_FEATURE_LEVEL_11_0:
			return "vs_5_0";
			break;
		case D3D_FEATURE_LEVEL_10_1:
			return "vs_4_1";
			break;
		case D3D_FEATURE_LEVEL_10_0:
			return "vs_4_0";
			break;
		case D3D_FEATURE_LEVEL_9_3:
			return "vs_4_0_level_9_3";
			break;
		case D3D_FEATURE_LEVEL_9_2:
		case D3D_FEATURE_LEVEL_9_1:
			return "vs_4_0_level_9_1";
			break;
		}
		break;
	case Shader::TessellationControlShader:
		switch (_featureLevel)
		{
		case D3D_FEATURE_LEVEL_11_1:
		case D3D_FEATURE_LEVEL_11_0:
			return "ds_5_0";
			break;
		}
		break;
	case Shader::TessellationEvaluationShader:
		switch (_featureLevel)
		{
		case D3D_FEATURE_LEVEL_11_1:
		case D3D_FEATURE_LEVEL_11_0:
			return "hs_5_0";
			break;
		}
		break;
	case Shader::GeometryShader:
		switch (_featureLevel)
		{
		case D3D_FEATURE_LEVEL_11_1:
		case D3D_FEATURE_LEVEL_11_0:
			return "gs_5_0";
			break;
		case D3D_FEATURE_LEVEL_10_1:
			return "gs_4_1";
			break;
		case D3D_FEATURE_LEVEL_10_0:
			return "gs_4_0";
			break;
		}
		break;
	case Shader::PixelShader:
		switch (_featureLevel)
		{
		case D3D_FEATURE_LEVEL_11_1:
		case D3D_FEATURE_LEVEL_11_0:
			return "ps_5_0";
			break;
		case D3D_FEATURE_LEVEL_10_1:
			return "ps_4_1";
			break;
		case D3D_FEATURE_LEVEL_10_0:
			return "ps_4_0";
			break;
		case D3D_FEATURE_LEVEL_9_3:
			return "ps_4_0_level_9_3";
			break;
		case D3D_FEATURE_LEVEL_9_2:
		case D3D_FEATURE_LEVEL_9_1:
			return "ps_4_0_level_9_1";
			break;
		}
		break;
	case Shader::ComputeShader:
		switch (_featureLevel)
		{
		case D3D_FEATURE_LEVEL_11_1:
		case D3D_FEATURE_LEVEL_11_0:
			return "cs_5_0";
			break;
		case D3D_FEATURE_LEVEL_10_1:
			return "cs_4_1";
			break;
		case D3D_FEATURE_LEVEL_10_0:
			return "cs_4_0";
			break;
		}
	}

	return "";
}

