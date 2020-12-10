#include "stdafx.h"

// General
#include "Skeleton.h"

// Additional
#include "SkeletonBone.h"

CSkeleton::CSkeleton()
{
}

CSkeleton::~CSkeleton()
{
}



//
// ISkeleton
//
std::shared_ptr<ISkeletonBone> CSkeleton::GetRootBone() const
{
	return m_RootBone;
}

std::shared_ptr<ISkeletonBone> CSkeleton::GetBone(size_t Index) const
{
	_ASSERT(Index < m_Bones.size());
	return m_Bones.at(Index);
}

size_t CSkeleton::GetBoneIndexByName(const std::string& BoneName) const
{
	for (size_t i = 0; i < m_Bones.size(); ++i)
		if (::_stricmp(m_Bones[i]->GetName().c_str(), BoneName.c_str()) == 0)
			return i;
	throw CException("Bone '%s' not found.", BoneName.c_str());
}

std::shared_ptr<ISkeletonBone> CSkeleton::GetBoneByName(const std::string& BoneName) const
{
	for (size_t i = 0; i < m_Bones.size(); ++i)
		if (::_stricmp(m_Bones[i]->GetName().c_str(), BoneName.c_str()) == 0)
			return m_Bones[i];
	throw CException("Bone '%s' not found.", BoneName.c_str());
}

const std::vector<std::shared_ptr<ISkeletonBone>>& CSkeleton::GetBones() const
{
	return m_Bones;
}

const glm::mat4& CSkeleton::GetRootBoneLocalTransform() const
{
	return m_RootBoneLocalTranform;
}



//
// ISkeletonInternal
//
void CSkeleton::SetRootBoneLocalTranform(glm::mat4 RootBoneLocalTranform)
{
	m_RootBoneLocalTranform = RootBoneLocalTranform;
}

void CSkeleton::AddBone(std::shared_ptr<ISkeletonBone> Bone)
{
	if (Bone->GetParentIndex() == -1)
	{
		if (m_RootBone != nullptr)
			throw CException("Unable to set '%s' as root bone, because '%s' already root.", Bone->GetName().c_str(), m_RootBone->GetName().c_str());
		m_RootBone = Bone;
		Log::Green("SkeletonAnimation: '%s' is root bone.", m_RootBone->GetName().c_str());
	}

	m_Bones.push_back(Bone);
}



//
// IObjectLoadSave
//
std::shared_ptr<IObject> CSkeleton::Copy() const
{
	throw CException("Not implemented.");
}

void CSkeleton::CopyTo(std::shared_ptr<IObject> Destination) const
{
	throw CException("Not implemented.");
}

void CSkeleton::Load(const std::shared_ptr<IByteBuffer>& Buffer)
{
	Buffer->read(&m_RootBoneLocalTranform);

	std::vector<std::shared_ptr<IAnimation>> bones;
	size_t bonesCount;
	if (Buffer->read(&bonesCount))
	{
		bones.reserve(bonesCount);
		for (size_t i = 0; i < bonesCount; i++)
			AddBone(MakeShared(CSkeletonBone, Buffer));
	}
}

void CSkeleton::Save(const std::shared_ptr<IByteBuffer>& Buffer) const
{
	Buffer->write(&m_RootBoneLocalTranform);

	const auto& bones = GetBones();
	size_t bonesCount = bones.size();
	Buffer->write(&bonesCount);
	for (const auto& b : bones)
		std::dynamic_pointer_cast<IObjectLoadSave>(b)->Save(Buffer);
}

void CSkeleton::Load(const std::shared_ptr<IXMLReader>& Reader)
{
	m_RootBoneLocalTranform = Utils::StringToMatrix(Reader->GetStrAttribute("RootBoneLocalTranform"));

	for (const auto& boneReaderXML : Reader->GetChild("Bones")->GetChilds())
		AddBone(MakeShared(CSkeletonBone, boneReaderXML));
}

void CSkeleton::Save(const std::shared_ptr<IXMLWriter>& Writer) const
{
	Writer->SetStrAttribute(Utils::MatrixToString(m_RootBoneLocalTranform), "RootBoneLocalTranform");

	auto bonesWriterXML = Writer->CreateChild("Bones");
	for (const auto& b : GetBones())
	{
		auto boneWriterXML = bonesWriterXML->CreateChild("Bone");
		std::dynamic_pointer_cast<IObjectLoadSave>(b)->Save(boneWriterXML);
	}
}