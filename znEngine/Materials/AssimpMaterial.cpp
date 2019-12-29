#include "stdafx.h"

// General
#include "AssimpMaterial.h"


CAssimpMaterial::CAssimpMaterial(std::shared_ptr<IRenderDevice> RenderDevice)
	: MaterialProxie(RenderDevice->CreateMaterial(sizeof(MaterialProperties)))
{
	m_pProperties = (MaterialProperties*)_aligned_malloc(sizeof(MaterialProperties), 16);
	*m_pProperties = MaterialProperties();

	std::shared_ptr<ISamplerState> repeatSampler = RenderDevice->CreateSamplerState();
	repeatSampler->SetFilter(ISamplerState::MinFilter::MinLinear, ISamplerState::MagFilter::MagLinear, ISamplerState::MipFilter::MipLinear);
	repeatSampler->SetWrapMode(ISamplerState::WrapMode::Repeat, ISamplerState::WrapMode::Repeat);
	SetSampler(0, repeatSampler);

	std::shared_ptr<ISamplerState> clampSampler = RenderDevice->CreateSamplerState();
	clampSampler->SetFilter(ISamplerState::MinFilter::MinLinear, ISamplerState::MagFilter::MagLinear, ISamplerState::MipFilter::MipLinear);
	clampSampler->SetWrapMode(ISamplerState::WrapMode::Clamp, ISamplerState::WrapMode::Clamp);
	SetSampler(1, clampSampler);
}

CAssimpMaterial::~CAssimpMaterial()
{
	if (m_pProperties)
	{
		_aligned_free(m_pProperties);
		m_pProperties = nullptr;
	}
}


const glm::vec4& CAssimpMaterial::GetGlobalAmbientColor() const
{
	return m_pProperties->m_GlobalAmbient;
}

void CAssimpMaterial::SetGlobalAmbientColor(const glm::vec4& globalAmbient)
{
	m_pProperties->m_GlobalAmbient = globalAmbient;
	MarkConstantBufferDirty();
}

const glm::vec4& CAssimpMaterial::GetAmbientColor() const
{
	return m_pProperties->m_AmbientColor;
}

void CAssimpMaterial::SetAmbientColor(const glm::vec4& ambient)
{
	m_pProperties->m_AmbientColor = ambient;
	MarkConstantBufferDirty();
}




std::shared_ptr<ITexture> CAssimpMaterial::GetTexture(TextureType Type) const
{
	return MaterialProxie::GetTexture((uint8)Type);
}

void CAssimpMaterial::SetTexture(TextureType Type, std::shared_ptr<ITexture> texture)
{
	MaterialProxie::SetTexture((uint8)Type, texture);

	switch (Type)
	{
		case TextureType::Ambient:
		{
			m_pProperties->m_HasAmbientTexture = (texture != nullptr);
		}
		break;
		case TextureType::Emissive:
		{
			m_pProperties->m_HasEmissiveTexture = (texture != nullptr);
		}
		break;
		case TextureType::Diffuse:
		{
			m_pProperties->m_HasDiffuseTexture = (texture != nullptr);
		}
		break;
		case TextureType::Specular:
		{
			m_pProperties->m_HasSpecularTexture = (texture != nullptr);
		}
		break;
		case TextureType::SpecularPower:
		{
			m_pProperties->m_HasSpecularPowerTexture = (texture != nullptr);
		}
		break;
		case TextureType::Normal:
		{
			m_pProperties->m_HasNormalTexture = (texture != nullptr);
		}
		break;
		case TextureType::Bump:
		{
			m_pProperties->m_HasBumpTexture = (texture != nullptr);
		}
		break;
		case TextureType::Opacity:
		{
			m_pProperties->m_HasOpacityTexture = (texture != nullptr);
		}
		break;
	}
	MarkConstantBufferDirty();
}

const glm::vec4& CAssimpMaterial::GetDiffuseColor() const
{
	return m_pProperties->m_DiffuseColor;
}

void CAssimpMaterial::SetDiffuseColor(const glm::vec4& diffuse)
{
	m_pProperties->m_DiffuseColor = diffuse;
	MarkConstantBufferDirty();
}

const glm::vec4& CAssimpMaterial::GetEmissiveColor() const
{
	return m_pProperties->m_EmissiveColor;
}

void CAssimpMaterial::SetEmissiveColor(const glm::vec4& emissive)
{
	m_pProperties->m_EmissiveColor = emissive;
	MarkConstantBufferDirty();
}

const glm::vec4& CAssimpMaterial::GetSpecularColor() const
{
	return m_pProperties->m_SpecularColor;
}

void CAssimpMaterial::SetSpecularColor(const glm::vec4& specular)
{
	m_pProperties->m_SpecularColor = specular;
	MarkConstantBufferDirty();
}

float CAssimpMaterial::GetSpecularPower() const
{
	return m_pProperties->m_SpecularPower;
}

void CAssimpMaterial::SetSpecularPower(float phongPower)
{
	m_pProperties->m_SpecularPower = phongPower;
	MarkConstantBufferDirty();
}

const glm::vec4& CAssimpMaterial::GetReflectance() const
{
	return m_pProperties->m_Reflectance;
}

void CAssimpMaterial::SetReflectance(const glm::vec4& reflectance)
{
	m_pProperties->m_Reflectance = reflectance;
	MarkConstantBufferDirty();
}


const float CAssimpMaterial::GetOpacity() const
{
	return m_pProperties->m_Opacity;
}

void CAssimpMaterial::SetOpacity(float Opacity)
{
	m_pProperties->m_Opacity = Opacity;
	MarkConstantBufferDirty();
}

float CAssimpMaterial::GetIndexOfRefraction() const
{
	return m_pProperties->m_IndexOfRefraction;
}

void CAssimpMaterial::SetIndexOfRefraction(float indexOfRefraction)
{
	m_pProperties->m_IndexOfRefraction = indexOfRefraction;
	MarkConstantBufferDirty();
}

float CAssimpMaterial::GetBumpIntensity() const
{
	return m_pProperties->m_BumpIntensity;
}
void CAssimpMaterial::SetBumpIntensity(float bumpIntensity)
{
	m_pProperties->m_BumpIntensity = bumpIntensity;
	MarkConstantBufferDirty();
}

void CAssimpMaterial::UpdateConstantBuffer() const
{
	MaterialProxie::UpdateConstantBuffer(m_pProperties, sizeof(MaterialProperties));
}