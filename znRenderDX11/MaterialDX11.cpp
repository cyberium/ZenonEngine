#include "stdafx.h"

// General
#include "MaterialDX11.h"

MaterialDX11::MaterialDX11(IRenderDeviceDX11& RenderDeviceDX11, size_t Size)
	: MaterialBase(RenderDeviceDX11, Size)
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
			const auto& texture = textureIt.second;
			if (texture != nullptr) // TODO: ASSERT

			texture->Bind((uint32_t)textureIt.first, shader, IShaderParameter::Type::Texture);
		}

		for (const auto& samplerStateIt : m_Samplers)
		{
			const auto& samplerState = samplerStateIt.second;
			if (samplerState != nullptr) // TODO: ASSERT

			samplerState->Bind((uint32_t)samplerStateIt.first, shader, IShaderParameter::Type::Sampler);
		}
	}

	auto& materialParameter = shader->GetShaderParameterByName("Material");
	if (materialParameter.IsValid() && m_pConstantBuffer != nullptr)
	{
		materialParameter.SetConstantBuffer(m_pConstantBuffer);
		materialParameter.Bind();
	}
}

void MaterialDX11::UnbindForShader(const IShader* shader) const
{
	_ASSERT_EXPR(shader != nullptr, L"Shader must be not null.");

	if (shader->GetType() == EShaderType::PixelShader)
	{
		for (const auto& textureIt : m_Textures)
		{
			const auto& texture = textureIt.second;
			if (texture != nullptr) // TODO: ASSERT

			texture->UnBind((uint32_t)textureIt.first, shader, IShaderParameter::Type::Texture);
		}

		for (const auto& samplerStateIt : m_Samplers)
		{
			const auto& samplerState = samplerStateIt.second;
			if (samplerState != nullptr) // TODO: ASSERT

			samplerState->UnBind((uint32_t)samplerStateIt.first, shader, IShaderParameter::Type::Sampler);
		}
	}

	auto& materialParameter = shader->GetShaderParameterByName("Material");
	if (materialParameter.IsValid() && m_pConstantBuffer != nullptr)
	{
		materialParameter.Unbind();
	}
}
