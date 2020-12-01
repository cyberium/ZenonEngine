#include "stdafx.h"

// General
#include "MaterialDX11.h"

MaterialDX11::MaterialDX11(IRenderDeviceDX11& RenderDeviceDX11)
	: MaterialBase(RenderDeviceDX11)
{
}

MaterialDX11::~MaterialDX11()
{}



//
// IMaterial
//
void MaterialDX11::Bind(const IShader* PixelShader) const
{
    MaterialBase::Bind(PixelShader);

	BindForShader(PixelShader);
}

void MaterialDX11::Unbind(const IShader* PixelShader) const
{
	UnbindForShader(PixelShader);

    MaterialBase::Unbind(PixelShader);
}



//
// Private
//
void MaterialDX11::BindForShader(const IShader* shader) const
{
	_ASSERT_EXPR(shader != nullptr, L"Shader must be not null.");

	for (const auto& textureIt : m_Textures)
	{
		const auto& texture = textureIt.second;
		if (texture != nullptr) // TODO: ASSERT
			texture->Bind((uint32)textureIt.first, shader, IShaderParameter::EType::Texture);
	}

	for (const auto& samplerStateIt : m_Samplers)
	{
		const auto& samplerState = samplerStateIt.second;
		if (samplerState != nullptr) // TODO: ASSERT
			samplerState->Bind((uint32)samplerStateIt.first, shader, IShaderParameter::EType::Sampler);
	}

	if (m_ConstantBuffer)
	{
		auto* materialParameter = shader->GetShaderParameterByName("Material");
		if (materialParameter)
		{
			materialParameter->SetConstantBuffer(m_ConstantBuffer);
			materialParameter->Bind();
		}
	}
}

void MaterialDX11::UnbindForShader(const IShader* shader) const
{
	_ASSERT_EXPR(shader != nullptr, L"Shader must be not null.");

	if (m_ConstantBuffer)
	{
		auto* materialParameter = shader->GetShaderParameterByName("Material");
		if (materialParameter)
		{
			materialParameter->Unbind();
		}
	}

	for (const auto& textureIt : m_Textures)
	{
		const auto& texture = textureIt.second;
		if (texture != nullptr) // TODO: ASSERT
			texture->UnBind((uint32)textureIt.first, shader, IShaderParameter::EType::Texture);
	}

	for (const auto& samplerStateIt : m_Samplers)
	{
		const auto& samplerState = samplerStateIt.second;
		if (samplerState != nullptr) // TODO: ASSERT
			samplerState->UnBind((uint32)samplerStateIt.first, shader, IShaderParameter::EType::Sampler);
	}
}
