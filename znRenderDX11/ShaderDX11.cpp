#include "stdafx.h"

// General
#include "ShaderDX11.h"

// Additional
#include "ShaderDX11Include.h"

namespace
{
	std::string FixShaderFileName(std::string FileName)
	{
		std::string fixedName = "";
		for (const auto& c : FileName)
		{
			if (std::isalnum(c, std::locale()))
				fixedName += c;
			else
				fixedName += '_';
		}
		return fixedName;
	}

	std::string ShaderTypeToString(EShaderType shaderType)
	{
		if (shaderType == EShaderType::VertexShader)
			return "vs";
		else if (shaderType == EShaderType::PixelShader)
			return "ps";
		else if (shaderType == EShaderType::GeometryShader)
			return "gs";
		return "unknown";
	}

	std::string GetLatestProfile(EShaderType type, const D3D_FEATURE_LEVEL& FeatureLevel)
	{
		switch (type)
		{
			case EShaderType::VertexShader:
			{
				switch (FeatureLevel)
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
			}
			break;
			case EShaderType::TessellationControlShader:
			{
				switch (FeatureLevel)
				{
					case D3D_FEATURE_LEVEL_11_1:
					case D3D_FEATURE_LEVEL_11_0:
						return "ds_5_0";
				}
			}
			break;
			case EShaderType::TessellationEvaluationShader:
			{
				switch (FeatureLevel)
				{
					case D3D_FEATURE_LEVEL_11_1:
					case D3D_FEATURE_LEVEL_11_0:
						return "hs_5_0";
				}
			}
			break;
			case EShaderType::GeometryShader:
			{
				switch (FeatureLevel)
				{
					case D3D_FEATURE_LEVEL_11_1:
					case D3D_FEATURE_LEVEL_11_0:
						return "gs_5_0";
					case D3D_FEATURE_LEVEL_10_1:
						return "gs_4_1";
					case D3D_FEATURE_LEVEL_10_0:
						return "gs_4_0";
				}
			}
			break;
			case EShaderType::PixelShader:
			{
				switch (FeatureLevel)
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
			}
			break;
			case EShaderType::ComputeShader:
			{
				switch (FeatureLevel)
				{
					case D3D_FEATURE_LEVEL_11_1:
					case D3D_FEATURE_LEVEL_11_0:
						return "cs_5_0";
					case D3D_FEATURE_LEVEL_10_1:
						return "cs_4_1";
					case D3D_FEATURE_LEVEL_10_0:
						return "cs_4_0";
				}
			}
			break;
		}

		throw CznRenderException("Feature level '%s' is unkwnon for shader '%s'.", FeatureLevel, ShaderTypeToString(type).c_str());
	}

	std::string CalculateUniqueName(EShaderType ShaderType, std::string FileName, IShader::ShaderMacros ShaderMacros, std::string EntryPoint)
	{
		std::string str = "";
		str += ShaderTypeToString(ShaderType);
		str += "_";
		str += FixShaderFileName(FileName);
		str += "_";

		std::string macrosHash = "";
		for (const auto& m : ShaderMacros)
		{
			macrosHash += m.MacrosName;
			macrosHash += m.ValueStr;
		}
		str += std::to_string(std::hash<std::string>{}(macrosHash));
		str += "_";
		str += EntryPoint;
		return "shaders_cache/" + str + ".shader";
	}
}




UINT flags = D3DCOMPILE_ENABLE_STRICTNESS
#if defined( _DEBUG )
| D3DCOMPILE_DEBUG
#else
| D3DCOMPILE_OPTIMIZATION_LEVEL3
#endif
;



ShaderDX11::ShaderDX11(IRenderDeviceDX11& RenderDeviceDX11)
	: m_RenderDeviceDX11(RenderDeviceDX11)
{}

ShaderDX11::~ShaderDX11()
{
	Destroy();
}



