#include "stdafx.h"

// General
#include "ModelComponent.h"

CModelComponent::CModelComponent(const ISceneNode& OwnerNode)
    : CComponentBase(OwnerNode)
	
	// Model
	, m_IsCastShadows(true)

	// Bones

	// Animations
	, m_IsAnimationLooped(false)
	, m_IsAnimationPaused(false)
	, m_CurrentAnimation(nullptr)
	, m_CurrentAnimationIndex(0)
	, m_CurrentTime(0.0)
{
	GetProperties()->SetName("ModelsComponent"); // TODO: ObjectClassName

	{
		auto minBounds = MakeShared(CPropertyWrapped<bool>, "CastShadows", "Is this model can cast shadows", true);
		minBounds->SetValueSetter(std::bind(&CModelComponent::SetCastShadows, this, std::placeholders::_1));
		minBounds->SetValueGetter(std::bind(&CModelComponent::IsCastShadows, this));
		GetProperties()->AddProperty(minBounds);
	}
}

CModelComponent::~CModelComponent()
{}



//
// IModelComponent
//
void CModelComponent::SetModel(const std::shared_ptr<IModel>& Model)
{
	if (Model == nullptr)
		throw CException("Nullptr model can't be assigned to node '%s'.", GetOwnerNode().GetName().c_str());

	if (m_Model != nullptr)
	{
		Log::Warn("Node '%s' already has model '%s'. Do you really want to replace it with '%s'?", GetOwnerNode().GetName().c_str(), m_Model->GetFileName().c_str(), Model->GetFileName().c_str());
		ResetModel();
	}

	auto modelBounds = Model->GetBounds();
	if (modelBounds.IsInfinite())
	{
		modelBounds = BoundingBox(glm::vec3(-25.0f), glm::vec3(25.0f));
		std::dynamic_pointer_cast<IModelInternal>(Model)->SetBounds(modelBounds);
	//	throw CException("Model '%s' with inifinity bounds can't be assigned to node '%s'.", Model->GetFileName().c_str(), GetOwnerNode().GetName().c_str());
	}

	m_Model = Model;
	InitializeBones();

	/*if (auto rootBone = GetModel()->GetRootBone())
	{
		modelBounds.transform(rootBone->GetLocalMatrix());	
	}*/

	GetComponentT<IColliderComponent>()->SetBounds(modelBounds);


}

void CModelComponent::ResetModel()
{
	m_Model.reset();
	ResetBones();
	ResetAnimation();
}

std::shared_ptr<IModel> CModelComponent::GetModel() const
{
	return m_Model;
}

void CModelComponent::SetCastShadows(bool Value)
{
	m_IsCastShadows = Value;
}

bool CModelComponent::IsCastShadows() const
{
	return m_IsCastShadows;
}



//
// Bones functional
//
/*std::shared_ptr<ISkeletonComponentBone3D> CModelComponent::GetRootBone() const
{
	return m_RootBone;
}

std::shared_ptr<ISkeletonComponentBone3D> CModelComponent::GetBone(size_t Index) const
{
	if (Index >= m_Bones.size())
		return nullptr;
	return m_Bones.at(Index);
}

const std::vector<std::shared_ptr<ISkeletonComponentBone3D>>& CModelComponent::GetBones() const
{
	return m_Bones;
}*/

const SBoneInstance& CModelComponent::GetCalculatedBone(const std::string & BoneName) const
{
	for (const auto& b : m_BonesCalculated)
	{
		if (b.Bone != nullptr)
		{
			if (b.Bone->GetName() == BoneName)
			{
				return b;
			}
		}
	}

	throw CException("Bone '%s' not found.", BoneName.c_str());
}

const SBoneInstance& CModelComponent::GetCalculatedBone(size_t Index) const
{
	return m_BonesCalculated[Index];
}

const std::vector<SBoneInstance>& CModelComponent::GetCalculatedBones() const
{
	return m_BonesCalculated;
}

std::shared_ptr<IStructuredBuffer> CModelComponent::GetBonesSkinBuffer() const
{
	return m_StructuredBuffer;
}

void CModelComponent::CreatePose(size_t BoneStartIndex, size_t BonesCount)
{
	const std::shared_ptr<ISkeletonBone>& rootBone = GetModel()->GetSkeleton()->GetRootBone();
	const std::vector<std::shared_ptr<ISkeletonBone>>& bones = GetModel()->GetSkeleton()->GetBones();

	if (BonesCount == 0)
		BonesCount = bones.size() - BoneStartIndex;

	//_ASSERT(BoneStartIndex + BonesCount - 1 < m_Bones.size());
	
	for (size_t i = 0; i < bones.size(); i++)
	{
		if ((i >= BoneStartIndex) && i < (BoneStartIndex + BonesCount))
		{
			m_BonesCalculatedSkinMatrices[i] = rootBone->GetLocalMatrix() * m_BonesCalculated[i].BoneMatrix * bones[i]->GetSkinMatrix();
		}
		else
		{
			m_BonesCalculatedSkinMatrices[i] = glm::mat4(1.0f);
		}
	}

	m_StructuredBuffer->Set(m_BonesCalculatedSkinMatrices);
}



