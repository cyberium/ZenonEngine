#include "stdafx.h"

// General
#include "SkeletonComponentBone.h"

CSkeletonComponentBone3D::CSkeletonComponentBone3D(const std::shared_ptr<ISkeletonBone>& Bone)
	: m_ProtoBone(Bone)
	, m_PivotPoint(glm::vec3(0.0f))
	, m_Matrix(glm::mat4(1.0f))
	, m_RotateMatrix(glm::mat4(1.0f))
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

glm::vec3 CSkeletonComponentBone3D::GetPivotPoint() const
{
	return m_PivotPoint;
}

const glm::mat4& CSkeletonComponentBone3D::GetMatrix() const
{
	return m_Matrix;
}

const glm::mat4& CSkeletonComponentBone3D::GetRotateMatrix() const
{
	return m_RotateMatrix;
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
		_ASSERT(m_Bone->GetParentIndex() < Bones.size());
		auto parentBone = Bones[parentIndex];
		std::dynamic_pointer_cast<ISkeletonComponentBoneInternal3D>(parentBone)->AddChildInternal(shared_from_this());
		m_ParentBone = parentBone;
	}
}

void CSkeletonComponentBone3D::Calculate(const IModelsComponent3D* ModelsComponent, const ICameraComponent3D* Camera)
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
		_ASSERT(m_Bone->GetParentIndex() == -1);
	}

	m_Matrix = m_ProtoBone->CalcMatrix(ModelsComponent);
	m_RotateMatrix = m_ProtoBone->CalcRotateMatrix(ModelsComponent);

	if (parentBone)
	{
		m_Matrix = parentBone->GetMatrix() * m_Matrix;
		//m_RotateMatrix = parentBone->GetRotateMatrix() * m_RotateMatrix;
	}

	m_IsCalculated = true;
}

void CSkeletonComponentBone3D::Reset()
{
	m_Matrix = glm::mat4(1.0f);
	m_RotateMatrix = glm::mat4(1.0f);
	m_IsCalculated = false;
}

