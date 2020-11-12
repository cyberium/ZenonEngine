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
	LocalTransform222 = other.LocalTransform222;
}

bool CSkeletonBone::operator==(const CSkeletonBone & other) const
{
	return Name == other.Name && ParentIndex == other.ParentIndex;
}



glm::mat4 CSkeletonBone::CalcMatrix(const ISceneNode& Instance) const
{
	glm::mat4 m(1.0f);
	if (const auto& animator = Instance.GetComponentT<ISkeletonAnimationComponent>())
	{
		//m *= glm::inverse(GlobalTransform);

		//m = LocalTransform;

		//glm::vec3 tr = glm::vec3(LocalTransform[3][0], LocalTransform[3][1], LocalTransform[3][2]);

		//m = glm::translate(m, tr);


		//if (ParentIndex == -1)
		//{
		//	glm::mat4 newMatrix = glm::inverse(LocalTransform) * glm::translate(CalcTranslate(Instance));
		//	m = m * newMatrix;
		//}
		//else
		m = m * glm::translate(CalcTranslate(Instance));





		//if (ParentIndex == -1)
		//{
		//	glm::mat4 newMatrix = glm::inverse(glm::toMat4(glm::quat(LocalTransform))) * CalcRotate(Instance);
		//	m = m * newMatrix;
		//}
		//else
		//glm::vec3 orig = glm::radians(CalcRotate(Instance));
		//m = m * glm::toMat4(glm::quat(orig));


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
		
		//m = glm::translate(m, tr);

		
			//if (mM.IsUsesBySequence(animator->getSequenceIndex()))
			//	m = m * mM.GetValue(animator->getSequenceIndex(), animator->getCurrentTime());
		m = LocalTransform;
	}
	

	return m;
}


glm::vec3 CSkeletonBone::CalcTranslate(const ISceneNode & Instance) const
{
	glm::vec3 p(0.0f);
	if (const auto& animator = Instance.GetComponentT<ISkeletonAnimationComponent>())
	{
		if (pX.IsUsesBySequence(animator->getSequenceIndex()))
			p.x = pX.GetValue(animator->getSequenceIndex(), animator->getCurrentTime());

		if (pY.IsUsesBySequence(animator->getSequenceIndex()))
			p.y = pY.GetValue(animator->getSequenceIndex(), animator->getCurrentTime());

		if (pZ.IsUsesBySequence(animator->getSequenceIndex()))
			p.z = pZ.GetValue(animator->getSequenceIndex(), animator->getCurrentTime());
	}
	return p;
}

glm::vec3 CSkeletonBone::CalcRotate(const ISceneNode & Instance) const
{
	glm::vec3 p(0.0f);
	if (const auto& animator = Instance.GetComponentT<ISkeletonAnimationComponent>())
	{
		if (rX.IsUsesBySequence(animator->getSequenceIndex()))
			p.x = rX.GetValue(animator->getSequenceIndex(), animator->getCurrentTime());

		if (rY.IsUsesBySequence(animator->getSequenceIndex()))
			p.y = rY.GetValue(animator->getSequenceIndex(), animator->getCurrentTime());

		if (rZ.IsUsesBySequence(animator->getSequenceIndex()))
			p.z = rZ.GetValue(animator->getSequenceIndex(), animator->getCurrentTime());
	}
	return p;
}
