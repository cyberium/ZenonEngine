#pragma once

#include "AnimatedValue.h"

class ZN_API CSkeletonBone
	: public ISkeletonBone
	, public IObjectLoadSave
{
public:
	CSkeletonBone(const std::shared_ptr<IByteBuffer>& Buffer);
	CSkeletonBone(const std::string& Name, int32 ParentIndex);
	virtual ~CSkeletonBone();

	// ISkeletonBone
	void MergeWithOther(std::shared_ptr<ISkeletonBone> OtherBone) override;

	std::string GetName() const override;
	int32 GetParentIndex() const override;
	void SetLocalMatrix(const glm::mat4& Matrix) override;
	glm::mat4 GetLocalMatrix() const override;
	void SetPivotMatrix(const glm::mat4& Matrix) override;
	glm::mat4 GetPivotMatrix() const override;
	void SetFuckingMatrix(const glm::mat4& Matrix) override;
	glm::mat4 GetFuckingMatrix() const override;

	glm::mat4 CalcMatrix(const IModelsComponent3D* ModelsComponent) const override;
	glm::mat4 CalcRotateMatrix(const IModelsComponent3D* ModelsComponent) const override;

	// IObjectLoadSave
	std::shared_ptr<IObject> Copy() const override;
	void CopyTo(std::shared_ptr<IObject> Destination) const override;
	void Load(const std::shared_ptr<IByteBuffer>& Buffer) override;
	void Save(const std::shared_ptr<IByteBuffer>& Buffer) const override;
	void Load(const std::shared_ptr<IXMLReader>& Reader) override;
	void Save(const std::shared_ptr<IXMLWriter>& Writer) const override;

protected:
	bool IsRootBone() const;

private:
	std::string          m_Name;
	int32                m_ParentIndex;

	glm::mat4            m_LocalTransform;
	glm::mat4            m_PivotMatrix;
	glm::mat4            m_FuckingMatrix;
	
public:
	CznAnimatedValue<float> pX, pY, pZ;
	CznAnimatedValue<float> rX, rY, rZ;
	CznAnimatedValue<float> sX, sY, sZ;

	CznAnimatedValue<glm::mat4> m_CalculatedMatrixes;
};