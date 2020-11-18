#pragma once

#include "ObjectsFactories/Object.h"

class ZN_API ModelProxie 
	: public IModel
	, public Object
{
public:
	ModelProxie(const std::shared_ptr<IModel>& Model);
	virtual ~ModelProxie();

	// IModel
	virtual void SetBounds(const BoundingBox& Bounds) override;
	virtual BoundingBox GetBounds() const override;
	void SetFileName(const std::string& FileName) override;
	std::string GetFileName() const override;
	void AddConnection(const std::shared_ptr<IMaterial>& Material, const std::shared_ptr<IGeometry>& Geometry, SGeometryDrawArgs GeometryDrawArgs = SGeometryDrawArgs()) override final;
	const std::vector<SConnection>& GetConnections() const override final;

	// Skeleton
	void                                                      ApplyOtherSkeleton(std::shared_ptr<IModel> other) override;
	void                                                      AddBone(const std::shared_ptr<ISkeletonBone> Bone) override;
	void                                                      SetFixSkeleton(const glm::mat4& Matrix) override;
	glm::mat4                                                 GetFixSkeleton() const override;
	std::shared_ptr<ISkeletonBone>                            GetRootBone() const override;
	std::shared_ptr<ISkeletonBone>                            GetBone(size_t Index) const override;
	size_t                                                    GetBoneIndexByName(const std::string& BoneName) const override;
	std::shared_ptr<ISkeletonBone>                            GetBoneByName(const std::string& BoneName) const override;
	const std::vector<std::shared_ptr<ISkeletonBone>>&        GetBones() const override;

	// Animations
	void                                                      AddAnimation(const std::string& AnimationName, const std::shared_ptr<IAnimation>& Animation) override;
	const Animations_t&                                       GetAnimations() const override;

	virtual bool Render() const override;
	virtual void Accept(IVisitor* visitor) override;

	// IObjectLoadSave
	void Load(const std::shared_ptr<IByteBuffer>& Buffer) override;
	void Save(const std::shared_ptr<IByteBuffer>& Buffer) const override;
	void Load(const std::shared_ptr<IXMLReader>& Reader) override;
	void Save(const std::shared_ptr<IXMLWriter>& Writer) const override;

protected:
	const std::shared_ptr<IModel> GetModel() const;

private:
	std::shared_ptr<IModel> m_Model;
};