//
// Animation functional
//
void CModelComponent::PlayAnimation(const std::string & AnimationName, bool Loop)
{
	auto model = GetModel();
	if (model == nullptr)
		throw CException("Node '%s' don't contains model.", GetOwnerNode().GetName().c_str());

	const auto& animations = GetModel()->GetAnimations();
	if (animations.empty())
		throw CException("Animations list empty in Model '%s'.", GetModel()->GetFileName().c_str());

	const auto& animIt = animations.find(AnimationName);
	if (animIt == animations.end())
		throw CException("Animation '%s' not found.", AnimationName.c_str());

	m_IsAnimationLooped = Loop;
	m_IsAnimationPaused = false;
	m_CurrentAnimation = animIt->second.get();
	m_CurrentAnimationIndex = std::distance(animations.begin(), animIt);
	m_CurrentTime = 0.0;
}

void CModelComponent::SetAnimationEndedCallback(std::function<void(const IAnimation*)> Func)
{
	_ASSERT(m_OnAnimationEnded == nullptr);
	m_OnAnimationEnded = Func;
}

bool CModelComponent::IsAnimationPlayed() const
{
	return m_CurrentAnimation != nullptr;
}

const IAnimation * CModelComponent::GetCurrentAnimation() const
{
	return m_CurrentAnimation;
}

size_t CModelComponent::GetCurrentAnimationIndex() const
{
	return m_CurrentAnimationIndex;
}

uint32 CModelComponent::GetCurrentAnimationFrame() const
{
	if (m_CurrentAnimation == nullptr)
		return 0;

	uint32 currentAnimationFrame = m_CurrentAnimation->GetFrameStart() + static_cast<uint32>(glm::round(m_CurrentTime));
	if (currentAnimationFrame >= m_CurrentAnimation->GetFrameEnd())
	{
		_ASSERT(m_CurrentAnimation->GetFrameEnd() != 0);
		currentAnimationFrame = m_CurrentAnimation->GetFrameEnd() - 1;
	}
	return currentAnimationFrame;
}



//
// ISceneNodeComponent
//
void CModelComponent::Update(const UpdateEventArgs & e)
{
	__super::Update(e);
	
	if (m_Model == nullptr)
		return;

	if (auto skeleton = m_Model->GetSkeleton())
	{
		for (auto& b : m_BonesCalculated)
			b = SBoneInstance();

		const auto& bones = skeleton->GetBones();
		for (size_t i = 0; i < bones.size(); i++)
			CalculateBone(i, bones[i]);	
		
		CreatePose();
	}

	if ((m_CurrentAnimation != nullptr) && (false == m_IsAnimationPaused))
	{
		// Animation don't ended
		if (m_CurrentTime < m_CurrentAnimation->GetFrameEnd())
		{
			m_CurrentTime += e.DeltaTimeMultiplier;
		}
		else
		{
			m_CurrentTime = m_CurrentAnimation->GetFrameEnd() - 1;

			if (m_OnAnimationEnded)
				m_OnAnimationEnded(m_CurrentAnimation);

			// Ended!
			/*if (m_CurrentAnimation->hasNextVatianton())
			{
				m_CurrentAnimation = m_CurrentAnimation->getNextVariation();
				m_CurrentTime = m_CurrentAnimation->getStart();
				m_IsPlayed = false;
				m_AnimTime = 0;
				return;
			}*/

			if (m_IsAnimationLooped)
			{
				PlayAnimation(m_CurrentAnimation->GetName(), true);
			}
			else
			{
				PauseAnimation();
			}
		}
	}
}

void CModelComponent::Accept(IVisitor* visitor)
{
	if (GetModel())
		GetModel()->Accept(visitor);
}



//
// IObjectSaveLoad
//
void CModelComponent::CopyTo(std::shared_ptr<IObject> Destination) const
{
	CComponentBase::CopyTo(Destination);

	auto destCast = std::dynamic_pointer_cast<CModelComponent>(Destination);

	if (m_Model != nullptr)
		destCast->SetModel(m_Model);

	destCast->SetCastShadows(m_IsCastShadows);
}

