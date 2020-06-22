#include "stdafx.h"

// General
#include "ShaderDX11.h"

// Additional
#include "ShaderDX11Include.h"

// FORWARD BEGIN
std::string GetLatestProfile(EShaderType type, const D3D_FEATURE_LEVEL& _featureLevel);
// FORWARD END

ShaderDX11::ShaderDX11(IRenderDeviceDX11& RenderDeviceDX11)
	: m_RenderDeviceDX11(RenderDeviceDX11)
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
		

		std::string _profile = profile;
		if (profile == "latest")
		{
			D3D_FEATURE_LEVEL featureLevel = m_RenderDeviceDX11.GetDeviceD3D11()->GetFeatureLevel();
			_profile = GetLatestProfile(shaderType, featureLevel);
			if (_profile.empty())
			{
				throw CznRenderException("Invalid shader type for feature level.");
			}
		}

        std::shared_ptr<IFile> file = m_RenderDeviceDX11.GetBaseManager().GetManager<IFilesManager>()->Open(fileName);
        std::string data = RecursionInclude(m_RenderDeviceDX11.GetBaseManager(), file);

		UINT flags = D3DCOMPILE_ENABLE_STRICTNESS;
#if defined( _DEBUG )
		flags |= D3DCOMPILE_DEBUG;
#else
		flags |= D3DCOMPILE_OPTIMIZATION_LEVEL3;
#endif

		std::vector<D3D_SHADER_MACRO> macros;
		for (const auto& macro : shaderMacros)
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

		ATL::CComPtr<ID3DBlob> pErrorBlob;

		CShaderDX11Include shaderInclude(m_RenderDeviceDX11.GetBaseManager());

			hr = D3DCompile(data.c_str(), data.size(), fileName.c_str(), macros.data(), &shaderInclude, entryPoint.c_str(), _profile.c_str(), flags, 0, &pShaderBlob, &pErrorBlob);
			if (FAILED(hr))
			{
				if (pErrorBlob != nullptr)
				{
					throw CznRenderException(std::string(static_cast<char*>(pErrorBlob->GetBufferPointer()), pErrorBlob->GetBufferSize()));
				}
				return false;
			}

			// We're done compiling.. Delete the macro definitions.
			for (D3D_SHADER_MACRO macro : macros)
			{
				delete[] macro.Name;
				delete[] macro.Definition;
			}
			macros.clear();

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
		hr = m_RenderDeviceDX11.GetDeviceD3D11()->CreateVertexShader(m_pShaderBlob->GetBufferPointer(), m_pShaderBlob->GetBufferSize(), nullptr, &m_pVertexShader);
		break;
	case EShaderType::TessellationControlShader:
		hr = m_RenderDeviceDX11.GetDeviceD3D11()->CreateHullShader(m_pShaderBlob->GetBufferPointer(), m_pShaderBlob->GetBufferSize(), nullptr, &m_pHullShader);
		break;
	case EShaderType::TessellationEvaluationShader:
		hr = m_RenderDeviceDX11.GetDeviceD3D11()->CreateDomainShader(m_pShaderBlob->GetBufferPointer(), m_pShaderBlob->GetBufferSize(), nullptr, &m_pDomainShader);
		break;
	case EShaderType::GeometryShader:
		hr = m_RenderDeviceDX11.GetDeviceD3D11()->CreateGeometryShader(m_pShaderBlob->GetBufferPointer(), m_pShaderBlob->GetBufferSize(), nullptr, &m_pGeometryShader);
		break;
	case EShaderType::PixelShader:
		hr = m_RenderDeviceDX11.GetDeviceD3D11()->CreatePixelShader(m_pShaderBlob->GetBufferPointer(), m_pShaderBlob->GetBufferSize(), nullptr, &m_pPixelShader);
		break;
	case EShaderType::ComputeShader:
		hr = m_RenderDeviceDX11.GetDeviceD3D11()->CreateComputeShader(m_pShaderBlob->GetBufferPointer(), m_pShaderBlob->GetBufferSize(), nullptr, &m_pComputeShader);
		break;
	default:
		throw CznRenderException("Invalid shader type.");
	}

	if (FAILED(hr))
	{
		throw CznRenderException("Failed to create shader.");
	}

	// Reflect the parameters from the shader. Inspired by: http://members.gamedev.net/JasonZ/Heiroglyph/D3D11ShaderReflection.pdf
	ATL::CComPtr<ID3D11ShaderReflection> shaderReflector;
	CHECK_HR_MSG(D3DReflect(m_pShaderBlob->GetBufferPointer(), m_pShaderBlob->GetBufferSize(), IID_ID3D11ShaderReflection, (void**)&shaderReflector), L"Failed to reflect shader.");

	// Query input parameters and build the input layout
	D3D11_SHADER_DESC shaderDescription = { };
	CHECK_HR_MSG(shaderReflector->GetDesc(&shaderDescription), L"Failed to get shader description from shader reflector.");

	// LAYOUT HERE

	// Query Resources that are bound to the shader.
	for (UINT i = 0; i < shaderDescription.BoundResources; ++i)
	{
		D3D11_SHADER_INPUT_BIND_DESC bindDesc = { };
		CHECK_HR(shaderReflector->GetResourceBindingDesc(i, &bindDesc));
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
	std::shared_ptr<IFile> file = m_RenderDeviceDX11.GetBaseManager().GetManager<IFilesManager>()->Open(fileName);

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
	if (m_InputLayout != nullptr)
	{
		Log::Warn("ShaderDX11: Imput layout already loaded.");
		return true;
	}

	// Reflect the parameters from the shader. Inspired by: http://members.gamedev.net/JasonZ/Heiroglyph/D3D11ShaderReflection.pdf
	ATL::CComPtr<ID3D11ShaderReflection> shaderReflector;
	CHECK_HR_MSG(D3DReflect(m_pShaderBlob->GetBufferPointer(), m_pShaderBlob->GetBufferSize(), IID_ID3D11ShaderReflection, (void**)&shaderReflector), L"Failed to reflect shader.");

	std::shared_ptr<ShaderInputLayoutDX11> inputLayout = std::make_shared<ShaderInputLayoutDX11>(m_RenderDeviceDX11);
    inputLayout->LoadFromReflector(m_pShaderBlob, shaderReflector);
    m_InputLayout = inputLayout;

	return true;
}

