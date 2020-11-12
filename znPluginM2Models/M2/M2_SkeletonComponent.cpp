#include "stdafx.h"

// Include
#include "M2.h"
#include "M2_Base_Instance.h"

// General
#include "M2_SkeletonComponent.h"

CM2SkeletonBone3D::CM2SkeletonBone3D(const SM2_Part_Bone_Wrapper& M2Bone)
	: m_M2Bone(M2Bone)
	, m_PivotPoint(glm::vec3(0.0f))
	, m_Matrix(glm::mat4(1.0f))
	, m_RotateMatrix(glm::mat4(1.0f))
	, m_IsCalculated(false)
{
}

CM2SkeletonBone3D::~CM2SkeletonBone3D()
{
}



//
// ISkeletonComponentBone3D
//
std::string CM2SkeletonBone3D::GetName() const
{
	return std::string(m_M2Bone.GetGameBoneName());
}

const std::weak_ptr<ISkeletonComponentBone3D>& CM2SkeletonBone3D::GetParentBone() const
{
	return m_ParentBone;
}

const std::vector<std::shared_ptr<ISkeletonComponentBone3D>>& CM2SkeletonBone3D::GetChilds() const
{
	return m_Childs;
}

glm::vec3 CM2SkeletonBone3D::GetPivotPoint() const
{
	return m_M2Bone.getPivot();
}

const glm::mat4& CM2SkeletonBone3D::GetMatrix() const
{
	return m_Matrix;
}

const glm::mat4& CM2SkeletonBone3D::GetRotateMatrix() const
{
	return m_RotateMatrix;
}



//
// ISkeletonComponentBoneInternal3D
//
void CM2SkeletonBone3D::AddChildInternal(const std::shared_ptr<ISkeletonComponentBone3D>& Child)
{
}

void CM2SkeletonBone3D::SetParentAndChildsInternals(const std::vector<std::shared_ptr<ISkeletonComponentBone3D>>& Bones)
{
	if (m_M2Bone.getParentBoneID() != -1)
	{
		_ASSERT(m_M2Bone.getParentBoneID() < Bones.size());
		auto parentBone = Bones[m_M2Bone.getParentBoneID()];
		std::dynamic_pointer_cast<ISkeletonComponentBoneInternal3D>(parentBone)->AddChildInternal(shared_from_this());
		m_ParentBone = parentBone;
	}
}

void CM2SkeletonBone3D::Calculate(const ISceneNode& Instance, const ICameraComponent3D* Camera)
{
	if (m_IsCalculated)
		return;

	auto parentBone = m_ParentBone.lock();
	if (parentBone)
		std::dynamic_pointer_cast<ISkeletonComponentBoneInternal3D>(parentBone)->Calculate(Instance, Camera);

	m_Matrix = m_M2Bone.calcMatrix(dynamic_cast<const CM2_Base_Instance&>(Instance), Instance.GetScene().GetBaseManager().GetApplication().GetGameTime());
	m_RotateMatrix = m_M2Bone.calcRotationMatrix(dynamic_cast<const CM2_Base_Instance&>(Instance), Instance.GetScene().GetBaseManager().GetApplication().GetGameTime());

	if (parentBone)
	{
		m_Matrix = parentBone->GetMatrix() * m_Matrix;
		m_RotateMatrix = parentBone->GetRotateMatrix() * m_RotateMatrix;
	}

	if (m_M2Bone.IsBillboard())
	{
		m_Matrix = m_Matrix * m_M2Bone.calcBillboardMatrix(m_Matrix, dynamic_cast<const CM2_Base_Instance&>(Instance), Camera);
	}

	m_IsCalculated = true;
}

void CM2SkeletonBone3D::Reset()
{
	m_IsCalculated = false;
}



//
// CM2SkeletonComponent3D
//
CM2SkeletonComponent3D::CM2SkeletonComponent3D(const CM2_Base_Instance& OwnerNode)
	: CComponentBase(OwnerNode)
{
	for (const auto& m2Bone : OwnerNode.getM2().getSkeleton().GetBones())
		m_Bones.push_back(std::make_shared<CM2SkeletonBone3D>(m2Bone));

	for (const auto& bone : m_Bones)
		std::dynamic_pointer_cast<ISkeletonComponentBoneInternal3D>(bone)->SetParentAndChildsInternals(m_Bones);
}

CM2SkeletonComponent3D::~CM2SkeletonComponent3D()
{
}



//
// CComponentBase
//
void CM2SkeletonComponent3D::Update(const UpdateEventArgs & e)
{
	for (const auto& b : m_Bones)
		std::dynamic_pointer_cast<ISkeletonComponentBoneInternal3D>(b)->Reset();

	for (const auto& b : m_Bones)
		std::dynamic_pointer_cast<ISkeletonComponentBoneInternal3D>(b)->Calculate(GetM2OwnerNode(), e.CameraForCulling);

	// TODO: Fix me
	const_cast<CM2_Base_Instance&>(GetM2OwnerNode()).UpdateAttachPositionAfterSkeletonUpdate();
}



//
// ISkeletonComponent3D
//
std::shared_ptr<ISkeletonComponentBone3D> CM2SkeletonComponent3D::GetBone(size_t Index) const
{
	_ASSERT(Index < m_Bones.size());
	return m_Bones.at(Index);
}

const std::vector<std::shared_ptr<ISkeletonComponentBone3D>>& CM2SkeletonComponent3D::GetBones() const
{
	return m_Bones;
}

std::shared_ptr<IStructuredBuffer> CM2SkeletonComponent3D::GetBonesBuffer() const
{
	return std::shared_ptr<IStructuredBuffer>();
}


std::vector<glm::mat4> CM2SkeletonComponent3D::CreatePose(size_t BoneStartIndex, size_t BonesCount) const
{
	std::vector<glm::mat4> result;
	result.reserve(BonesCount);
	for (size_t i = BoneStartIndex; i < BoneStartIndex + BonesCount; i++)
		result.push_back(m_Bones[GetM2OwnerNode().getM2().getSkeleton().getBoneLookupIndex(i)]->GetMatrix());
	return result;
}



//
// Protected
//
const CM2_Base_Instance& CM2SkeletonComponent3D::GetM2OwnerNode() const
{
	return dynamic_cast<const CM2_Base_Instance&>(GetOwnerNode());
}
