#pragma once

#include "GeometryBase.h"

class ZN_API ModelBase 
	: public IModel
	, public Object
{
public:
	ModelBase(IRenderDevice& RenderDevice);
	virtual ~ModelBase();

	void                                            SetFileName(const std::string& FileName) override;
	std::string                                     GetFileName() const override;
	void                                            SetBounds(const BoundingBox& Bounds) override;
	BoundingBox                                     GetBounds() const override;
	void									        AddConnection(const std::shared_ptr<IMaterial>& Material, const std::shared_ptr<IGeometry>& Geometry, SGeometryDrawArgs GeometryDrawArgs = SGeometryDrawArgs()) override;
	const std::vector<SConnection>&                 GetConnections() const override;

	// Skeleton
	void                                            ApplyOtherSkeleton(std::shared_ptr<IModel> other) override;
	void                                            AddBone(const std::shared_ptr<ISkeletonBone> Bone) override;
	void                                            SetFixSkeleton(const glm::mat4& Matrix) override;
	glm::mat4                                       GetFixSkeleton() const override;
	std::shared_ptr<ISkeletonBone>                  GetRootBone() const override;
	std::shared_ptr<ISkeletonBone>                  GetBone(size_t Index) const override;
	size_t                                          GetBoneIndexByName(const std::string& BoneName) const override;
	std::shared_ptr<ISkeletonBone>                  GetBoneByName(const std::string& BoneName) const override;
	const std::vector<std::shared_ptr<ISkeletonBone>>& GetBones() const override;


	// Animation
	void                                            AddAnimation(const std::string& AnimationName, const std::shared_ptr<IAnimation>& Animation);
	const Animations_t&                             GetAnimations() const;

	virtual void                                    Accept(IVisitor* visitor) override;

protected:
	void                                            UpdateBounds(const std::shared_ptr<IGeometry>& Geometry);

protected:
	BoundingBox                                     m_BoundingBox;
	std::vector<SConnection>                        m_Connections;
	std::string                                     m_FileName;

	// Bones & skeleton
	glm::mat4                                       m_FixMatrix;
	std::shared_ptr<ISkeletonBone>                  m_RootBone;
	std::vector<std::shared_ptr<ISkeletonBone>>     m_Bones;

	// Animations
	Animations_t                                    m_Animations;

private:
	IRenderDevice&                                  m_RenderDevice;
};
