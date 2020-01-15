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
	ELightType lightTypes[] = { ELightType::Point, ELightType::Directional, ELightType::Spot, ELightType::Unknown, ELightType::Unknown };

	DisplayString("    Type: ", lLightTypes[NativeLight->LightType.Get()]);
	DisplayBool("    Cast Light: ", NativeLight->CastLight.Get());

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


	m_OwnerFBXNode.lock()->GetComponent<ILightComponent3D>()->SetType(lightTypes[NativeLight->LightType.Get()]);
	m_OwnerFBXNode.lock()->GetComponent<ILightComponent3D>()->SetColor(glm::vec3(NativeLight->Color.Get()[0], NativeLight->Color.Get()[1], NativeLight->Color.Get()[2]));
	m_OwnerFBXNode.lock()->GetComponent<ILightComponent3D>()->SetSpotlightAngle(NativeLight->OuterAngle.Get());
	m_OwnerFBXNode.lock()->GetComponent<ILightComponent3D>()->SetRange(NativeLight->Intensity.Get() / 50.0f);
}
