#include "stdafx.h"

// General
#include "SkeletonBone.h"

CSkeletonBone::CSkeletonBone(const std::string & Name, int32 ParentIndex)
	: Name(Name)
	, ParentIndex(ParentIndex)
{
}

CSkeletonBone::~CSkeletonBone()
{
}

glm::mat4 CSkeletonBone::CalcMatrix(const ISceneNode3D& Instance) const
{
	glm::mat4 m(1.0f);
	if (const auto& animator = Instance.GetComponent<ISkeletonAnimationComponent>())
	{
		//if (pX.IsUsesBySequence(animator->getSequenceIndex()))
		//{
		glm::vec3 p = glm::vec3(
			pX.GetValue(animator->getSequenceIndex(), animator->getCurrentTime()),
			pY.GetValue(animator->getSequenceIndex(), animator->getCurrentTime()),
			pZ.GetValue(animator->getSequenceIndex(), animator->getCurrentTime())
		);
		m = glm::translate(m, p);
		//}

		//if (m_RotateAnimated.IsUsesBySequence(animator->getSequenceIndex()))
		//{
		glm::vec3 r = glm::vec3(
			rX.GetValue(animator->getSequenceIndex(), animator->getCurrentTime()),
			rY.GetValue(animator->getSequenceIndex(), animator->getCurrentTime()),
			rZ.GetValue(animator->getSequenceIndex(), animator->getCurrentTime())
		);
		glm::mat4 rot(1.0f);
		rot = glm::rotate(rot, r.x, glm::vec3(1, 0, 0));
		rot = glm::rotate(rot, r.y, glm::vec3(0, 1, 0));
		rot = glm::rotate(rot, r.z, glm::vec3(0, 0, 1));

		m *= rot;
		//}

		//if (m_ScaleAnimated.IsUsesBySequence(animator->getSequenceIndex()))
		//{
		glm::vec3 s = glm::vec3(
			sX.GetValue(animator->getSequenceIndex(), animator->getCurrentTime()),
			sY.GetValue(animator->getSequenceIndex(), animator->getCurrentTime()),
			sZ.GetValue(animator->getSequenceIndex(), animator->getCurrentTime())
		);
		m = glm::scale(m, s);
		//}
	}

	return m;
}