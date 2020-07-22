#include "stdafx.h"

// General
#include "MaterialDX11.h"

MaterialDX11::MaterialDX11(IRenderDeviceDX11& RenderDeviceDX11)
	: MaterialBase(RenderDeviceDX11)
{
}

MaterialDX11::~MaterialDX11()
{}

void MaterialDX11::Bind(const ShaderMap& shaders) const
{
    MaterialBase::Bind(shaders);

	// means shader in Pipeline state
	if (m_Shaders.empty())
	{
		for (const auto& shader : shaders)
		{
			BindForShader(shader.second.get());
		}
	}
	else
	{
		_ASSERT(false);
		for (const auto& shader : m_Shaders)
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
		for (const auto& shader : shaders)
		{
			UnbindForShader(shader.second.get());
		}
	}
	else
	{
		_ASSERT(false);
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

	if (shader->GetShaderType() == EShaderType::PixelShader)
	{
		for (const auto& textureIt : m_Textures)
		{
			const auto& texture = textureIt.second;
			if (texture != nullptr) // TODO: ASSERT
				texture->Bind((uint32)textureIt.first, shader, IShaderParameter::Type::Texture);
		}

		for (const auto& samplerStateIt : m_Samplers)
		{
			const auto& samplerState = samplerStateIt.second;
			if (samplerState != nullptr) // TODO: ASSERT
				samplerState->Bind((uint32)samplerStateIt.first, shader, IShaderParameter::Type::Sampler);
		}
	}

	if (m_ConstantBuffer)
	{
		auto& materialParameter = shader->GetShaderParameterByName("Material");
		if (materialParameter.IsValid())
		{
			materialParameter.SetConstantBuffer(m_ConstantBuffer);
			materialParameter.Bind();
		}
	}
}

void MaterialDX11::UnbindForShader(const IShader* shader) const
{
	_ASSERT_EXPR(shader != nullptr, L"Shader must be not null.");

	if (m_ConstantBuffer)
	{
		auto& materialParameter = shader->GetShaderParameterByName("Material");
		if (materialParameter.IsValid())
		{
			materialParameter.Unbind();
		}
	}

	if (shader->GetShaderType() == EShaderType::PixelShader)
	{
		for (const auto& textureIt : m_Textures)
		{
			const auto& texture = textureIt.second;
			if (texture != nullptr) // TODO: ASSERT
				texture->UnBind((uint32)textureIt.first, shader, IShaderParameter::Type::Texture);
		}

		for (const auto& samplerStateIt : m_Samplers)
		{
			const auto& samplerState = samplerStateIt.second;
			if (samplerState != nullptr) // TODO: ASSERT
				samplerState->UnBind((uint32)samplerStateIt.first, shader, IShaderParameter::Type::Sampler);
		}
	}
}