//
//
//
bool ShaderDX11::LoadFromFile(EShaderType Type, std::string Filename, ShaderMacros Macroses, std::string EntryPoint)
{
	if (m_Type != EShaderType::UnknownShaderType)
		throw CznRenderException("Shader already loaded.");

	Log::Info("Loading shader '%s':'%s'.", Filename.c_str(), EntryPoint.c_str());

	std::string compiledShaderFilename = CalculateUniqueName(Type, Filename, Macroses, EntryPoint);
	std::shared_ptr<IByteBuffer> compiledShaderBuffer;

	auto filesManager = m_RenderDeviceDX11.GetBaseManager().GetManager<IFilesManager>();
	if (false == filesManager->IsFileExists(compiledShaderFilename))
	{
		compiledShaderBuffer = CompileShader(Type, Filename, Macroses, EntryPoint);

		auto compiledShaderFile = filesManager->Create(compiledShaderFilename);
		compiledShaderFile->writeBytes(compiledShaderBuffer->getData(), compiledShaderBuffer->getSize());
		compiledShaderFile->Save();
	}
	else
	{
		compiledShaderBuffer = filesManager->Open(compiledShaderFilename);
	}

	m_Type = Type;
	m_FileName = Filename;
	m_Macros = Macroses;
	m_EntryPoint = EntryPoint;

	LoadCompiledShader(compiledShaderBuffer);

}

bool ShaderDX11::LoadInputLayoutFromReflector()
{
	if (m_InputLayout != nullptr)
		throw CException("Input layout already loaded.");

	std::string compiledShaderFilename = CalculateUniqueName(m_Type, m_FileName, m_Macros, m_EntryPoint);
	std::shared_ptr<IByteBuffer> compiledShaderBuffer = m_RenderDeviceDX11.GetBaseManager().GetManager<IFilesManager>()->Open(compiledShaderFilename);
	if (compiledShaderBuffer == nullptr)
		throw CznRenderException("Shader compiled file not found.");

	std::shared_ptr<ShaderInputLayoutDX11> inputLayout = MakeShared(ShaderInputLayoutDX11, m_RenderDeviceDX11);
    inputLayout->LoadFromReflector(compiledShaderBuffer);
    m_InputLayout = inputLayout;

	return true;
}

bool ShaderDX11::LoadInputLayoutFromCustomElements(const std::vector<SCustomInputElement>& CustomElements)
{
	if (m_InputLayout != nullptr)
		throw CException("ShaderDX11: Input layout already loaded.");

	std::string compiledShaderFilename = CalculateUniqueName(m_Type, m_FileName, m_Macros, m_EntryPoint);
	std::shared_ptr<IByteBuffer> compiledShaderBuffer = m_RenderDeviceDX11.GetBaseManager().GetManager<IFilesManager>()->Open(compiledShaderFilename);
	if (compiledShaderBuffer == nullptr)
		throw CznRenderException("Shader compiled file not found.");

    std::shared_ptr<ShaderInputLayoutDX11> inputLayout = MakeShared(ShaderInputLayoutDX11, m_RenderDeviceDX11);
    inputLayout->LoadFromCustomElements(compiledShaderBuffer, CustomElements);
    m_InputLayout = inputLayout;

	return true;
}

