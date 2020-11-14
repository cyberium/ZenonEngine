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



//
// ISkeletonBone
//
void CSkeletonBone::MergeWithOther(std::shared_ptr<ISkeletonBone> OtherBone)
{
	auto otherAsMe = std::static_pointer_cast<CSkeletonBone>(OtherBone);

	pX.MergeWithOther(otherAsMe->pX);
	pY.MergeWithOther(otherAsMe->pY);
	pZ.MergeWithOther(otherAsMe->pZ);

	rX.MergeWithOther(otherAsMe->rX);
	rY.MergeWithOther(otherAsMe->rY);
	rZ.MergeWithOther(otherAsMe->rZ);

	sX.MergeWithOther(otherAsMe->sX);
	sY.MergeWithOther(otherAsMe->sY);
	sZ.MergeWithOther(otherAsMe->sZ);

	GlobalTransform = otherAsMe->GlobalTransform;
	LocalTransform = otherAsMe->LocalTransform;
}

std::string CSkeletonBone::GetName() const
{
	return Name;
}

glm::vec3 CSkeletonBone::GetPivotPoint() const
{
	return glm::vec3(0.0f);
}

int32 CSkeletonBone::GetParentIndex() const
{
	return ParentIndex;
}

glm::mat4 CSkeletonBone::CalcMatrix(const IModelsComponent3D* ModelsComponent) const
{
#if 0
	glm::mat4 m(1.0f);
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

	{
		glm::vec3 translate(0.0f);
		if (pX.IsUsesBySequence(ModelsComponent->GetCurrentAnimationIndex()))
			translate.x = pX.GetValue(ModelsComponent->GetCurrentAnimationIndex(), ModelsComponent->GetCurrentTime_());

		if (pY.IsUsesBySequence(ModelsComponent->GetCurrentAnimationIndex()))
			translate.y = pY.GetValue(ModelsComponent->GetCurrentAnimationIndex(), ModelsComponent->GetCurrentTime_());

		if (pZ.IsUsesBySequence(ModelsComponent->GetCurrentAnimationIndex()))
			translate.z = pZ.GetValue(ModelsComponent->GetCurrentAnimationIndex(), ModelsComponent->GetCurrentTime_());

		m = m * glm::translate(translate);
	}


	{
		glm::vec3 rotate(0.0f);
		if (rX.IsUsesBySequence(ModelsComponent->GetCurrentAnimationIndex()))
			rotate.x = rX.GetValue(ModelsComponent->GetCurrentAnimationIndex(), ModelsComponent->GetCurrentTime_());

		if (rY.IsUsesBySequence(ModelsComponent->GetCurrentAnimationIndex()))
			rotate.y = rY.GetValue(ModelsComponent->GetCurrentAnimationIndex(), ModelsComponent->GetCurrentTime_());

		if (rZ.IsUsesBySequence(ModelsComponent->GetCurrentAnimationIndex()))
			rotate.z = rZ.GetValue(ModelsComponent->GetCurrentAnimationIndex(), ModelsComponent->GetCurrentTime_());
	}

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



	return m;
#endif

	throw CException("Not implemented!");
}

glm::mat4 CSkeletonBone::CalcRotateMatrix(const IModelsComponent3D* ModelsComponent) const
{
	return glm::mat4();
}
