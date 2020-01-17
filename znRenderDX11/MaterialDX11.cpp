#include "stdafx.h"

// General
#include "MaterialDX11.h"

MaterialDX11::MaterialDX11(std::weak_ptr<IRenderDevice> renderDevice, size_t Size)
	: MaterialBase(renderDevice, Size)
{}

MaterialDX11::~MaterialDX11()
{}

void MaterialDX11::Bind(const ShaderMap& shaders) const
{
    MaterialBase::Bind(shaders);

	// means shader in Pipeline state
	if (m_Shaders.empty())
	{
		for (auto shader : shaders)
		{
			BindForShader(shader.second.get());
		}
	}
	else
	{
		for (auto shader : m_Shaders)
		{
			shader.second->Bind();
			BindForShader(shader.second.get());
		}
	}
}

void MaterialDX11::Unbind(const ShaderMap& shaders) const
{
	if (m_Shaders.empty())
	{
		for (auto shader : shaders)
		{
			UnbindForShader(shader.second.get());
		}
	}
	else
	{
		for (const auto& it : m_Shaders)
		{
			UnbindForShader(it.second.get());
		}
	}

    MaterialBase::Unbind(shaders);
}

void MaterialDX11::BindForShader(const IShader* shader) const
{
	_ASSERT_EXPR(shader != nullptr, L"Shader must be not null.");

	if (shader->GetType() == EShaderType::PixelShader)
	{
		for (const auto& textureIt : m_Textures)
		{
			const ITexture* texture = textureIt.second.get();
			_ASSERT(texture != nullptr);

			texture->Bind((uint32_t)textureIt.first, shader, IShaderParameter::Type::Texture);
		}

		for (const auto& samplerStateIt : m_Samplers)
		{
			const ISamplerState* samplerState = samplerStateIt.second.get();
			_ASSERT(samplerState != nullptr);

			samplerState->Bind((uint32_t)samplerStateIt.first, shader, IShaderParameter::Type::Sampler);
		}
	}

	const std::shared_ptr<IShaderParameter>& materialParameter = shader->GetShaderParameterByName("Material");
	if (materialParameter->IsValid() && m_pConstantBuffer != nullptr)
	{
		materialParameter->SetConstantBuffer(m_pConstantBuffer.get());
		materialParameter->Bind();
	}
}

void MaterialDX11::UnbindForShader(const IShader* shader) const
{
	_ASSERT_EXPR(shader != nullptr, L"Shader must be not null.");

	if (shader->GetType() == EShaderType::PixelShader)
	{
		for (const auto& textureIt : m_Textures)
		{
			const ITexture* texture = textureIt.second.get();
			_ASSERT(texture != nullptr);

			texture->UnBind((uint32_t)textureIt.first, shader, IShaderParameter::Type::Texture);
		}

		for (const auto& samplerStateIt : m_Samplers)
		{
			const ISamplerState* samplerState = samplerStateIt.second.get();
			_ASSERT(samplerState != nullptr);

			samplerState->UnBind((uint32_t)samplerStateIt.first, shader, IShaderParameter::Type::Sampler);
		}
	}

	const std::shared_ptr<IShaderParameter>& materialParameter = shader->GetShaderParameterByName("Material");
	if (materialParameter->IsValid() && m_pConstantBuffer != nullptr)
	{
		materialParameter->Unbind();
	}
}
