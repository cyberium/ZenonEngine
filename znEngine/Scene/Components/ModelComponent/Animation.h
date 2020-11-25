#pragma once

class ZN_API CAnimation
	: public IAnimation
	, public IAnimationInternal
	, public IObjectLoadSave
{
public:
	CAnimation(const std::shared_ptr<IByteBuffer>& Buffer);
	CAnimation(uint16 IndexIntoSequences, const std::string& Name, uint32 FrameStart, uint32 FrameEnd);
	virtual ~CAnimation();

	// IAnimation
	uint16 GetIndexInSequences() const override;
	const std::string& GetName() const override;
	uint32 GetFrameStart() const override;
	uint32 GetFrameEnd() const override;

	// IAnimationInternal
	void SetName(const std::string& Name) override;

	// IObjectLoadSave
	std::shared_ptr<IObject> Copy() const override;
	void CopyTo(std::shared_ptr<IObject> Destination) const override;
	void Load(const std::shared_ptr<IByteBuffer>& Buffer) override;
	void Save(const std::shared_ptr<IByteBuffer>& Buffer) const override;
	void Load(const std::shared_ptr<IXMLReader>& Reader) override;
	void Save(const std::shared_ptr<IXMLWriter>& Writer) const override;

private:
	std::string m_Name;
	uint32 m_FrameStart;
	uint32 m_FrameEnd;
	uint16 m_IndexIntoSequences;
};