#include "stdafx.h"

// Include
#include "FBXScene.h"
#include "FBXSceneNode.h"

// General
#include "FBXLight.h"

// Additional
#include "FBXDisplayCommon.h"

CFBXLight::CFBXLight(const IBaseManager * BaseManager, std::weak_ptr<CFBXSceneNode> OwnerFBXNode)
	: m_BaseManager(BaseManager)
	, m_OwnerFBXNode(OwnerFBXNode)
{
}

CFBXLight::~CFBXLight()
{
}

void CFBXLight::Load(fbxsdk::FbxLight * NativeLight)
{
	DisplayString("Light Name: ", (char *)NativeLight->GetName());
	DisplayMetaDataConnections(NativeLight);

	const char* lLightTypes[] = { "Point", "Directional", "Spot", "Area", "Volume" };
	Light::LightType lightTypes[] = { Light::LightType::Point, Light::LightType::Directional, Light::LightType::Spot, Light::LightType::Unknown, Light::LightType::Unknown };

	DisplayString("    Type: ", lLightTypes[NativeLight->LightType.Get()]);
	DisplayBool("    Cast Light: ", NativeLight->CastLight.Get());

	fbxsdk::FbxDouble3 lTranslation = m_OwnerFBXNode.lock()->GetNativeNode()->LclTranslation.Get();
	Display4DVector("Translation: ", lTranslation, "");

	fbxsdk::FbxDouble3 lRotation = m_OwnerFBXNode.lock()->GetNativeNode()->LclRotation.Get();
	Display4DVector("Rotation: ", lRotation, "");


	DisplayString("        File Name: \"", NativeLight->FileName.Get().Buffer(), "\"");
	DisplayBool("        Ground Projection: ", NativeLight->DrawGroundProjection.Get());
	DisplayBool("        Volumetric Projection: ", NativeLight->DrawVolumetricLight.Get());
	DisplayBool("        Front Volumetric Projection: ", NativeLight->DrawFrontFacingVolumetricLight.Get());


	FbxDouble3 c = NativeLight->Color.Get();
	FbxColor lColor(c[0], c[1], c[2]);
	DisplayColor("        Default Color: ", lColor);
	DisplayDouble("        Default Intensity: ", NativeLight->Intensity.Get());
	DisplayDouble("        Default Inner Angle: ", NativeLight->InnerAngle.Get());
	DisplayDouble("        Default Outer Angle: ", NativeLight->OuterAngle.Get());
	DisplayDouble("        Default Fog: ", NativeLight->Fog.Get());
	DisplayBool("        EnableNearAttenuation: ", NativeLight->EnableNearAttenuation.Get());
	DisplayDouble("        NearAttenuationStart: ", NativeLight->NearAttenuationStart.Get());
	DisplayDouble("        NearAttenuationEnd: ", NativeLight->NearAttenuationEnd.Get());
	DisplayBool("        EnableFarAttenuation: ", NativeLight->EnableFarAttenuation.Get());
	DisplayDouble("        FarAttenuationStart: ", NativeLight->FarAttenuationStart.Get());
	DisplayDouble("        FarAttenuationEnd: ", NativeLight->FarAttenuationEnd.Get());
	DisplayString("----------------------------------------------------------------------");

	Light light;
	light.m_PositionWS = glm::vec4(lTranslation[0], lTranslation[1], lTranslation[2], 1.0f);
	light.m_DirectionWS = glm::normalize(glm::vec4(lRotation[0], lRotation[1], lRotation[2], 0.0f));
	light.m_Type = lightTypes[NativeLight->LightType.Get()];
	light.m_Color = glm::vec4(NativeLight->Color.Get()[0], NativeLight->Color.Get()[1], NativeLight->Color.Get()[2], 1.0f);
	light.m_SpotlightAngle = NativeLight->OuterAngle.Get() ;
	light.m_Range = NativeLight->Intensity.Get() / 50.0f;

	setLight(light);
}
