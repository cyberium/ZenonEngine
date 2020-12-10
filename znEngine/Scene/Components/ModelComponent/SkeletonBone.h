#pragma once

class ZN_API CSkeletonBone
	: public ISkeletonBone
	, public IObjectLoadSave
{
public:
	CSkeletonBone(const std::shared_ptr<IXMLReader>& Reader);
	CSkeletonBone(const std::shared_ptr<IByteBuffer>& Buffer);
	CSkeletonBone(const std::string& Name, int32 ParentIndex);
	virtual ~CSkeletonBone();

	// ISkeletonBone
	std::string GetName() const override;
	int32 GetParentIndex() const override;
	bool IsRootBone() const override;
	void SetLocalMatrix(const glm::mat4& Matrix) override;
	glm::mat4 GetLocalMatrix() const override;
	void SetPivotMatrix(const glm::mat4& Matrix) override;
	glm::mat4 GetPivotMatrix() const override;
	void SetSkinMatrix(const glm::mat4& Matrix) override;
	glm::mat4 GetSkinMatrix() const override;

	// IObjectLoadSave
	std::shared_ptr<IObject> Copy() const override;
	void CopyTo(std::shared_ptr<IObject> Destination) const override;
	void Load(const std::shared_ptr<IByteBuffer>& Buffer) override;
	void Save(const std::shared_ptr<IByteBuffer>& Buffer) const override;
	void Load(const std::shared_ptr<IXMLReader>& Reader) override;
	void Save(const std::shared_ptr<IXMLWriter>& Writer) const override;

private:
	std::string          m_Name;
	int32                m_ParentIndex;

	glm::mat4            m_LocalTransform;
	glm::mat4            m_PivotMatrix;
	glm::mat4            m_SkinMatrix;
};