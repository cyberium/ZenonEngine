#include "stdafx.h"

// General
#include "SkeletonComponentBone.h"

CSkeletonComponentBone3D::CSkeletonComponentBone3D(const std::shared_ptr<ISkeletonBone>& Bone)
	: m_ProtoBone(Bone)
	, m_CalculatedMatrix(glm::mat4(1.0f))
	, m_IsCalculated(false)
{
}

CSkeletonComponentBone3D::~CSkeletonComponentBone3D()
{
}


//
// ISkeletonComponentBone3D
//
std::string CSkeletonComponentBone3D::GetName() const
{
	return m_ProtoBone->GetName();
}

const std::weak_ptr<ISkeletonComponentBone3D>& CSkeletonComponentBone3D::GetParentBone() const
{
	return m_ParentBone;
}

const std::vector<std::shared_ptr<ISkeletonComponentBone3D>>& CSkeletonComponentBone3D::GetChilds() const
{
	return m_Childs;
}

const ISkeletonBone& CSkeletonComponentBone3D::GetProtoBone() const
{
	return *(m_ProtoBone.get());
}

const glm::mat4& CSkeletonComponentBone3D::GetCalculatedMatrix() const
{
	return m_CalculatedMatrix;
}



//
// ISkeletonBoneInternal3D
//
void CSkeletonComponentBone3D::AddChildInternal(const std::shared_ptr<ISkeletonComponentBone3D>& Child)
{
	if (std::find(m_Childs.begin(), m_Childs.end(), Child) != m_Childs.end())
		throw CException("Bone already contain this child.");
	m_Childs.push_back(Child);
}

void CSkeletonComponentBone3D::SetParentAndChildsInternals(const std::vector<std::shared_ptr<ISkeletonComponentBone3D>>& Bones)
{
	int32 parentIndex = m_ProtoBone->GetParentIndex();
	if (parentIndex != -1)
	{
		_ASSERT(m_ParentBone.lock() == nullptr);
		_ASSERT(m_ProtoBone->GetParentIndex() < Bones.size());
		auto parentBone = Bones[parentIndex];
		std::dynamic_pointer_cast<ISkeletonComponentBoneInternal3D>(parentBone)->AddChildInternal(shared_from_this());
		m_ParentBone = parentBone;
	}
}

void CSkeletonComponentBone3D::Calculate(const IModelComponent* ModelsComponent, const ICameraComponent3D* Camera)
{
	if (m_IsCalculated)
		return;

	auto parentBone = m_ParentBone.lock();
	if (parentBone)
	{
		std::dynamic_pointer_cast<ISkeletonComponentBoneInternal3D>(parentBone)->Calculate(ModelsComponent, Camera);
	}
	else
	{
		_ASSERT(m_ProtoBone->GetParentIndex() == -1);
	}

	m_CalculatedMatrix = m_ProtoBone->CalculateBontMatrix(ModelsComponent);

	if (parentBone)
		m_CalculatedMatrix = parentBone->GetCalculatedMatrix() * m_CalculatedMatrix;

	m_IsCalculated = true;
}

void CSkeletonComponentBone3D::Reset()
{
	m_CalculatedMatrix = glm::mat4(1.0f);
	m_IsCalculated = false;
}

