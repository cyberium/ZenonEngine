#include "stdafx.h"

// General
#include "ShaderDX11.h"

// FORWARD BEGIN
std::string GetLatestProfile(EShaderType type, const D3D_FEATURE_LEVEL& _featureLevel);
// FORWARD END

ShaderDX11::ShaderDX11(IRenderDeviceDX11* RenderDeviceD3D11)
	: m_RenderDeviceD3D11(RenderDeviceD3D11)
{}

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

bool ShaderDX11::LoadShaderFromString(EShaderType shaderType, const std::string& fileName, const std::string& source, const ShaderMacros& shaderMacros, const std::string& entryPoint, const std::string& profile, IShaderInputLayout* _customLayout)
{
	HRESULT hr;
	{
		ATL::CComPtr<ID3DBlob> pShaderBlob;
		ATL::CComPtr<ID3DBlob> pErrorBlob;

		std::string _profile = profile;
		if (profile == "latest")
		{
			D3D_FEATURE_LEVEL featureLevel = m_RenderDeviceD3D11->GetDeviceD3D11()->GetFeatureLevel();
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
#else
		flags |= D3DCOMPILE_OPTIMIZATION_LEVEL3;
#endif

        std::shared_ptr<IFile> file = m_RenderDeviceD3D11->GetRenderDevice()->GetBaseManager()->GetManager<IFilesManager>()->Open(fileName);
        std::string data = RecursionInclude(m_RenderDeviceD3D11->GetRenderDevice()->GetBaseManager(), file);

		hr = D3DCompile(data.c_str(), data.size(), fileName.c_str(), macros.data(), D3D_COMPILE_STANDARD_FILE_INCLUDE, entryPoint.c_str(), _profile.c_str(), flags, 0, &pShaderBlob, &pErrorBlob);

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
	case EShaderType::VertexShader:
		hr = m_RenderDeviceD3D11->GetDeviceD3D11()->CreateVertexShader(m_pShaderBlob->GetBufferPointer(), m_pShaderBlob->GetBufferSize(), nullptr, &m_pVertexShader);
		break;
	case EShaderType::TessellationControlShader:
		hr = m_RenderDeviceD3D11->GetDeviceD3D11()->CreateHullShader(m_pShaderBlob->GetBufferPointer(), m_pShaderBlob->GetBufferSize(), nullptr, &m_pHullShader);
		break;
	case EShaderType::TessellationEvaluationShader:
		hr = m_RenderDeviceD3D11->GetDeviceD3D11()->CreateDomainShader(m_pShaderBlob->GetBufferPointer(), m_pShaderBlob->GetBufferSize(), nullptr, &m_pDomainShader);
		break;
	case EShaderType::GeometryShader:
		hr = m_RenderDeviceD3D11->GetDeviceD3D11()->CreateGeometryShader(m_pShaderBlob->GetBufferPointer(), m_pShaderBlob->GetBufferSize(), nullptr, &m_pGeometryShader);
		break;
	case EShaderType::PixelShader:
		hr = m_RenderDeviceD3D11->GetDeviceD3D11()->CreatePixelShader(m_pShaderBlob->GetBufferPointer(), m_pShaderBlob->GetBufferSize(), nullptr, &m_pPixelShader);
		break;
	case EShaderType::ComputeShader:
		hr = m_RenderDeviceD3D11->GetDeviceD3D11()->CreateComputeShader(m_pShaderBlob->GetBufferPointer(), m_pShaderBlob->GetBufferSize(), nullptr, &m_pComputeShader);
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

		IShaderParameter::Type parameterType = IShaderParameter::Type::Invalid;

		switch (bindDesc.Type)
		{
		case D3D_SIT_TEXTURE:
			parameterType = IShaderParameter::Type::Texture;
			break;
		case D3D_SIT_SAMPLER:
			parameterType = IShaderParameter::Type::Sampler;
			break;
		case D3D_SIT_CBUFFER:
		case D3D_SIT_STRUCTURED:
			parameterType = IShaderParameter::Type::Buffer;
			break;
		case D3D_SIT_UAV_RWSTRUCTURED:
			parameterType = IShaderParameter::Type::RWBuffer;
			break;
		case D3D_SIT_UAV_RWTYPED:
			parameterType = IShaderParameter::Type::RWTexture;
			break;
		}

		// Create an empty shader parameter that should be filled-in by the application.
		std::shared_ptr<IShaderParameter> shaderParameter = std::make_shared<ShaderParameterBase>(resourceName, bindDesc.BindPoint, this, parameterType);
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

bool ShaderDX11::LoadShaderFromFile(EShaderType shaderType, const std::string& fileName, const ShaderMacros& shaderMacros, const std::string& entryPoint, const std::string& profile, IShaderInputLayout* _customLayout)
{
	std::shared_ptr<IFile> file = m_RenderDeviceD3D11->GetRenderDevice()->GetBaseManager()->GetManager<IFilesManager>()->Open(fileName);

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

	std::shared_ptr<ShaderInputLayoutDX11> inputLayout = std::make_shared<ShaderInputLayoutDX11>(m_RenderDeviceD3D11);
    inputLayout->LoadFromReflector(m_pShaderBlob, pReflector);
    m_InputLayout = inputLayout;

	return true;
}

bool ShaderDX11::LoadInputLayoutFromCustomElements(const std::vector<SCustomVertexElement>& declIn)
{
	if (m_InputLayout)
		return true;

    std::shared_ptr<ShaderInputLayoutDX11> inputLayout = std::make_shared<ShaderInputLayoutDX11>(m_RenderDeviceD3D11);
    inputLayout->LoadFromCustomElements(m_pShaderBlob, declIn);
    m_InputLayout = inputLayout;

	return true;
}

void ShaderDX11::Bind() const
{
	if (m_pVertexShader)
	{
		_ASSERT(m_InputLayout);
		_ASSERT(std::dynamic_pointer_cast<ShaderInputLayoutDX11>(m_InputLayout)->GetInputLayout());
		m_RenderDeviceD3D11->GetDeviceContextD3D11()->IASetInputLayout(std::dynamic_pointer_cast<ShaderInputLayoutDX11>(m_InputLayout)->GetInputLayout());
		m_RenderDeviceD3D11->GetDeviceContextD3D11()->VSSetShader(m_pVertexShader, nullptr, 0);
	}
	else if (m_pHullShader)
	{
		m_RenderDeviceD3D11->GetDeviceContextD3D11()->HSSetShader(m_pHullShader, nullptr, 0);
	}
	else if (m_pDomainShader)
	{
		m_RenderDeviceD3D11->GetDeviceContextD3D11()->DSSetShader(m_pDomainShader, nullptr, 0);
	}
	else if (m_pGeometryShader)
	{
		m_RenderDeviceD3D11->GetDeviceContextD3D11()->GSSetShader(m_pGeometryShader, nullptr, 0);
	}
	else if (m_pPixelShader)
	{
		m_RenderDeviceD3D11->GetDeviceContextD3D11()->PSSetShader(m_pPixelShader, nullptr, 0);
	}
	else if (m_pComputeShader)
	{
		m_RenderDeviceD3D11->GetDeviceContextD3D11()->CSSetShader(m_pComputeShader, nullptr, 0);
	}

	for (const auto& value : m_ShaderParameters)
	{
		value.second->Bind();
	}
}

void ShaderDX11::UnBind() const
{
	for (const auto& value : m_ShaderParameters)
	{
		value.second->Unbind();
	}

	if (m_pVertexShader)
	{
		m_RenderDeviceD3D11->GetDeviceContextD3D11()->IASetInputLayout(nullptr);
		m_RenderDeviceD3D11->GetDeviceContextD3D11()->VSSetShader(nullptr, nullptr, 0);
	}
	else if (m_pHullShader)
	{
		m_RenderDeviceD3D11->GetDeviceContextD3D11()->HSSetShader(nullptr, nullptr, 0);
	}
	else if (m_pDomainShader)
	{
		m_RenderDeviceD3D11->GetDeviceContextD3D11()->DSSetShader(nullptr, nullptr, 0);
	}
	else if (m_pGeometryShader)
	{
		m_RenderDeviceD3D11->GetDeviceContextD3D11()->GSSetShader(nullptr, nullptr, 0);
	}
	else if (m_pPixelShader)
	{
		m_RenderDeviceD3D11->GetDeviceContextD3D11()->PSSetShader(nullptr, nullptr, 0);
	}
	else if (m_pComputeShader)
	{
		m_RenderDeviceD3D11->GetDeviceContextD3D11()->CSSetShader(nullptr, nullptr, 0);
	}
}

void ShaderDX11::Dispatch(const glm::uvec3& numGroups)
{
	if (m_RenderDeviceD3D11->GetDeviceContextD3D11() && m_pComputeShader)
	{
		m_RenderDeviceD3D11->GetDeviceContextD3D11()->Dispatch(numGroups.x, numGroups.y, numGroups.z);
	}
}

std::string GetLatestProfile(EShaderType type, const D3D_FEATURE_LEVEL& _featureLevel)
{
	switch (type)
	{
	case EShaderType::VertexShader:
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
	case EShaderType::TessellationControlShader:
		switch (_featureLevel)
		{
		case D3D_FEATURE_LEVEL_11_1:
		case D3D_FEATURE_LEVEL_11_0:
			return "ds_5_0";
			break;
		}
		break;
	case EShaderType::TessellationEvaluationShader:
		switch (_featureLevel)
		{
		case D3D_FEATURE_LEVEL_11_1:
		case D3D_FEATURE_LEVEL_11_0:
			return "hs_5_0";
			break;
		}
		break;
	case EShaderType::GeometryShader:
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
	case EShaderType::PixelShader:
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
	case EShaderType::ComputeShader:
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

