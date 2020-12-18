#include "stdafx.h"

#ifdef ZN_FBX_SDK_ENABLE

// General
#include "FBXLight.h"

// Additional
#include "FBXDisplayCommon.h"

CFBXLight::CFBXLight(const IBaseManager& BaseManager, const IFBXNode& FBXNode)
	: m_BaseManager(BaseManager)
	, m_FBXNode(FBXNode)
{
	m_Light = (SGPULight*)_aligned_malloc(sizeof(SGPULight), 16);
	*m_Light = SGPULight();
}

CFBXLight::~CFBXLight()
{
	_aligned_free(m_Light);
}

void CFBXLight::Load(fbxsdk::FbxLight * NativeLight)
{
	DisplayString("Light Name: ", (char *)NativeLight->GetName());

	const char* lLightTypes[] = { "Point", "Directional", "Spot", "Area", "Volume" };
	ELightType lightTypes[] = { ELightType::Point, ELightType::Directional, ELightType::Spot/*, ELightType::Unknown, ELightType::Unknown*/ };

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

	m_Light->Type = lightTypes[NativeLight->LightType.Get()];
	m_Light->Color = ColorRGBA(NativeLight->Color.Get()[0], NativeLight->Color.Get()[1], NativeLight->Color.Get()[2], 1.0f);
	m_Light->SpotlightAngle = NativeLight->OuterAngle.Get() / 2.0f;
	m_Light->Range = NativeLight->Intensity.Get() / 40.0f;
}

const SGPULight& CFBXLight::GetLight() const
{
	return *m_Light;
}

#endif
