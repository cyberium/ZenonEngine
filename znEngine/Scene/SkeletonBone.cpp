#include "stdafx.h"

// General
#include "SkeletonBone.h"

CSkeletonBone::CSkeletonBone(const std::string & Name, int32 ParentIndex)
	: Name(Name)
	, ParentIndex(ParentIndex)
	, GlobalTransform(glm::mat4(1.0f))
	, LocalTransform(glm::mat4(1.0f))
{
}

CSkeletonBone::~CSkeletonBone()
{
}

void CSkeletonBone::MergeWithOther(const CSkeletonBone & other)
{
	pX.MergeWithOther(other.pX);
	pY.MergeWithOther(other.pY);
	pZ.MergeWithOther(other.pZ);

	rX.MergeWithOther(other.rX);
	rY.MergeWithOther(other.rY);
	rZ.MergeWithOther(other.rZ);

	sX.MergeWithOther(other.sX);
	sY.MergeWithOther(other.sY);
	sZ.MergeWithOther(other.sZ);

	mM.MergeWithOther(other.mM);

	GlobalTransform = other.GlobalTransform;
	LocalTransform = other.LocalTransform;
}

bool CSkeletonBone::operator==(const CSkeletonBone & other) const
{
	return Name == other.Name && ParentIndex == other.ParentIndex;
}



glm::mat4 CSkeletonBone::CalcMatrix(const ISceneNode3D& Instance) const
{
	glm::mat4 m(1.0f);
	if (const auto& animator = Instance.GetComponent<ISkeletonAnimationComponent>())
	{
		//m *= glm::inverse(GlobalTransform);

		//m *= glm::translate(CalcTranslate(Instance));


		
		//m *= CalcRotate(Instance);
		
		/*if (sX.IsUsesBySequence(animator->getSequenceIndex()))
		{
			glm::vec3 s = glm::vec3(
				sX.GetValue(animator->getSequenceIndex(), animator->getCurrentTime()),
				sY.GetValue(animator->getSequenceIndex(), animator->getCurrentTime()),
				sZ.GetValue(animator->getSequenceIndex(), animator->getCurrentTime())
			);
			m = glm::scale(m, s);
		}*/
		
		//m *= glm::inverse(LocalTransform);

		/*if (mM.IsUsesBySequence(animator->getSequenceIndex()))
		{
			m *= mM.GetValue(animator->getSequenceIndex(), animator->getCurrentTime());
		}*/

		m *= LocalTransform;
	}
	

	return m;
}


glm::vec3 CSkeletonBone::CalcTranslate(const ISceneNode3D & Instance) const
{
	glm::vec3 p(0.0f);
	if (const auto& animator = Instance.GetComponent<ISkeletonAnimationComponent>())
	{
		if (pX.IsUsesBySequence(animator->getSequenceIndex()))
		{
			p = glm::vec3(
				pX.GetValue(animator->getSequenceIndex(), animator->getCurrentTime()),
				pY.GetValue(animator->getSequenceIndex(), animator->getCurrentTime()),
				pZ.GetValue(animator->getSequenceIndex(), animator->getCurrentTime())
			);
		}
	}
	return p;
}

glm::mat4 CSkeletonBone::CalcRotate(const ISceneNode3D & Instance) const
{
	glm::mat4 m(1.0f);
	if (const auto& animator = Instance.GetComponent<ISkeletonAnimationComponent>())
	{
		if (rX.IsUsesBySequence(animator->getSequenceIndex()))
		{
			float r = glm::radians(rX.GetValue(animator->getSequenceIndex(), animator->getCurrentTime()));
			glm::mat4 rot = glm::rotate(r, glm::vec3(1, 0, 0));
			m *= rot;
		}

		if (rY.IsUsesBySequence(animator->getSequenceIndex()))
		{
			float r = glm::radians(rY.GetValue(animator->getSequenceIndex(), animator->getCurrentTime()));
			glm::mat4 rot = glm::rotate(r, glm::vec3(0, 1, 0));
			m *= rot;
		}

		if (rZ.IsUsesBySequence(animator->getSequenceIndex()))
		{
			float r = glm::radians(rZ.GetValue(animator->getSequenceIndex(), animator->getCurrentTime()));
			glm::mat4 rot = glm::rotate(r, glm::vec3(0, 0, 1));
			m *= rot;
		}
	}
	return m;
}