void ShaderDX11::Bind() const
{
	if (m_VertexShader)
	{
		_ASSERT(m_InputLayout);
		_ASSERT(std::dynamic_pointer_cast<ShaderInputLayoutDX11>(m_InputLayout)->GetInputLayout());
		m_RenderDeviceDX11.GetDeviceContextD3D11()->IASetInputLayout(std::dynamic_pointer_cast<ShaderInputLayoutDX11>(m_InputLayout)->GetInputLayout());
		m_RenderDeviceDX11.GetDeviceContextD3D11()->VSSetShader(m_VertexShader, nullptr, 0);
	}
	else if (m_HullShader)
	{
		m_RenderDeviceDX11.GetDeviceContextD3D11()->HSSetShader(m_HullShader, nullptr, 0);
	}
	else if (m_DomainShader)
	{
		m_RenderDeviceDX11.GetDeviceContextD3D11()->DSSetShader(m_DomainShader, nullptr, 0);
	}
	else if (m_GeometryShader)
	{
		m_RenderDeviceDX11.GetDeviceContextD3D11()->GSSetShader(m_GeometryShader, nullptr, 0);
	}
	else if (m_PixelShader)
	{
		m_RenderDeviceDX11.GetDeviceContextD3D11()->PSSetShader(m_PixelShader, nullptr, 0);
	}
	else if (m_ComputeShader)
	{
		m_RenderDeviceDX11.GetDeviceContextD3D11()->CSSetShader(m_ComputeShader, nullptr, 0);
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

	if (m_VertexShader)
	{
		m_RenderDeviceDX11.GetDeviceContextD3D11()->IASetInputLayout(nullptr);
		m_RenderDeviceDX11.GetDeviceContextD3D11()->VSSetShader(nullptr, nullptr, 0);
	}
	else if (m_HullShader)
	{
		m_RenderDeviceDX11.GetDeviceContextD3D11()->HSSetShader(nullptr, nullptr, 0);
	}
	else if (m_DomainShader)
	{
		m_RenderDeviceDX11.GetDeviceContextD3D11()->DSSetShader(nullptr, nullptr, 0);
	}
	else if (m_GeometryShader)
	{
		m_RenderDeviceDX11.GetDeviceContextD3D11()->GSSetShader(nullptr, nullptr, 0);
	}
	else if (m_PixelShader)
	{
		m_RenderDeviceDX11.GetDeviceContextD3D11()->PSSetShader(nullptr, nullptr, 0);
	}
	else if (m_ComputeShader)
	{
		m_RenderDeviceDX11.GetDeviceContextD3D11()->CSSetShader(nullptr, nullptr, 0);
	}
}

void ShaderDX11::Dispatch(const glm::uvec3& numGroups)
{
	if (m_RenderDeviceDX11.GetDeviceContextD3D11() && m_ComputeShader)
	{
		m_RenderDeviceDX11.GetDeviceContextD3D11()->Dispatch(numGroups.x, numGroups.y, numGroups.z);
	}
}



//
// Private
//
std::shared_ptr<IByteBuffer> ShaderDX11::CompileShader(EShaderType Type, std::string Filename, ShaderMacros Macroses, std::string EntryPoint)
{
	auto filesManager = m_RenderDeviceDX11.GetBaseManager().GetManager<IFilesManager>();

	std::string profile = GetLatestProfile(Type, m_RenderDeviceDX11.GetDeviceD3D11()->GetFeatureLevel());
	if (profile.empty())
		throw CznRenderException("Invalid shader type for feature level.");

	std::string textShaderFilename = "shaders/" + Filename;
	std::shared_ptr<IFile> textShaderFile = filesManager->Open(textShaderFilename);
	if (textShaderFile == nullptr)
	{
		textShaderFile = filesManager->Open(Filename);
		if (textShaderFile == nullptr)
			throw CznRenderException("Shader file '%s' not found.", textShaderFilename.c_str());
	}

	std::string text;
	if (false == textShaderFile->getText(&text))
		throw CznRenderException("Unable to read text from file '%s'", textShaderFile->Name().c_str());

	std::vector<D3D_SHADER_MACRO> macroses;
	for (const auto& macro : Macroses)
		macroses.push_back({ macro.MacrosName, macro.ValueStr });
	macroses.push_back({ nullptr, nullptr });


	CShaderDX11Include shaderInclude(m_RenderDeviceDX11.GetBaseManager());
	ATL::CComPtr<ID3DBlob> shaderBlob;
	ATL::CComPtr<ID3DBlob> errorBlob;
	if (FAILED(D3DCompile(text.c_str(), text.size(), Filename.c_str(), macroses.data(), &shaderInclude, EntryPoint.c_str(), profile.c_str(), flags, 0, &shaderBlob, &errorBlob)))
	{
		if (errorBlob != nullptr)
		{
			std::string errorBlobMessage(static_cast<char*>(errorBlob->GetBufferPointer()), errorBlob->GetBufferSize());

			Log::Error(errorBlobMessage.c_str());
			throw CznRenderException(errorBlobMessage);
		}


		throw CznRenderException("D3DCompile error. Shader: '%s':'%s'.", Filename.c_str(), EntryPoint.c_str());
	}

	auto compiledShaderByteBuffer = MakeShared(CByteBuffer);
	compiledShaderByteBuffer->writeBytes(shaderBlob->GetBufferPointer(), shaderBlob->GetBufferSize());
	return compiledShaderByteBuffer;
}

void ShaderDX11::LoadCompiledShader(std::shared_ptr<IByteBuffer> ByteBuffer)
{
	switch (m_Type)
	{
	case EShaderType::VertexShader:
		CHECK_HR(m_RenderDeviceDX11.GetDeviceD3D11()->CreateVertexShader(ByteBuffer->getData(), ByteBuffer->getSize(), nullptr, &m_VertexShader));
		break;
	case EShaderType::TessellationControlShader:
		CHECK_HR(m_RenderDeviceDX11.GetDeviceD3D11()->CreateHullShader(ByteBuffer->getData(), ByteBuffer->getSize(), nullptr, &m_HullShader));
		break;
	case EShaderType::TessellationEvaluationShader:
		CHECK_HR(m_RenderDeviceDX11.GetDeviceD3D11()->CreateDomainShader(ByteBuffer->getData(), ByteBuffer->getSize(), nullptr, &m_DomainShader));
		break;
	case EShaderType::GeometryShader:
		CHECK_HR(m_RenderDeviceDX11.GetDeviceD3D11()->CreateGeometryShader(ByteBuffer->getData(), ByteBuffer->getSize(), nullptr, &m_GeometryShader));
		break;
	case EShaderType::PixelShader:
		CHECK_HR(m_RenderDeviceDX11.GetDeviceD3D11()->CreatePixelShader(ByteBuffer->getData(), ByteBuffer->getSize(), nullptr, &m_PixelShader));
		break;
	case EShaderType::ComputeShader:
		CHECK_HR(m_RenderDeviceDX11.GetDeviceD3D11()->CreateComputeShader(ByteBuffer->getData(), ByteBuffer->getSize(), nullptr, &m_ComputeShader));
		break;
	default:
		throw CznRenderException("Unknown shader type.");
	}

	// Reflect the parameters from the shader. Inspired by: http://members.gamedev.net/JasonZ/Heiroglyph/D3D11ShaderReflection.pdf
	ATL::CComPtr<ID3D11ShaderReflection> shaderReflector;
	CHECK_HR_MSG(D3DReflect(ByteBuffer->getData(), ByteBuffer->getSize(), IID_ID3D11ShaderReflection, (void**)&shaderReflector), L"Failed to reflect shader.");

	// Query input parameters and build the input layout
	D3D11_SHADER_DESC shaderDescription = { };
	CHECK_HR_MSG(shaderReflector->GetDesc(&shaderDescription), L"Failed to get shader description from shader reflector.");

	// LAYOUT HERE

	// Query Resources that are bound to the shader.
	for (UINT i = 0; i < shaderDescription.BoundResources; ++i)
	{
		D3D11_SHADER_INPUT_BIND_DESC bindDesc = { };
		CHECK_HR(shaderReflector->GetResourceBindingDesc(i, &bindDesc));

		IShaderParameter::EType parameterType = IShaderParameter::EType::Invalid;

		switch (bindDesc.Type)
		{
		case D3D_SIT_TEXTURE:
			parameterType = IShaderParameter::EType::Texture;
			break;
		case D3D_SIT_SAMPLER:
			parameterType = IShaderParameter::EType::Sampler;
			break;
		case D3D_SIT_CBUFFER:
		case D3D_SIT_STRUCTURED:
			parameterType = IShaderParameter::EType::Buffer;
			break;
		case D3D_SIT_UAV_RWSTRUCTURED:
			parameterType = IShaderParameter::EType::RWBuffer;
			break;
		case D3D_SIT_UAV_RWTYPED:
			parameterType = IShaderParameter::EType::RWTexture;
			break;
		}

		// Create an empty shader parameter that should be filled-in by the application.
		std::shared_ptr<IShaderParameter> shaderParameter = MakeShared(ShaderParameterBase, bindDesc.Name, bindDesc.BindPoint, this, parameterType);
		m_ShaderParameters.insert(ParameterMap::value_type(bindDesc.Name, shaderParameter));
	}
}

void ShaderDX11::Destroy()
{
	m_PixelShader.Release();
	m_DomainShader.Release();
	m_HullShader.Release();
	m_GeometryShader.Release();
	m_VertexShader.Release();
	m_ComputeShader.Release();

	m_Type = EShaderType::UnknownShaderType;
	m_FileName.clear();
	m_EntryPoint.clear();
	m_Macros.clear();

	m_InputLayout.reset();
	m_ShaderParameters.clear();
}

