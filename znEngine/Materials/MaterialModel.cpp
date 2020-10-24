#include "stdafx.h"

// General
#include "MaterialModel.h"

const char* cTextureTypeNames[] =
{
	"Diffuse",
	"Emissive",
	"Ambient",
	"Specular",
	"Shininess",
	"NormalMap",
	"Bump",
	"Transparency",
	"Reflection",
	"Displacement"
};

MaterialModel::MaterialModel(const IBaseManager& BaseManager)
	: MaterialProxieT(BaseManager.GetApplication().GetRenderDevice(), "MaterialModel")
	, m_BaseManager(BaseManager)
{
}

MaterialModel::~MaterialModel()
{

}

void MaterialModel::SetEmissiveColor(const glm::vec3 & Color)
{
	MaterialData().Emissive = Color;
	MarkMaterialDataDirty();
}

void MaterialModel::SetAmbientColor(const glm::vec3 & Color)
{
	MaterialData().Ambient = Color;
	MarkMaterialDataDirty();
}

void MaterialModel::SetDiffuseColor(const glm::vec3 & Color)
{
	MaterialData().Diffuse = Color;
	MarkMaterialDataDirty();
}

void MaterialModel::SetBump(const glm::vec3 & Color)
{
	MaterialData().Bump = Color;
	MarkMaterialDataDirty();
}

void MaterialModel::SetBumpFactor(float Factor)
{
	MaterialData().BumpFactor = Factor;
	MarkMaterialDataDirty();
}

void MaterialModel::SetSpecularColor(const glm::vec3 & Color)
{
	MaterialData().Specular = Color;
	MarkMaterialDataDirty();
}

void MaterialModel::SetSpecularFactor(float Factor)
{
	MaterialData().SpecularFactor = Factor;
	MarkMaterialDataDirty();
}

void MaterialModel::SetReflectionColor(const glm::vec3 & Color)
{
	MaterialData().Reflection = Color;
	MarkMaterialDataDirty();
}

void MaterialModel::SetReflectionFactor(float Factor)
{
	MaterialData().ReflectionFactor = Factor;
	MarkMaterialDataDirty();
}

void MaterialModel::SetTexture(ETextureType TextureType, std::shared_ptr<ITexture> texture)
{
	switch (TextureType)
	{
		case ETextureType::TextureDiffuse:
		{
			MaterialData().HasTextureDiffuse = true;
			MarkMaterialDataDirty();
		}
		break;
		case ETextureType::TextureEmissive:
		{
			MaterialData().HasTextureEmissive = true;
			MarkMaterialDataDirty();
		}
		break;
		case ETextureType::TextureAmbient:
		{
			MaterialData().HasTextureAmbient = true;
			MarkMaterialDataDirty();
		}
		break;
		case ETextureType::TextureSpecular:
		{
			MaterialData().HasTextureSpecular = true;
			MarkMaterialDataDirty();
		}
		break;
		case ETextureType::TextureShininess:
		{
			MaterialData().HasTextureShininess = true;
			MarkMaterialDataDirty();
		}
		break;
		case ETextureType::TextureNormalMap:
		{
			MaterialData().HasTextureNormalMap = true;
			MarkMaterialDataDirty();
		}
		break;
		case ETextureType::TextureBump:
		{
			MaterialData().HasTextureBump = true;
			MarkMaterialDataDirty();
		}
		break;
		case ETextureType::TextureTransparency:
		{
			MaterialData().HasTextureTransparency = true;
			MarkMaterialDataDirty();
		}
		break;
		case ETextureType::TextureReflection:
		{
			MaterialData().HasTextureReflection = true;
			MarkMaterialDataDirty();
		}
		break;
		case ETextureType::TextureDisplacement:
		{
			MaterialData().HasTextureDisplacement = true;
			MarkMaterialDataDirty();
		}
		break;
		default:
			_ASSERT(false);
	};

	MaterialProxie::SetTexture((uint32)TextureType, texture);
}

const std::shared_ptr<ITexture>& MaterialModel::GetTexture(ETextureType TextureType) const
{
	return MaterialProxie::GetTexture((uint32)TextureType);
}



//
// IMaterial
//
std::string MaterialModel::GetTextureTypeName(uint8 ID) const
{
	return cTextureTypeNames[ID];
}



//
// Protected
//
void MaterialModel::PrintInfo()
{
	Log::Info("--------------------------------------------------");
	Log::Info("MaterialModel: ");
	Log::Info("Emissive: [%f, %f, %f]. Factor [%f].", MaterialData().Emissive.r, MaterialData().Emissive.g, MaterialData().Emissive.b, MaterialData().EmissiveFactor);
	Log::Info("Ambient: [%f, %f, %f]. Factor [%f].", MaterialData().Ambient.r, MaterialData().Ambient.g, MaterialData().Ambient.b, MaterialData().AmbientFactor);
	Log::Info("Diffuse: [%f, %f, %f]. Factor [%f].", MaterialData().Diffuse.r, MaterialData().Diffuse.g, MaterialData().Diffuse.b, MaterialData().DiffuseFactor);
	Log::Info("NormalMap: [%f, %f, %f]. Shrineses [%f].", MaterialData().NormalMap.r, MaterialData().NormalMap.g, MaterialData().NormalMap.b, MaterialData().Shininess);
	Log::Info("Bump: [%f, %f, %f]. Factor [%f].", MaterialData().Bump.r, MaterialData().Bump.g, MaterialData().Bump.b, MaterialData().BumpFactor);
	Log::Info("TransparentColor: [%f, %f, %f]. Factor [%f].", MaterialData().TransparentColor.r, MaterialData().TransparentColor.g, MaterialData().TransparentColor.b, MaterialData().TransparencyFactor);
	Log::Info("DisplacementColor: [%f, %f, %f]. Factor [%f].", MaterialData().DisplacementColor.r, MaterialData().DisplacementColor.g, MaterialData().DisplacementColor.b, MaterialData().DisplacementFactor);
	Log::Info("VectorDisplacementColor: [%f, %f, %f]. Factor [%f].", MaterialData().VectorDisplacementColor.r, MaterialData().VectorDisplacementColor.g, MaterialData().VectorDisplacementColor.b, MaterialData().VectorDisplacementFactor);
	Log::Info("Specular: [%f, %f, %f]. Factor [%f].", MaterialData().Specular.r, MaterialData().Specular.g, MaterialData().Specular.b, MaterialData().SpecularFactor);
	Log::Info("VectorDisplacementColor: [%f, %f, %f]. Factor [%f].", MaterialData().Reflection.r, MaterialData().Reflection.g, MaterialData().Reflection.b, MaterialData().ReflectionFactor);
	Log::Info("");
}
