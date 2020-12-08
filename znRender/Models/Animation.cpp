#include "stdafx.h"

// General
#include "Animation.h"

CAnimation::CAnimation(const std::shared_ptr<IXMLReader>& Reader)
{
	Load(Reader);
}

CAnimation::CAnimation(const std::shared_ptr<IByteBuffer>& Buffer)
{
	Load(Buffer);
}

CAnimation::CAnimation(uint16 IndexIntoSequences, const std::string& Name, uint32 FrameStart, uint32 FrameEnd)
	: m_IndexIntoSequences(IndexIntoSequences)
	, m_Name(Name)
	, m_FrameStart(FrameStart)
	, m_FrameEnd(FrameEnd)
{
}

CAnimation::~CAnimation()
{
}



//
// IAnimation
//
const std::string& CAnimation::GetName() const
{
	return m_Name;
}

uint32 CAnimation::GetFrameStart() const
{
	return m_FrameStart;
}

uint32 CAnimation::GetFrameEnd() const
{
	return m_FrameEnd;
}

uint16 CAnimation::GetIndexInSequences() const
{
	return m_IndexIntoSequences;
}

std::shared_ptr<ISkeletonAnimation> CAnimation::GetSkeletonAnimation() const
{
	return m_SkeletonAnimation;
}



//
// IAnimationInternal
//
void CAnimation::SetName(const std::string & Name)
{
	_ASSERT(false == Name.empty());
	m_Name = Name;
}

void CAnimation::SetSkeletonAnimation(std::shared_ptr<ISkeletonAnimation> SkeletonAnimation)
{
	_ASSERT(SkeletonAnimation != nullptr);
	_ASSERT(m_SkeletonAnimation == nullptr);

	m_SkeletonAnimation = SkeletonAnimation;
}



//
// IObjectLoadSave
//
std::shared_ptr<IObject> CAnimation::Copy() const
{
	throw CException("Not implemented.");
}

void CAnimation::CopyTo(std::shared_ptr<IObject> Destination) const
{
	throw CException("Not implemented.");
}

void CAnimation::Load(const std::shared_ptr<IByteBuffer>& Buffer)
{
	Buffer->readString(&m_Name);
	Buffer->read(&m_FrameStart);
	Buffer->read(&m_FrameEnd);
	Buffer->read(&m_IndexIntoSequences);

	/*uint32 skeletonExists;
	if (Buffer->read(&skeletonExists))
	{
		if (skeletonExists == 1)
		{

		}
	}*/
}

void CAnimation::Save(const std::shared_ptr<IByteBuffer>& Buffer) const
{
	Buffer->writeString(m_Name);
	Buffer->write(&m_FrameStart);
	Buffer->write(&m_FrameEnd);
	Buffer->write(&m_IndexIntoSequences);

	/*if (auto skeletonAnimation = GetSkeletonAnimation())
	{
		uint32 skeletonExists = 1;
		Buffer->write(&skeletonExists);

		std::dynamic_pointer_cast<IObjectLoadSave>(skeletonAnimation)->Save(Buffer);
	}*/
}

void CAnimation::Load(const std::shared_ptr<IXMLReader>& Reader)
{
	m_Name = Reader->GetStrAttribute("Name");
	m_FrameStart = Reader->GetUIntAttribute("Start");
	m_FrameEnd = Reader->GetUIntAttribute("End");
	m_IndexIntoSequences = Reader->GetUIntAttribute("IndexIntoSequences");
}

void CAnimation::Save(const std::shared_ptr<IXMLWriter>& Writer) const
{
	Writer->SetStrAttribute(m_Name, "Name");
	Writer->SetUIntAttribute(m_FrameStart, "Start");
	Writer->SetUIntAttribute(m_FrameEnd, "End");
	Writer->SetUIntAttribute(m_IndexIntoSequences, "IndexIntoSequences");

	/*if (auto skeletonAnimation = GetSkeletonAnimation())
	{
		auto skeletonAnimationWriter = Writer->CreateChild("SkeletonAnimation");
		std::
	}*/
}
