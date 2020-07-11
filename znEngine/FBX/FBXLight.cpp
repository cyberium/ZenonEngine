#include "stdafx.h"

#ifdef ZN_FBX_SDK_ENABLE

// Include
#include "FBXScene.h"
#include "FBXSceneNode.h"

// General
#include "FBXLight.h"

// Additional
#include "FBXDisplayCommon.h"

CFBXLight::CFBXLight(const IBaseManager& BaseManager, std::weak_ptr<CFBXSceneNode> OwnerFBXNode)
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

	/*fbxsdk::FbxDouble3 lTranslation = m_OwnerFBXNode.lock()->GetNativeNode()->LclTranslation.Get();
	Display4DVector("Translation: ", lTranslation, "");
	m_OwnerFBXNode.lock()->SetTranslate(glm::vec3(lTranslation[0], lTranslation[1], lTranslation[2]));

	fbxsdk::FbxDouble3 lRotation = m_OwnerFBXNode.lock()->GetNativeNode()->LclRotation.Get();
	glm::vec3 rotationVec = glm::vec3();
	rotationVec.x = glm::cos(glm::radians(lRotation[0])) * glm::cos(glm::radians(lRotation[1]));
	rotationVec.y = glm::sin(glm::radians(lRotation[0])) * glm::cos(glm::radians(lRotation[1]));
	rotationVec.z = glm::sin(glm::radians(lRotation[1]));

	Display4DVector("Rotation: ", lRotation, "");
	m_OwnerFBXNode.lock()->SetRotation(rotationVec);

	fbxsdk::FbxDouble3 lScale = m_OwnerFBXNode.lock()->GetNativeNode()->LclScaling.Get();
	Display4DVector("Scale: ", lScale, "");
	m_OwnerFBXNode.lock()->SetScale(glm::vec3(lScale[0], lScale[1], lScale[2]));*/

	m_OwnerFBXNode.lock()->ISceneNode3D::GetComponent<ILightComponent3D>()->SetType(lightTypes[NativeLight->LightType.Get()]);
	m_OwnerFBXNode.lock()->ISceneNode3D::GetComponent<ILightComponent3D>()->SetColor(glm::vec3(NativeLight->Color.Get()[0], NativeLight->Color.Get()[1], NativeLight->Color.Get()[2]));
	m_OwnerFBXNode.lock()->ISceneNode3D::GetComponent<ILightComponent3D>()->SetSpotlightAngle(NativeLight->OuterAngle.Get() / 2.0f);
	m_OwnerFBXNode.lock()->ISceneNode3D::GetComponent<ILightComponent3D>()->SetRange(NativeLight->Intensity.Get() / 40.0f);
}

#endif
