#include "stdafx.h"

// Include
#include "M2.h"
#include "M2_Base_Instance.h"

// General
#include "M2_Part_Light.h"

SM2_Part_Light_Wrapper::SM2_Part_Light_Wrapper(const CM2& M2Object, const std::shared_ptr<IFile>& File, const SM2_Light& M2Light) 
	: m_M2Object(M2Object)
	, m_M2Light(M2Light)
{
	ambColor.Initialize(M2Light.ambient_color, File, M2Object.getSkeleton().GetAnimFiles());
	ambIntensity.Initialize(M2Light.ambient_intensity, File, M2Object.getSkeleton().GetAnimFiles());

	diffColor.Initialize(M2Light.diffuse_color, File, M2Object.getSkeleton().GetAnimFiles());
	diffIntensity.Initialize(M2Light.diffuse_intensity, File, M2Object.getSkeleton().GetAnimFiles());

	attenuation_start.Initialize(M2Light.attenuation_start, File, M2Object.getSkeleton().GetAnimFiles());
	attenuation_end.Initialize(M2Light.attenuation_end, File, M2Object.getSkeleton().GetAnimFiles());

	visibility.Initialize(M2Light.visibility, File, M2Object.getSkeleton().GetAnimFiles());
}

SM2_Part_Light_Wrapper::~SM2_Part_Light_Wrapper()
{
}

SLight SM2_Part_Light_Wrapper::GetLight(const CM2_Base_Instance * M2Instance, uint32 globalTime) const
{
	SLight lightStruct;
	if (m_M2Light.type == SM2_Light::Type::Directional)
	{
		// TODO: rotation used world pos of node + rotate_bone * pos
	}
	else if (m_M2Light.type == SM2_Light::Type::Point)
	{
		// TODO: position used world pos of node + transform_bone * pos
	}
	else
	{
		_ASSERT(false);
	}

	//glm::vec4 ambient = glm::vec4(1.0f, 1.0f, 1.0f, 1.0f);
	glm::vec4 diffuse = glm::vec4(1.0f, 1.0f, 1.0f, 1.0f);
	float attenStart = 1.0f, attenEnd = 150.0f;

	if (const auto& animator = M2Instance->GetModelsComponent())
	{
		//if (ambColor.IsUsesBySequence(animator->GetCurrentAnimationIndex()))
		//	ambient.rgb = ambColor.GetValue(animator->GetCurrentAnimationIndex(), animator->getCurrentTime(), m_M2Object.getSkeleton().getGlobalLoops(), globalTime);
		//if (ambIntensity.IsUsesBySequence(animator->GetCurrentAnimationIndex()))
		//	ambient.a = ambIntensity.GetValue(animator->GetCurrentAnimationIndex(), animator->getCurrentTime(), m_M2Object.getSkeleton().getGlobalLoops(), globalTime);
		
		if (diffColor.IsUsesBySequence(animator->GetCurrentAnimationIndex()))
			lightStruct.Color.rgb = diffColor.GetValue(animator->GetCurrentAnimationIndex(), animator->GetCurrentTime_(), m_M2Object.getSkeleton().getGlobalLoops(), globalTime);
		if (diffIntensity.IsUsesBySequence(animator->GetCurrentAnimationIndex()))
			lightStruct.Intensity = diffIntensity.GetValue(animator->GetCurrentAnimationIndex(), animator->GetCurrentTime_(), m_M2Object.getSkeleton().getGlobalLoops(), globalTime);

		if (attenuation_start.IsUsesBySequence(animator->GetCurrentAnimationIndex()))
			attenStart = attenuation_start.GetValue(animator->GetCurrentAnimationIndex(), animator->GetCurrentTime_(), m_M2Object.getSkeleton().getGlobalLoops(), globalTime);
		if (attenuation_end.IsUsesBySequence(animator->GetCurrentAnimationIndex()))
			attenEnd = attenuation_end.GetValue(animator->GetCurrentAnimationIndex(), animator->GetCurrentTime_(), m_M2Object.getSkeleton().getGlobalLoops(), globalTime);
		lightStruct.Range = attenEnd;
	}

	return lightStruct;
}
