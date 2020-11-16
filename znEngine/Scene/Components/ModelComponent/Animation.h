#pragma once

class ZN_API CAnimation
	: public IAnimation
{
public:
	CAnimation(uint16 IndexIntoSequences, const std::string& Name, uint32 FrameStart, uint32 FrameEnd);
	virtual ~CAnimation();

	// IAnimation
	uint16 GetIndexInSequences() const override;
	const std::string& GetName() const override;
	uint32 GetFrameStart() const override;
	uint32 GetFrameEnd() const override;

private:
	uint16 m_IndexIntoSequences;
	std::string m_Name;
	uint32 m_FrameStart;
	uint32 m_FrameEnd;
};