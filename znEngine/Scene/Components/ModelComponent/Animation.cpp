#include "stdafx.h"

// General
#include "Animation.h"

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
uint16 CAnimation::GetIndexInSequences() const
{
	return m_IndexIntoSequences;
}

const std::string & CAnimation::GetName() const
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
