#include "stdafx.h"

// General
#include "ModelsComponent3D.h"

// Additional
#include "SkeletonComponentBone.h"

CModelsComponent3D::CModelsComponent3D(const ISceneNode& OwnerNode)
    : CComponentBase(OwnerNode)
	, m_IsCastShadows(true)
	, m_IsLoop(false)
	, m_IsStopped(true)
	, animtime(0.0)
	, m_CurrentAnimation(nullptr)
	, m_CurrentAnimationIndex(0)
	, m_CurrentTime(0)
	, m_GlobalTime(0)
{
	GetProperties()->SetName("ModelsComponent"); // TODO: ObjectClassName

	{
		auto minBounds = MakeShared(CPropertyWrapped<bool>, "CastShadows");
		minBounds->SetValueSetter(std::bind(&CModelsComponent3D::SetCastShadows, this, std::placeholders::_1));
		minBounds->SetValueGetter(std::bind(&CModelsComponent3D::IsCastShadows, this));
		GetProperties()->AddProperty(minBounds);
	}
}

CModelsComponent3D::~CModelsComponent3D()
{}



//
// IModelsComponent3D
//
void CModelsComponent3D::SetModel(const std::shared_ptr<IModel>& Model)
{
	if (Model == nullptr)
		throw CException("Nullptr model can't be assigned to node '%s'.", GetOwnerNode().GetName().c_str());

	if (m_Model != nullptr)
		Log::Warn("Node '%s' already has model '%s'. Do you really want to replace it with '%s'?", GetOwnerNode().GetName().c_str(), m_Model->GetFileName().c_str(), Model->GetFileName().c_str());

	m_Model = Model;
	const auto& modelBounds = GetModel()->GetBounds();
	if (modelBounds.IsInfinite())
		throw CException("Model '%s' with inifinity bounds can't be assigned to node '%s'.", Model->GetFileName().c_str(), GetOwnerNode().GetName().c_str());

	InitializeBones();

	GetComponentT<IColliderComponent3D>()->ExtendBounds(modelBounds);
}

void CModelsComponent3D::ResetModel()
{
	m_Model.reset();
}

std::shared_ptr<IModel> CModelsComponent3D::GetModel() const
{
	return m_Model;
}

void CModelsComponent3D::SetCastShadows(bool Value)
{
	m_IsCastShadows = Value;
}

bool CModelsComponent3D::IsCastShadows() const
{
	return m_IsCastShadows;
}



//
// Bones functional
//
std::shared_ptr<ISkeletonComponentBone3D> CModelsComponent3D::GetBone(size_t Index) const
{
	if (Index >= m_Bones.size())
		return nullptr;
	return m_Bones.at(Index);
}

const std::vector<std::shared_ptr<ISkeletonComponentBone3D>>& CModelsComponent3D::GetBones() const
{
	return m_Bones;
}

std::shared_ptr<IStructuredBuffer> CModelsComponent3D::GetBonesBuffer() const
{
	return m_StructuredBuffer;
}

std::vector<glm::mat4> CModelsComponent3D::CreatePose(size_t BoneStartIndex, size_t BonesCount) const
{
	if (BonesCount == 0)
		BonesCount = m_Bones.size();

	_ASSERT(BoneStartIndex + BonesCount < m_Bones.size());
	std::vector<glm::mat4> result;
	result.reserve(BonesCount);
	for (size_t i = BoneStartIndex; i < BoneStartIndex + BonesCount; i++)
		result.push_back(m_Bones[i]->GetMatrix());
	return result;
}



//
// Animation functional
//
void CModelsComponent3D::PlayAnimation(uint16 AnimationId, bool Loop)
{
	m_IsLoop = Loop;

	const auto& animations = GetModel()->GetAnimations();
	const auto& animIt = animations.find(AnimationId);
	if (animIt != animations.end())
	{
		m_CurrentAnimation = animIt->second.get();
		m_CurrentAnimationIndex = animIt->first;
	}
	else
	{
		_ASSERT(animations.size() > 0);
		m_CurrentAnimation = animations.begin()->second.get();
		m_CurrentAnimationIndex = 0;
	}

	m_CurrentTime = m_CurrentAnimation->GetFrameStart();
	m_IsStopped = false;
	animtime = 0.0;
}

