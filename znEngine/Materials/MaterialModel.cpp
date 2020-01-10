#include "stdafx.h"

// General
#include "MaterialModel.h"

MaterialModel::MaterialModel(const IBaseManager* BaseManager)
	: MaterialProxie(BaseManager->GetManager<IRenderDevice>()->CreateMaterial(sizeof(MaterialProperties)))
	, m_BaseManager(BaseManager)
	, m_RenderDevice(BaseManager->GetManager<IRenderDevice>())
{
	// Constant buffer
	m_pProperties = (MaterialProperties*)_aligned_malloc(sizeof(MaterialProperties), 16);
	*m_pProperties = MaterialProperties();

	std::shared_ptr<ISamplerState> g_Sampler = m_RenderDevice->CreateSamplerState();
	g_Sampler->SetFilter(ISamplerState::MinFilter::MinLinear, ISamplerState::MagFilter::MagLinear, ISamplerState::MipFilter::MipLinear);
	g_Sampler->SetWrapMode(ISamplerState::WrapMode::Repeat, ISamplerState::WrapMode::Repeat);
	SetSampler(0, g_Sampler);

	std::shared_ptr<ISamplerState> g_Sampler2 = m_RenderDevice->CreateSamplerState();
	g_Sampler2->SetFilter(ISamplerState::MinFilter::MinLinear, ISamplerState::MagFilter::MagLinear, ISamplerState::MipFilter::MipLinear);
	g_Sampler2->SetWrapMode(ISamplerState::WrapMode::Clamp, ISamplerState::WrapMode::Clamp);
	SetSampler(1, g_Sampler2);
}

MaterialModel::~MaterialModel()
{
	if (m_pProperties)
	{
		_aligned_free(m_pProperties);
		m_pProperties = nullptr;
	}
}

void MaterialModel::SetEmissiveColor(const glm::vec3 & Color)
{
	m_pProperties->Emissive = Color;
	MarkConstantBufferDirty();
}

void MaterialModel::SetAmbientColor(const glm::vec3 & Color)
{
	m_pProperties->Ambient = Color;
	MarkConstantBufferDirty();
}

void MaterialModel::SetDiffuseColor(const glm::vec3 & Color)
{
	m_pProperties->Diffuse = Color;
	MarkConstantBufferDirty();
}

void MaterialModel::SetBump(const glm::vec3 & Color)
{
	m_pProperties->Bump = Color;
	MarkConstantBufferDirty();
}

void MaterialModel::SetBumpFactor(float Factor)
{
	m_pProperties->BumpFactor = Factor;
	MarkConstantBufferDirty();
}

void MaterialModel::SetSpecularColor(const glm::vec3 & Color)
{
	m_pProperties->Specular = Color;
	MarkConstantBufferDirty();
}

void MaterialModel::SetSpecularFactor(float Factor)
{
	m_pProperties->SpecularFactor = Factor;
	MarkConstantBufferDirty();
}

void MaterialModel::SetReflectionColor(const glm::vec3 & Color)
{
	m_pProperties->Reflection = Color;
	MarkConstantBufferDirty();
}

void MaterialModel::SetReflectionFactor(float Factor)
{
	m_pProperties->ReflectionFactor = Factor;
	MarkConstantBufferDirty();
}

std::shared_ptr<ITexture> MaterialModel::GetTexture(ETextureType TextureType) const
{
	return MaterialProxie::GetTexture((uint32)TextureType);
}

