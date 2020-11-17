#include "stdafx.h"

// General
#include "SkeletonBone.h"

CSkeletonBone::CSkeletonBone(const std::string & Name, int32 ParentIndex)
	: Name(Name)
	, ParentIndex(ParentIndex)
	, PivotMatrix(glm::mat4(1.0f))
	, LocalTransform(glm::mat4(1.0f))
	, FuckingMatrix(glm::mat4(1.0f))
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

	m_CalculatedMatrixes.MergeWithOther(otherAsMe->m_CalculatedMatrixes);

	LocalTransform = otherAsMe->LocalTransform;
	PivotMatrix = otherAsMe->PivotMatrix;
}

std::string CSkeletonBone::GetName() const
{
	return Name;
}

int32 CSkeletonBone::GetParentIndex() const
{
	return ParentIndex;
}

void CSkeletonBone::SetLocalMatrix(const glm::mat4 & Matrix)
{
	LocalTransform = Matrix;
}

glm::mat4 CSkeletonBone::GetLocalMatrix() const
{
	return LocalTransform;
}

void CSkeletonBone::SetPivotMatrix(const glm::mat4 & Matrix)
{
	PivotMatrix = Matrix;
}

glm::mat4 CSkeletonBone::GetPivotMatrix() const
{
	return PivotMatrix;
}

void CSkeletonBone::SetFuckingMatrix(const glm::mat4 & Matrix)
{
	FuckingMatrix = Matrix;
}

glm::mat4 CSkeletonBone::GetFuckingMatrix() const
{
	return FuckingMatrix;
}

glm::mat4 CSkeletonBone::CalcMatrix(const IModelsComponent3D* ModelsComponent) const
{
	glm::mat4 m(1.0f);

	m *= GetPivotMatrix();
	{
		if (ParentIndex == -1)
			m *= glm::inverse(LocalTransform);

		if (m_CalculatedMatrixes.IsUsesBySequence(ModelsComponent->GetCurrentAnimationIndex()))
			m *= m_CalculatedMatrixes.GetValue(ModelsComponent->GetCurrentAnimationIndex(), ModelsComponent->GetCurrentTime_());
		else
			m *= LocalTransform;
	}
	m *= glm::inverse(GetPivotMatrix());

	return m;
}

glm::mat4 CSkeletonBone::CalcRotateMatrix(const IModelsComponent3D* ModelsComponent) const
{
	return FuckingMatrix;
}