size_t CModelsComponent3D::GetCurrentAnimationIndex() const
{
	return m_CurrentAnimation->GetIndexInSequences();
}

uint32 CModelsComponent3D::GetCurrentTime_() const
{
	return m_CurrentTime;
}

uint32 CModelsComponent3D::GetGlobalTime() const
{
	return m_GlobalTime;
}


//
// ISceneNodeComponent
//
void CModelsComponent3D::Update(const UpdateEventArgs & e)
{
	if (false == m_Bones.empty())
	{
		for (const auto& b : m_Bones)
			std::dynamic_pointer_cast<ISkeletonComponentBoneInternal3D>(b)->Reset();

		for (const auto& b : m_Bones)
			std::dynamic_pointer_cast<ISkeletonComponentBoneInternal3D>(b)->Calculate(this, e.CameraForCulling);

		//m_BonesList = CreatePose();
		//m_StructuredBuffer->Set(m_BonesList);
	}

	if (false == m_IsStopped)
	{
		animtime += e.DeltaTime ; // FIXME
		m_CurrentTime = static_cast<uint32>(m_CurrentAnimation->GetFrameStart() + animtime);
		m_GlobalTime = static_cast<uint32>(e.TotalTime);

		// Animation don't ended
		if (m_CurrentTime >= m_CurrentAnimation->GetFrameEnd())
		{
			// Ended!
			/*if (m_CurrentAnimation->hasNextVatianton())
			{
				m_CurrentAnimation = m_CurrentAnimation->getNextVariation();
				m_CurrentTime = m_CurrentAnimation->getStart();
				m_IsPlayed = false;
				animtime = 0;
				return;
			}*/

			m_CurrentTime = m_CurrentAnimation->GetFrameEnd() - 1;
			m_IsStopped = true;

			if (m_IsLoop)
			{
				PlayAnimation(m_CurrentAnimationIndex, true);
			}
		}
	}
}

void CModelsComponent3D::Accept(IVisitor* visitor)
{
	if (GetModel())
		GetModel()->Accept(visitor);
}



//
// IObjectSaveLoad
//
void CModelsComponent3D::CopyTo(std::shared_ptr<IObject> Destination) const
{
	CComponentBase::CopyTo(Destination);

	auto destCast = std::dynamic_pointer_cast<CModelsComponent3D>(Destination);

	if (m_Model != nullptr)
		destCast->SetModel(m_Model);

	destCast->SetCastShadows(m_IsCastShadows);
}

void CModelsComponent3D::Load(const std::shared_ptr<IXMLReader>& Reader)
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

void CModelsComponent3D::Save(const std::shared_ptr<IXMLWriter>& Writer) const
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
void CModelsComponent3D::InitializeBones()
{
	for (const auto& b : GetModel()->GetBones())
	{
		auto bone = MakeShared(CSkeletonComponentBone3D, b);
		AddBone(bone);
	}

	for (const auto& bone : GetBones())
		std::dynamic_pointer_cast<ISkeletonComponentBoneInternal3D>(bone)->SetParentAndChildsInternals(GetBones());

	if (false == GetBones().empty())
	{
		m_BonesList.resize(m_Bones.size());
		m_StructuredBuffer = GetBaseManager().GetApplication().GetRenderDevice().GetObjectsFactory().CreateStructuredBuffer(nullptr, 256, sizeof(glm::mat4), EAccess::CPUWrite);
	}
}

void CModelsComponent3D::AddBone(std::shared_ptr<ISkeletonComponentBone3D> Bone)
{
	m_Bones.push_back(Bone);
}
