#include "stdafx.h"

// General
#include "MaterialModel.h"

const char* cTextureTypeNames[] =
{ 
	"Ambient",
	"Diffuse",
	"Specular",
	"Emissive",

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
	// Ambient
	{
		auto prop = MakeShared(CPropertyWrappedVec3, "AmbientColor", "descr", glm::vec3(0.0f));
		prop->SetValueSetter(std::bind(&MaterialModel::SetAmbientColor, this, std::placeholders::_1));
		prop->SetValueGetter(std::bind(&MaterialModel::GetAmbientColor, this));
		GetProperties()->AddProperty(prop);
	}

	// Diffuse
	{
		auto prop = MakeShared(CPropertyWrappedVec3, "DiffuseColor", "descr", glm::vec3(1.0f));
		prop->SetValueSetter(std::bind(&MaterialModel::SetDiffuseColor, this, std::placeholders::_1));
		prop->SetValueGetter(std::bind(&MaterialModel::GetDiffuseColor, this));
		GetProperties()->AddProperty(prop);
	}

	// Specular
	{
		auto prop = MakeShared(CPropertyWrappedVec3, "SpecularColor", "descr", glm::vec3(1.0f));
		prop->SetValueSetter(std::bind(&MaterialModel::SetSpecularColor, this, std::placeholders::_1));
		prop->SetValueGetter(std::bind(&MaterialModel::GetSpecularColor, this));
		GetProperties()->AddProperty(prop);
	}

	// Emissive
	{
		auto prop = MakeShared(CPropertyWrappedVec3, "EmissiveColor", "descr", glm::vec3(1.0f));
		prop->SetValueSetter(std::bind(&MaterialModel::SetEmissiveColor, this, std::placeholders::_1));
		prop->SetValueGetter(std::bind(&MaterialModel::GetEmissiveColor, this));
		GetProperties()->AddProperty(prop);
	}
}

MaterialModel::~MaterialModel()
{

}



void MaterialModel::SetAmbientColor(glm::vec3 Color)
{
	MaterialData().Ambient = Color;
	MarkMaterialDataDirty();
}

glm::vec3 MaterialModel::GetAmbientColor() const
{
	return MaterialDataReadOnly().Ambient;
}

void MaterialModel::SetAmbientFactor(float Factor)
{
	MaterialData().AmbientFactor = Factor;
	MarkMaterialDataDirty();
}



void MaterialModel::SetDiffuseColor(glm::vec3 Color)
{
	MaterialData().Diffuse = Color;
	MarkMaterialDataDirty();
}

glm::vec3 MaterialModel::GetDiffuseColor() const
{
	return MaterialDataReadOnly().Diffuse;
}

void MaterialModel::SetDiffuseFactor(float Factor)
{
	MaterialData().DiffuseFactor = Factor;
	MarkMaterialDataDirty();
}



void MaterialModel::SetSpecularColor(glm::vec3 Color)
{
	MaterialData().Specular = Color;
	MarkMaterialDataDirty();
}

glm::vec3 MaterialModel::GetSpecularColor() const
{
	return MaterialDataReadOnly().Specular;
}

void MaterialModel::SetSpecularFactor(float Factor)
{
	MaterialData().SpecularFactor = Factor;
	MarkMaterialDataDirty();
}



void MaterialModel::SetEmissiveColor(glm::vec3 Color)
{
	MaterialData().Emissive = Color;
	MarkMaterialDataDirty();
}

glm::vec3 MaterialModel::GetEmissiveColor() const
{
	return MaterialDataReadOnly().Emissive;
}

void MaterialModel::SetEmissiveFactor(float Factor)
{
	MaterialData().EmissiveFactor = Factor;
	MarkMaterialDataDirty();
}



void MaterialModel::SetShininess(float Factor)
{
	MaterialData().Shininess = Factor;
	MarkMaterialDataDirty();
}

void MaterialModel::SetBumpFactor(float Factor)
{
	MaterialData().BumpFactor = Factor;
	MarkMaterialDataDirty();
}



