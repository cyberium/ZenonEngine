#pragma once

class CAnimation
	: public IAnimation
{
public:
	CAnimation(const std::string& Name, uint32 FrameStart, uint32 FrameEnd);
	virtual ~CAnimation();

	// IAnimation
	const std::string& GetName() const override;
	uint32 GetFrameStart() const override;
	uint32 GetFrameEnd() const override;

private:
	std::string m_Name;
	uint32 m_FrameStart;
	uint32 m_FrameEnd;
};