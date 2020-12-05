#include "stdafx.h"

// General
#include "ModelComponent.h"

// Additional
#include "SkeletonComponentBone.h"

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
		throw CException("Model '%s' with inifinity bounds can't be assigned to node '%s'.", Model->GetFileName().c_str(), GetOwnerNode().GetName().c_str());

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
std::shared_ptr<ISkeletonComponentBone3D> CModelComponent::GetRootBone() const
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
}

std::shared_ptr<IStructuredBuffer> CModelComponent::GetBonesBuffer() const
{
	return m_StructuredBuffer;
}

std::vector<glm::mat4> CModelComponent::CreatePose(size_t BoneStartIndex, size_t BonesCount) const
{
	if (BonesCount == 0)
		BonesCount = m_Bones.size();

	const glm::mat4& rootBoneMatrix = GetModel()->GetRootBone()->GetLocalMatrix();

	_ASSERT(BoneStartIndex + BonesCount - 1 < m_Bones.size());
	std::vector<glm::mat4> result;
	result.reserve(BonesCount);
	for (size_t i = BoneStartIndex; i < BoneStartIndex + BonesCount; i++)
		result.push_back(rootBoneMatrix * m_Bones[i]->GetCalculatedMatrix() * m_Bones[i]->GetProtoBone().GetSkinMatrix());
	return result;
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

	if (false == m_Bones.empty())
	{
		for (const auto& b : m_Bones)
			std::dynamic_pointer_cast<ISkeletonComponentBoneInternal3D>(b)->Reset();

		for (const auto& b : m_Bones)
			std::dynamic_pointer_cast<ISkeletonComponentBoneInternal3D>(b)->Calculate(this, e.CameraForCulling);

		m_BonesList = CreatePose();
		m_StructuredBuffer->Set(m_BonesList);
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
	m_RootBone = nullptr;

	for (const auto& boneProto : GetModel()->GetBones())
	{
		std::shared_ptr<CSkeletonComponentBone3D> bone = MakeShared(CSkeletonComponentBone3D, boneProto);
		AddBone(bone);

		// Resolve root bone
		if (boneProto->GetParentIndex() == -1)
		{
			if (m_RootBone != nullptr)
				throw CException("ModelComponent: Unable to set '%s' as root bone, because '%s' already root.", boneProto->GetName().c_str(), m_RootBone->GetName().c_str());
			m_RootBone = bone;
			Log::Green("ModelComponent: '%s' is root bone.", m_RootBone->GetName().c_str());
		}
	}

	for (const auto& bone : GetBones())
		std::dynamic_pointer_cast<ISkeletonComponentBoneInternal3D>(bone)->SetParentAndChildsInternals(GetBones());

	if (false == GetBones().empty())
	{
		m_BonesList.resize(m_Bones.size());
		m_StructuredBuffer = GetBaseManager().GetApplication().GetRenderDevice().GetObjectsFactory().CreateStructuredBuffer(nullptr, 256, sizeof(glm::mat4), EAccess::CPUWrite);
	}
}

void CModelComponent::AddBone(std::shared_ptr<ISkeletonComponentBone3D> Bone)
{
	m_Bones.push_back(Bone);
}

void CModelComponent::ResetBones()
{
	m_RootBone = nullptr;
	m_Bones.clear();
	m_BonesList.clear();
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