void MaterialModel::SetTexture(ETextureType TextureType, std::shared_ptr<ITexture> texture)
{
	switch (TextureType)
	{
		case ETextureType::TextureAmbient:
		{
			MaterialData().HasTextureAmbient = true;
			MarkMaterialDataDirty();
		}
		break;
		case ETextureType::TextureDiffuse:
		{
			MaterialData().HasTextureDiffuse = true;
			MarkMaterialDataDirty();
		}
		break;
		case ETextureType::TextureSpecular:
		{
			MaterialData().HasTextureSpecular = true;
			MarkMaterialDataDirty();
		}
		break;
		case ETextureType::TextureEmissive:
		{
			MaterialData().HasTextureEmissive = true;
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
// IObjectLoadSave
//
void MaterialModel::Load(const std::shared_ptr<IXMLReader>& Reader)
{
	GetProperties()->Load(Reader);

	auto texturesReader = Reader->GetChild("Textures");
	for (const auto& textureNode : texturesReader->GetChilds())
	{
		MaterialModel::ETextureType textureID = GetTextureIDByTextureTypeName(textureNode->GetName().c_str());

		if (__super::GetTexture(textureID) != nullptr)
		{
			Log::Error("MaterialModel: Texture with ID '%d' already set.", textureID);
			continue;
		}

		std::string textureFileName = textureNode->GetValue();

		auto texture = m_BaseManager.GetManager<IznTexturesFactory>()->LoadTexture2D(textureFileName);
		if (texture == nullptr)
		{
			Log::Error("MaterialModel: Unable load texture: '%s' with ID '%d' for material '%s'.", textureFileName.c_str(), textureID, GetName().c_str());
			texture = m_BaseManager.GetManager<IznTexturesFactory>()->GetDefaultTexture();
		}

		SetTexture(textureID, texture);
	}
}

void MaterialModel::Save(const std::shared_ptr<IXMLWriter>& Writer) const
{
	GetProperties()->Save(Writer);

	auto texturesWriter = Writer->CreateChild("Textures");
	for (size_t i = 0; i < ETextureType::Count; i++)
	{
		auto texture = __super::GetTexture(i);
		if (texture == nullptr)
			continue;

		const auto textureFileName = texture->GetFilename();
		if (textureFileName.empty())
			throw CException("Empty filename for texture with ID '%d'.", i);

		auto textureWriter = texturesWriter->CreateChild(GetTextureTypeNameByTextureID(static_cast<MaterialModel::ETextureType>(i)));
		textureWriter->SetValue(textureFileName);
	}
}



//
// Protected
//
void MaterialModel::PrintInfo()
{
	Log::Info("Ambient: [%f, %f, %f]. Factor [%f].", MaterialData().Ambient.r, MaterialData().Ambient.g, MaterialData().Ambient.b, MaterialData().AmbientFactor);
	Log::Info("Diffuse: [%f, %f, %f]. Factor [%f].", MaterialData().Diffuse.r, MaterialData().Diffuse.g, MaterialData().Diffuse.b, MaterialData().DiffuseFactor);
	Log::Info("Specular: [%f, %f, %f]. Factor [%f].", MaterialData().Specular.r, MaterialData().Specular.g, MaterialData().Specular.b, MaterialData().SpecularFactor);
	Log::Info("Emissive: [%f, %f, %f]. Factor [%f].", MaterialData().Emissive.r, MaterialData().Emissive.g, MaterialData().Emissive.b, MaterialData().EmissiveFactor);
}

std::string MaterialModel::GetTextureTypeNameByTextureID(ETextureType ID) const
{
	for (size_t i = 0; i < ETextureType::Count; i++)
		if (i == ID)
			return cTextureTypeNames[i];
	throw CException("Unknown texture ID '%d'.", ID);
}

MaterialModel::ETextureType MaterialModel::GetTextureIDByTextureTypeName(const char * TypeName) const
{
	for (size_t i = 0; i < ETextureType::Count; i++)
		if (::stricmp(cTextureTypeNames[i], TypeName) == 0)
			return static_cast<MaterialModel::ETextureType>(i);
	throw CException("Unknown texture type name '%s'.", TypeName);
}
