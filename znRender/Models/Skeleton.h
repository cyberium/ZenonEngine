#pragma once

class CSkeleton
	: public ISkeleton
	, public ISkeletonInternal
	, public IObjectLoadSave
{
public:
	CSkeleton();
	virtual ~CSkeleton();

	// ISkeleton
	std::shared_ptr<ISkeletonBone> GetRootBone() const override;
	std::shared_ptr<ISkeletonBone> GetBone(size_t Index) const override;
	size_t GetBoneIndexByName(const std::string& BoneName) const override;
	std::shared_ptr<ISkeletonBone> GetBoneByName(const std::string& BoneName) const override;
	const std::vector<std::shared_ptr<ISkeletonBone>>& GetBones() const override;
	const glm::mat4& GetRootBoneLocalTransform() const override;

	// ISkeletonInternal
	void SetRootBoneLocalTranform(glm::mat4 RootBoneLocalTranform) override;
	void AddBone(std::shared_ptr<ISkeletonBone> Bone) override;

	// IObjectLoadSave
	std::shared_ptr<IObject> Copy() const override;
	void CopyTo(std::shared_ptr<IObject> Destination) const override;
	void Load(const std::shared_ptr<IByteBuffer>& Buffer) override;
	void Save(const std::shared_ptr<IByteBuffer>& Buffer) const override;
	void Load(const std::shared_ptr<IXMLReader>& Reader) override;
	void Save(const std::shared_ptr<IXMLWriter>& Writer) const override;

private:
	std::shared_ptr<ISkeletonBone> m_RootBone;
	std::vector<std::shared_ptr<ISkeletonBone>> m_Bones;

	glm::mat4 m_RootBoneLocalTranform;
};