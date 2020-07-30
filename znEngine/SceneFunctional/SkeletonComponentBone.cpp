#include "stdafx.h"

// General
#include "SkeletonComponent.h"

CSkeletonComponentBone3D::CSkeletonComponentBone3D(const CSkeletonBone& Bone)
	: m_Bone(Bone)
	, m_Matrix(glm::mat4(1.0f))
	, m_RotateMatrix(glm::mat4(1.0f))
	, m_IsCalculated(false)
{
}

CSkeletonComponentBone3D::~CSkeletonComponentBone3D()
{
}

std::string CSkeletonComponentBone3D::GetName() const
{
	return m_Bone.Name;
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
	return glm::vec3(0.0f);
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
	if (m_Bone.ParentIndex != -1 && m_ParentBone.lock() == nullptr)
	{
		_ASSERT(m_Bone.ParentIndex < Bones.size());
		auto parentBone = Bones[m_Bone.ParentIndex];
		std::dynamic_pointer_cast<ISkeletonComponentBoneInternal3D>(parentBone)->AddChildInternal(shared_from_this());
		m_ParentBone = parentBone;
	}
}

void CSkeletonComponentBone3D::Calculate(const ISceneNode3D& Instance, const ICameraComponent3D* Camera)
{
	if (m_IsCalculated)
		return;

	auto parentBone = m_ParentBone.lock();
	if (parentBone)
		std::dynamic_pointer_cast<ISkeletonComponentBoneInternal3D>(parentBone)->Calculate(Instance, Camera);

	m_Matrix = m_Bone.CalcMatrix(Instance);
	//m_RotateMatrix = m_Bone.calcRotationMatrix(Instance, GlobalTime);

	if (parentBone)
	{
		m_Matrix = parentBone->GetMatrix() * m_Matrix;
		//m_RotateMatrix = parentBone->GetRotateMatrix() * m_RotateMatrix;
	}

	//if (m_Bone.IsBillboard())
	//	m_Matrix = m_Matrix * m_M2Bone.calcBillboardMatrix(m_Matrix, Instance, Camera);

	m_IsCalculated = true;
}

void CSkeletonComponentBone3D::Reset()
{
	m_IsCalculated = false;
}

