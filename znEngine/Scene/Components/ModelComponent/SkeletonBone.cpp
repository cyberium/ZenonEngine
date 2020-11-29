#include "stdafx.h"

// General
#include "SkeletonBone.h"

CSkeletonBone::CSkeletonBone(const std::shared_ptr<IXMLReader>& Reader)
{
	Load(Reader);
}

CSkeletonBone::CSkeletonBone(const std::shared_ptr<IByteBuffer>& Buffer)
{
	Load(Buffer);
}

CSkeletonBone::CSkeletonBone(const std::string & Name, int32 ParentIndex)
	: m_Name(Name)
	, m_ParentIndex(ParentIndex)
	, m_PivotMatrix(glm::mat4(1.0f))
	, m_LocalTransform(glm::mat4(1.0f))
	, m_FuckingMatrix(glm::mat4(1.0f))
{
}

CSkeletonBone::~CSkeletonBone()
{
}



//
// ISkeletonBone
//-
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

	//m_LocalTransform = otherAsMe->m_LocalTransform;
	//PivotMatrix = otherAsMe->PivotMatrix;
	//FuckingMatrix = otherAsMe->FuckingMatrix;
}

std::string CSkeletonBone::GetName() const
{
	return m_Name;
}

int32 CSkeletonBone::GetParentIndex() const
{
	return m_ParentIndex;
}

void CSkeletonBone::SetLocalMatrix(const glm::mat4 & Matrix)
{
	m_LocalTransform = Matrix;
}

glm::mat4 CSkeletonBone::GetLocalMatrix() const
{
	return m_LocalTransform;
}

void CSkeletonBone::SetPivotMatrix(const glm::mat4 & Matrix)
{
	m_PivotMatrix = Matrix;
}

glm::mat4 CSkeletonBone::GetPivotMatrix() const
{
	return m_PivotMatrix;
}

void CSkeletonBone::SetFuckingMatrix(const glm::mat4 & Matrix)
{
	m_FuckingMatrix = Matrix;
}

glm::mat4 CSkeletonBone::GetFuckingMatrix() const
{
	return m_FuckingMatrix;
}

glm::mat4 CSkeletonBone::CalcMatrix(const IModelsComponent3D* ModelsComponent) const
{
	glm::mat4 m(1.0f);

	m *= GetPivotMatrix();
	{
		size_t currentAnimationIndex = ModelsComponent->GetCurrentAnimationIndex();

		if (m_ParentIndex == -1)
			m *= glm::inverse(ModelsComponent->GetModel()->GetSkeletonAnimation(currentAnimationIndex).RootBoneLocalTransform);

		if (m_CalculatedMatrixes.IsUsesBySequence(currentAnimationIndex))
			m *= m_CalculatedMatrixes.GetValue(ModelsComponent->GetCurrentAnimationIndex(), ModelsComponent->GetCurrentAnimationFrame());
		else
			m *= m_LocalTransform;
	}
	m *= glm::inverse(GetPivotMatrix());

	return m;
}

glm::mat4 CSkeletonBone::CalcRotateMatrix(const IModelsComponent3D* ModelsComponent) const
{
	return m_FuckingMatrix;
}



//
// IObjectLoadSave
//
std::shared_ptr<IObject> CSkeletonBone::Copy() const
{
	throw CException("Not implemented.");
}

void CSkeletonBone::CopyTo(std::shared_ptr<IObject> Destination) const
{
	throw CException("Not implemented.");
}

void CSkeletonBone::Load(const std::shared_ptr<IByteBuffer>& Buffer)
{
	Buffer->readString(&m_Name);
	Buffer->read(&m_ParentIndex);
	Buffer->read(&m_LocalTransform);
	Buffer->read(&m_PivotMatrix);
	Buffer->read(&m_FuckingMatrix);

	m_CalculatedMatrixes.Load(Buffer);
}

void CSkeletonBone::Save(const std::shared_ptr<IByteBuffer>& Buffer) const
{
	Buffer->writeString(m_Name);
	Buffer->write(&m_ParentIndex);
	Buffer->write(&m_LocalTransform);
	Buffer->write(&m_PivotMatrix);
	Buffer->write(&m_FuckingMatrix);
	m_CalculatedMatrixes.Save(Buffer);
}

void CSkeletonBone::Load(const std::shared_ptr<IXMLReader>& Reader)
{
	m_Name = Reader->GetStrAttribute("Name");
	m_ParentIndex = Reader->GetIntAttribute("ParentIndex");
	m_LocalTransform = Utils::StringToMatrix(Reader->GetStrAttribute("LocalTransform"));
	m_PivotMatrix = Utils::StringToMatrix(Reader->GetStrAttribute("PivotMatrix"));
	m_FuckingMatrix = Utils::StringToMatrix(Reader->GetStrAttribute("FuckingMatrix"));

	std::shared_ptr<CByteBuffer> byteBuffer = MakeShared(CByteBuffer, Utils::Base64_Decode(Reader->GetValue()));
	m_CalculatedMatrixes.Load(byteBuffer);
}

void CSkeletonBone::Save(const std::shared_ptr<IXMLWriter>& Writer) const
{
	Writer->SetStrAttribute(m_Name, "Name");
	Writer->SetIntAttribute(m_ParentIndex, "ParentIndex");
	Writer->SetStrAttribute(Utils::MatrixToString(m_LocalTransform), "LocalTransform");
	Writer->SetStrAttribute(Utils::MatrixToString(m_PivotMatrix), "PivotMatrix");
	Writer->SetStrAttribute(Utils::MatrixToString(m_FuckingMatrix), "FuckingMatrix");

	std::shared_ptr<CByteBuffer> byteBuffer = MakeShared(CByteBuffer);
	m_CalculatedMatrixes.Save(byteBuffer);
	Writer->SetValue(Utils::Base64_Encode(byteBuffer->getData(), byteBuffer->getSize()));

}



//
// Private
//
bool CSkeletonBone::IsRootBone() const
{
	return m_ParentIndex == -1;
}