bool ShaderDX11::LoadInputLayoutFromCustomElements(const std::vector<SCustomVertexElement>& declIn)
{
	if (m_InputLayout != nullptr)
	{
		Log::Warn("ShaderDX11: Imput layout already loaded.");
		return true;
	}

    std::shared_ptr<ShaderInputLayoutDX11> inputLayout = std::make_shared<ShaderInputLayoutDX11>(m_RenderDeviceDX11);
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
		m_RenderDeviceDX11.GetDeviceContextD3D11()->IASetInputLayout(std::dynamic_pointer_cast<ShaderInputLayoutDX11>(m_InputLayout)->GetInputLayout());
		m_RenderDeviceDX11.GetDeviceContextD3D11()->VSSetShader(m_pVertexShader, nullptr, 0);
	}
	else if (m_pHullShader)
	{
		m_RenderDeviceDX11.GetDeviceContextD3D11()->HSSetShader(m_pHullShader, nullptr, 0);
	}
	else if (m_pDomainShader)
	{
		m_RenderDeviceDX11.GetDeviceContextD3D11()->DSSetShader(m_pDomainShader, nullptr, 0);
	}
	else if (m_pGeometryShader)
	{
		m_RenderDeviceDX11.GetDeviceContextD3D11()->GSSetShader(m_pGeometryShader, nullptr, 0);
	}
	else if (m_pPixelShader)
	{
		m_RenderDeviceDX11.GetDeviceContextD3D11()->PSSetShader(m_pPixelShader, nullptr, 0);
	}
	else if (m_pComputeShader)
	{
		m_RenderDeviceDX11.GetDeviceContextD3D11()->CSSetShader(m_pComputeShader, nullptr, 0);
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
		m_RenderDeviceDX11.GetDeviceContextD3D11()->IASetInputLayout(nullptr);
		m_RenderDeviceDX11.GetDeviceContextD3D11()->VSSetShader(nullptr, nullptr, 0);
	}
	else if (m_pHullShader)
	{
		m_RenderDeviceDX11.GetDeviceContextD3D11()->HSSetShader(nullptr, nullptr, 0);
	}
	else if (m_pDomainShader)
	{
		m_RenderDeviceDX11.GetDeviceContextD3D11()->DSSetShader(nullptr, nullptr, 0);
	}
	else if (m_pGeometryShader)
	{
		m_RenderDeviceDX11.GetDeviceContextD3D11()->GSSetShader(nullptr, nullptr, 0);
	}
	else if (m_pPixelShader)
	{
		m_RenderDeviceDX11.GetDeviceContextD3D11()->PSSetShader(nullptr, nullptr, 0);
	}
	else if (m_pComputeShader)
	{
		m_RenderDeviceDX11.GetDeviceContextD3D11()->CSSetShader(nullptr, nullptr, 0);
	}
}

