#include "stdafx.h"

// General
#include "SkeletonAnimation.h"

CSkeletonAnimation::CSkeletonAnimation()
{
}

CSkeletonAnimation::~CSkeletonAnimation()
{
}



//
// CSkeletonAnimation
//
void CSkeletonAnimation::SetRootBoneMatrix(const glm::mat4 & RootBoneLocalTransform)
{
	m_RootBoneMatrix = RootBoneLocalTransform;
}

void CSkeletonAnimation::AddBone(const std::string & BoneName, const CznAnimatedValue<glm::mat4>& AnimatedValue)
{
	const auto& boneIt = m_BonesTransformAnimation.find(BoneName);
	if (boneIt != m_BonesTransformAnimation.end())
		throw CException("Unable add bone '%s' to skeleton animation, because already exists.", BoneName.c_str());
	m_BonesTransformAnimation.insert(std::make_pair(BoneName, AnimatedValue));
}



//
// ISkeletonAnimation
//
glm::mat4 CSkeletonAnimation::GetRootBoneMatrix() const
{
	return m_RootBoneMatrix;
}

bool CSkeletonAnimation::IsBoneAnimated(const std::string & BoneName, uint32 FrameIndex) const
{
	const auto& boneIt = m_BonesTransformAnimation.find(BoneName);
	if (boneIt == m_BonesTransformAnimation.end())
		return false;

	if (false == boneIt->second.IsAnimated(FrameIndex))
		return false;

	return true;
}

glm::mat4 CSkeletonAnimation::CalculateBoneMatrix(const std::string & BoneName, uint32 FrameIndex) const
{
	const auto& boneIt = m_BonesTransformAnimation.find(BoneName);
	if (boneIt == m_BonesTransformAnimation.end())
		return glm::mat4(1.0f);

	const auto& boneAnimated = boneIt->second;
	if (false == boneAnimated.IsAnimated(FrameIndex))
		return glm::mat4(1.0f);

	return boneAnimated.GetValue(FrameIndex);
}



//
// IObjectLoadSave
//
std::shared_ptr<IObject> CSkeletonAnimation::Copy() const
{
	throw CException("Not implemented.");
}

void CSkeletonAnimation::CopyTo(std::shared_ptr<IObject> Destination) const
{
	throw CException("Not implemented.");
}

void CSkeletonAnimation::Load(const std::shared_ptr<IByteBuffer>& Buffer)
{
	Buffer->read(&m_RootBoneMatrix);

	size_t bonesTransformAnimationsCount;
	Buffer->read(&bonesTransformAnimationsCount);
	for (size_t i = 0; i < bonesTransformAnimationsCount; i++)
	{
		std::string boneName;
		Buffer->readString(&boneName);
		
		CznAnimatedValue<glm::mat4> animatedValue;
		animatedValue.Load(Buffer);

		m_BonesTransformAnimation.insert(std::make_pair(boneName, animatedValue));
	}
}

void CSkeletonAnimation::Save(const std::shared_ptr<IByteBuffer>& Buffer) const
{
	Buffer->write(&m_RootBoneMatrix);

	size_t bonesTransformAnimationsCount = m_BonesTransformAnimation.size();
	Buffer->write(&bonesTransformAnimationsCount);
	for (const auto& boneTransformAnimationsIt : m_BonesTransformAnimation)
	{
		Buffer->writeString(boneTransformAnimationsIt.first);
		boneTransformAnimationsIt.second.Save(Buffer);
	}
}

void CSkeletonAnimation::Load(const std::shared_ptr<IXMLReader>& Reader)
{
	m_RootBoneMatrix = Utils::StringToMatrix(Reader->GetStrAttribute("RootBoneMatrix"));

	for (const auto& childIt : Reader->GetChilds())
	{
		std::string boneName = childIt->GetName();
		std::shared_ptr<CByteBuffer> byteBuffer = MakeShared(CByteBuffer, Utils::Base64_Decode(Reader->GetValue()));

		CznAnimatedValue<glm::mat4> animatedValue;
		animatedValue.Load(byteBuffer);

		m_BonesTransformAnimation.insert(std::make_pair(boneName, animatedValue));
	}
}

void CSkeletonAnimation::Save(const std::shared_ptr<IXMLWriter>& Writer) const
{
	Writer->SetStrAttribute(Utils::MatrixToString(m_RootBoneMatrix), "RootBoneMatrix");

	for (const auto& boneTransformAnimationsIt : m_BonesTransformAnimation)
	{
		auto boneTransformAnimationWriterXML = Writer->CreateChild(boneTransformAnimationsIt.first);
		std::shared_ptr<CByteBuffer> byteBuffer = MakeShared(CByteBuffer);
		boneTransformAnimationsIt.second.Save(byteBuffer);
		boneTransformAnimationWriterXML->SetValue(Utils::Base64_Encode(byteBuffer->getData(), byteBuffer->getSize()));
	}
}