void CModelComponent::Load(const std::shared_ptr<IXMLReader>& Reader)
{
	CComponentBase::Load(Reader);

	if (false == Reader->IsAttributeExists("FileName"))
	{
		Log::Warn("Model XML doesn't contains 'FileName' section. Node '%s'.", GetOwnerNode().GetName().c_str());
		return;
	}

	std::string fileName = Reader->GetStrAttribute("FileName");
	if (false == GetBaseManager().GetManager<IFilesManager>()->IsFileExists(fileName))
		throw CException("Model file '%s' doesn't exists. Node '%s'.", fileName.c_str(), GetOwnerNode().GetName().c_str());

	try
	{
		auto model = GetBaseManager().GetManager<IznModelsFactory>()->LoadModel(fileName);
		SetModel(model);
	}
	catch (const CException& e)
	{
		throw CException("Error occurs while loading '%s' model. Node '%s'. Error '%s'.", fileName.c_str(), GetOwnerNode().GetName().c_str(), e.MessageCStr());
	}
}

void CModelComponent::Save(const std::shared_ptr<IXMLWriter>& Writer) const
{
	CComponentBase::Save(Writer);

	auto model = GetModel();
	if (model == nullptr)
	{
		Log::Warn("Unable to save nullptr model. Node '%s'", GetOwnerNode().GetName().c_str());
		return;
	}
	
	auto fileName = model->GetFileName();
	if (fileName.empty())
		throw CException("Unable to save model with empty filename. Node '%s'", GetOwnerNode().GetName().c_str());

	if (false == GetBaseManager().GetManager<IFilesManager>()->IsFileExists(fileName))
		throw CException("Model file '%s' doesn't exists. Node '%s'.", fileName.c_str(), GetOwnerNode().GetName().c_str());

	CXMLManager xml(GetBaseManager());
	Writer->SetStrAttribute(model->GetFileName(), "FileName");
}



//
// Protected
//

// Bones

void CModelComponent::InitializeBones()
{
	if (auto skeleton = m_Model->GetSkeleton())
	{
		const auto& bones = skeleton->GetBones();
		m_BonesCalculated.resize(bones.size());
		m_BonesCalculatedSkinMatrices.resize(bones.size());

		m_StructuredBuffer = GetBaseManager().GetApplication().GetRenderDevice().GetObjectsFactory().CreateStructuredBuffer(nullptr, bones.size(), sizeof(glm::mat4), EAccess::CPUWrite);
	}
}

void CModelComponent::CalculateBone(size_t BoneIndex, const std::shared_ptr<ISkeletonBone>& Bone)
{
	SBoneInstance& boneInstance = m_BonesCalculated[BoneIndex];

	if (boneInstance.IsCalculated)
		return;

	int32 parentIndex = Bone->GetParentIndex();
	if (parentIndex != -1)
		CalculateBone(parentIndex, GetModel()->GetSkeleton()->GetBone(parentIndex));

	boneInstance.BoneMatrix = CalculateBoneLocalMatrix(Bone);

	if (parentIndex != -1)
		boneInstance.BoneMatrix = m_BonesCalculated[parentIndex].BoneMatrix * boneInstance.BoneMatrix;

	boneInstance.Bone = Bone.get();
	boneInstance.IsCalculated = true;
}

glm::mat4 CModelComponent::CalculateBoneLocalMatrix(const std::shared_ptr<ISkeletonBone>& Bone)
{
	glm::mat4 m(1.0f);

	m *= Bone->GetPivotMatrix();
	{
		if (const IAnimation* currentAnimation = GetCurrentAnimation())
		{
			if (const auto& skeletonAnimation = currentAnimation->GetSkeletonAnimation())
			{
				if (Bone->IsRootBone())
					m *= glm::inverse(currentAnimation->GetSkeletonAnimation()->GetRootBoneMatrix());

				if (skeletonAnimation->IsBoneAnimated(Bone->GetName(), GetCurrentAnimationFrame()))
				{
					m *= skeletonAnimation->CalculateBoneMatrix(Bone->GetName(), GetCurrentAnimationFrame());
				}
				else
				{
					m *= Bone->GetLocalMatrix();
				}
			}
			else
			{
				if (Bone->IsRootBone())
					m *= glm::inverse(GetModel()->GetSkeleton()->GetRootBoneLocalTransform());
				m *= Bone->GetLocalMatrix();
			}
		}
		else
		{
			if (Bone->IsRootBone())
				m *= glm::inverse(GetModel()->GetSkeleton()->GetRootBoneLocalTransform());
			m *= Bone->GetLocalMatrix();
		}
	}
	m *= glm::inverse(Bone->GetPivotMatrix());

	return m;
}

void CModelComponent::ResetBones()
{
	m_BonesCalculated.clear();
	m_BonesCalculatedSkinMatrices.clear();
}

// Animations

void CModelComponent::PauseAnimation()
{
	m_IsAnimationPaused = true;
}

void CModelComponent::ResetAnimation()
{
	m_CurrentAnimationIndex = 0;
	m_CurrentAnimation = nullptr;
	m_IsAnimationLooped = false;
	m_IsAnimationPaused = false;
	m_CurrentTime = 0.0;
}
