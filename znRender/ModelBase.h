#pragma once

#include "GeometryBase.h"

class ZN_API ModelBase 
	: public IModel
	, public IModelInternal
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
	const std::shared_ptr<ISkeleton>&               GetSkeleton() const override;

	// Animation
	void                                            AddAnimation(const std::string& AnimationName, const std::shared_ptr<IAnimation>& Animation);
	const Animations_t&                             GetAnimations() const;

	virtual void                                    Accept(IVisitor* visitor) override;


	// IModelInternal
	void                                            SetSkeleton(std::shared_ptr<ISkeleton> Skeleton) override;

protected:
	void                                            UpdateBounds(const std::shared_ptr<IGeometry>& Geometry);

protected:
	BoundingBox                                     m_BoundingBox;
	std::vector<SConnection>                        m_Connections;
	std::string                                     m_FileName;

	// Skeleton
	std::shared_ptr<ISkeleton>                      m_Skeleton;

	// Animations
	Animations_t                                    m_Animations;

private:
	IRenderDevice&                                  m_RenderDevice;
};