void MaterialModel::SetTexture(ETextureType TextureType, std::shared_ptr<ITexture> texture)
{
	switch (TextureType)
	{
		case ETextureType::TextureDiffuse:
		{
			m_pProperties->HasTextureDiffuse = true;
			MarkConstantBufferDirty();
		}
		break;
		case ETextureType::TextureEmissive:
		{
			m_pProperties->HasTextureEmissive = true;
			MarkConstantBufferDirty();
		}
		break;
		case ETextureType::TextureAmbient:
		{
			m_pProperties->HasTextureAmbient = true;
			MarkConstantBufferDirty();
		}
		break;
		case ETextureType::TextureSpecular:
		{
			m_pProperties->HasTextureSpecular = true;
			MarkConstantBufferDirty();
		}
		break;
		case ETextureType::TextureShininess:
		{
			m_pProperties->HasTextureShininess = true;
			MarkConstantBufferDirty();
		}
		break;
		case ETextureType::TextureNormalMap:
		{
			m_pProperties->HasTextureNormalMap = true;
			MarkConstantBufferDirty();
		}
		break;
		case ETextureType::TextureBump:
		{
			m_pProperties->HasTextureBump = true;
			MarkConstantBufferDirty();
		}
		break;
		case ETextureType::TextureTransparency:
		{
			m_pProperties->HasTextureTransparency = true;
			MarkConstantBufferDirty();
		}
		break;
		case ETextureType::TextureReflection:
		{
			m_pProperties->HasTextureReflection = true;
			MarkConstantBufferDirty();
		}
		break;
		case ETextureType::TextureDisplacement:
		{
			m_pProperties->HasTextureDisplacement = true;
			MarkConstantBufferDirty();
		}
		break;
		default:
			_ASSERT(false);
	};

	PrintInfo();

	MaterialProxie::SetTexture((uint32)TextureType, texture);
}


//
// Protected
//
void MaterialModel::UpdateConstantBuffer() const
{
	MaterialProxie::UpdateConstantBuffer(m_pProperties, sizeof(MaterialProperties));
}

void MaterialModel::PrintInfo()
{
	Log::Info("--------------------------------------------------");
	Log::Info("MaterialModel: ");
	Log::Info("Emissive: [%f, %f, %f]. Factor [%f].", m_pProperties->Emissive.r, m_pProperties->Emissive.g, m_pProperties->Emissive.b, m_pProperties->EmissiveFactor);
	Log::Info("Ambient: [%f, %f, %f]. Factor [%f].", m_pProperties->Ambient.r, m_pProperties->Ambient.g, m_pProperties->Ambient.b, m_pProperties->AmbientFactor);
	Log::Info("Diffuse: [%f, %f, %f]. Factor [%f].", m_pProperties->Diffuse.r, m_pProperties->Diffuse.g, m_pProperties->Diffuse.b, m_pProperties->DiffuseFactor);
	Log::Info("NormalMap: [%f, %f, %f]. Shrineses [%f].", m_pProperties->NormalMap.r, m_pProperties->NormalMap.g, m_pProperties->NormalMap.b, m_pProperties->Shininess);
	Log::Info("Bump: [%f, %f, %f]. Factor [%f].", m_pProperties->Bump.r, m_pProperties->Bump.g, m_pProperties->Bump.b, m_pProperties->BumpFactor);
	Log::Info("TransparentColor: [%f, %f, %f]. Factor [%f].", m_pProperties->TransparentColor.r, m_pProperties->TransparentColor.g, m_pProperties->TransparentColor.b, m_pProperties->TransparencyFactor);
	Log::Info("DisplacementColor: [%f, %f, %f]. Factor [%f].", m_pProperties->DisplacementColor.r, m_pProperties->DisplacementColor.g, m_pProperties->DisplacementColor.b, m_pProperties->DisplacementFactor);
	Log::Info("VectorDisplacementColor: [%f, %f, %f]. Factor [%f].", m_pProperties->VectorDisplacementColor.r, m_pProperties->VectorDisplacementColor.g, m_pProperties->VectorDisplacementColor.b, m_pProperties->VectorDisplacementFactor);
	Log::Info("Specular: [%f, %f, %f]. Factor [%f].", m_pProperties->Specular.r, m_pProperties->Specular.g, m_pProperties->Specular.b, m_pProperties->SpecularFactor);
	Log::Info("VectorDisplacementColor: [%f, %f, %f]. Factor [%f].", m_pProperties->Reflection.r, m_pProperties->Reflection.g, m_pProperties->Reflection.b, m_pProperties->ReflectionFactor);
	Log::Info("");
}
