#pragma once

#include "AnimatedValueSingle.h"

class CSkeletonAnimation
	: public ISkeletonAnimation
	, public IObjectLoadSave
{
public:
	CSkeletonAnimation();
	virtual ~CSkeletonAnimation();

	// CSkeletonAnimation
	void SetRootBoneMatrix(const glm::mat4& RootBoneLocalTransform);
	void AddBone(const std::string& BoneName, const CznAnimatedValue<glm::mat4>& AnimatedValue);

	// ISkeletonAnimation
	glm::mat4 GetRootBoneMatrix() const override;
	bool IsBoneAnimated(const std::string& BoneName, uint32 FrameIndex) const override;
	glm::mat4 CalculateBoneMatrix(const std::string& BoneName, uint32 FrameIndex) const override;

	// IObjectLoadSave
	std::shared_ptr<IObject> Copy() const override;
	void CopyTo(std::shared_ptr<IObject> Destination) const override;
	void Load(const std::shared_ptr<IByteBuffer>& Buffer) override;
	void Save(const std::shared_ptr<IByteBuffer>& Buffer) const override;
	void Load(const std::shared_ptr<IXMLReader>& Reader) override;
	void Save(const std::shared_ptr<IXMLWriter>& Writer) const override;

private:
	glm::mat4 m_RootBoneMatrix;
	std::map<std::string, CznAnimatedValue<glm::mat4>> m_BonesTransformAnimation;
};