void ShaderDX11::Dispatch(const glm::uvec3& numGroups)
{
	if (m_RenderDeviceDX11.GetDeviceContextD3D11() && m_pComputeShader)
	{
		m_RenderDeviceDX11.GetDeviceContextD3D11()->Dispatch(numGroups.x, numGroups.y, numGroups.z);
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
				case D3D_FEATURE_LEVEL_10_1:
					return "vs_4_1";
				case D3D_FEATURE_LEVEL_10_0:
					return "vs_4_0";
				case D3D_FEATURE_LEVEL_9_3:
					return "vs_4_0_level_9_3";
				case D3D_FEATURE_LEVEL_9_2:
				case D3D_FEATURE_LEVEL_9_1:
					return "vs_4_0_level_9_1";
			}
		break;
		case EShaderType::TessellationControlShader:
			switch (_featureLevel)
			{
				case D3D_FEATURE_LEVEL_11_1:
				case D3D_FEATURE_LEVEL_11_0:
					return "ds_5_0";
			}
		break;
		case EShaderType::TessellationEvaluationShader:
			switch (_featureLevel)
			{
				case D3D_FEATURE_LEVEL_11_1:
				case D3D_FEATURE_LEVEL_11_0:
					return "hs_5_0";
			}
		break;
		case EShaderType::GeometryShader:
			switch (_featureLevel)
			{
				case D3D_FEATURE_LEVEL_11_1:
				case D3D_FEATURE_LEVEL_11_0:
					return "gs_5_0";
				case D3D_FEATURE_LEVEL_10_1:
					return "gs_4_1";
				case D3D_FEATURE_LEVEL_10_0:
					return "gs_4_0";
			}
		break;
		case EShaderType::PixelShader:
			switch (_featureLevel)
			{
				case D3D_FEATURE_LEVEL_11_1:
				case D3D_FEATURE_LEVEL_11_0:
					return "ps_5_0";
				case D3D_FEATURE_LEVEL_10_1:
					return "ps_4_1";
				case D3D_FEATURE_LEVEL_10_0:
					return "ps_4_0";
				case D3D_FEATURE_LEVEL_9_3:
					return "ps_4_0_level_9_3";
				case D3D_FEATURE_LEVEL_9_2:
				case D3D_FEATURE_LEVEL_9_1:
					return "ps_4_0_level_9_1";
			}
		break;
		case EShaderType::ComputeShader:
			switch (_featureLevel)
			{
				case D3D_FEATURE_LEVEL_11_1:
				case D3D_FEATURE_LEVEL_11_0:
					return "cs_5_0";
				case D3D_FEATURE_LEVEL_10_1:
					return "cs_4_1";
				case D3D_FEATURE_LEVEL_10_0:
					return "cs_4_0";
			}
		break;
	}

	return "";
}

