#include "stdafx.h"

// General
#include "SkeletonBone.h"

CSkeletonBone::CSkeletonBone(const std::shared_ptr<IByteBuffer>& Buffer)
{
	Load(Buffer);
}

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
	//PivotMatrix = otherAsMe->PivotMatrix;
	//FuckingMatrix = otherAsMe->FuckingMatrix;
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
			m *= m_CalculatedMatrixes.GetValue(ModelsComponent->GetCurrentAnimationIndex(), ModelsComponent->GetCurrentAnimationFrame());
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



//
// IObjectLoadSave
//
void CSkeletonBone::CopyTo(std::shared_ptr<IObject> Destination) const
{
	throw CException("Not implemented.");
}

void CSkeletonBone::Load(const std::shared_ptr<IByteBuffer>& Buffer)
{
	Buffer->readString(&Name);
	Buffer->read(&ParentIndex);
	Buffer->read(&LocalTransform);
	Buffer->read(&PivotMatrix);
	Buffer->read(&FuckingMatrix);

	m_CalculatedMatrixes.Load(Buffer);
}

void CSkeletonBone::Save(const std::shared_ptr<IByteBuffer>& Buffer) const
{
	Buffer->writeString(Name);
	Buffer->write(&ParentIndex);
	Buffer->write(&LocalTransform);
	Buffer->write(&PivotMatrix);
	Buffer->write(&FuckingMatrix);

	m_CalculatedMatrixes.Save(Buffer);
}

void CSkeletonBone::Load(const std::shared_ptr<IXMLReader>& Reader)
{
	throw CException("Not implemented.");
}

void CSkeletonBone::Save(const std::shared_ptr<IXMLWriter>& Writer) const
{
	throw CException("Not implemented.");
}
