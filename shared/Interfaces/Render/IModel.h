#pragma once

#include "Interfaces/Render/Types/RenderTypes.h"
#include "Interfaces/Render/ISkeleton.h"

// FORWARD BEGIN
ZN_INTERFACE IMaterial;
ZN_INTERFACE IGeometry;
class RenderEventArgs;
ZN_INTERFACE IModelComponent;

ZN_INTERFACE ISkeletonBone;
ZN_INTERFACE ISkeleton;
ZN_INTERFACE ISkeletonInternal;

ZN_INTERFACE IAnimation;
ZN_INTERFACE ISkeletonAnimation;



// FORWARD END


ZN_INTERFACE ZN_API ISkeletonBone
{
	virtual ~ISkeletonBone() {}

	virtual std::string GetName() const = 0;
	virtual int32 GetParentIndex() const = 0;
	
	virtual void SetLocalMatrix(const glm::mat4& Matrix) = 0;
	virtual glm::mat4 GetLocalMatrix() const = 0;

	virtual void SetPivotMatrix(const glm::mat4& Matrix) = 0;
	virtual glm::mat4 GetPivotMatrix() const = 0;

	virtual void SetSkinMatrix(const glm::mat4& Matrix) = 0;
	virtual glm::mat4 GetSkinMatrix() const = 0;

	virtual glm::mat4 CalculateBontMatrix(const IModelComponent* ModelsComponent) const = 0;
};

ZN_INTERFACE ZN_API ISkeleton
{
	virtual ~ISkeleton() {}

	virtual std::shared_ptr<ISkeletonBone> GetRootBone() const = 0;
	virtual std::shared_ptr<ISkeletonBone> GetBone(size_t Index) const = 0;
	virtual size_t GetBoneIndexByName(const std::string& BoneName) const = 0;
	virtual std::shared_ptr<ISkeletonBone> GetBoneByName(const std::string& BoneName) const = 0;
	virtual const std::vector<std::shared_ptr<ISkeletonBone>>& GetBones() const = 0;

	virtual const glm::mat4& GetRootBoneLocalTransform() const = 0;
};

ZN_INTERFACE ZN_API ISkeletonInternal
{
	virtual ~ISkeletonInternal() {}

	virtual void SetRootBoneLocalTranform(glm::mat4 RootBoneLocalTranform) = 0;
	virtual void AddBone(std::shared_ptr<ISkeletonBone> Bone) = 0;
};




ZN_INTERFACE ZN_API IAnimation
{
	virtual ~IAnimation() {}

	virtual const std::string& GetName() const = 0;
	virtual uint32 GetFrameStart() const = 0;
	virtual uint32 GetFrameEnd() const = 0;
	virtual uint16 GetIndexInSequences() const = 0;

	virtual std::shared_ptr<ISkeletonAnimation> GetSkeletonAnimation() const = 0;
};
typedef std::unordered_map<std::string, std::shared_ptr<IAnimation>> Animations_t;

ZN_INTERFACE ZN_API IAnimationInternal
{
	virtual ~IAnimationInternal() {}

	virtual void SetName(const std::string& Name) = 0;
	virtual void SetSkeletonAnimation(std::shared_ptr<ISkeletonAnimation> SkeletonAnimation) = 0;
};




ZN_INTERFACE ZN_API ISkeletonAnimation
{
	virtual ~ISkeletonAnimation() {}

	virtual glm::mat4 GetRootBoneMatrix() const = 0;
	virtual bool IsBoneAnimated(const std::string& BoneName, uint32 FrameIndex) const = 0;
	virtual glm::mat4 CalculateBoneMatrix(const std::string& BoneName, uint32 FrameIndex) const = 0;
};









/**
  * Модель представляет собой совокупность логически объединенной геометрии (например: геометрия ножен стола и геометрия столешницы),
  * материалы, используемые для отрисовки каждой геометрии (например: металл для ножен и дерево для столешницы)
  * и связь материала и геометрии между собой
  *
*/
ZN_INTERFACE ZN_API IModel 
	: public virtual IObject
    , public std::enable_shared_from_this<IModel>
{
	struct ZN_API SConnection
	{
		std::shared_ptr<IMaterial> Material;
		std::shared_ptr<IGeometry> Geometry;
		SGeometryDrawArgs          GeometryDrawArgs;
	};

	virtual ~IModel() {}


	virtual std::string                             GetFileName() const = 0;	
	virtual BoundingBox                             GetBounds() const = 0;

	// Add pair geometry[+GeometryPart] - Material
	virtual void									AddConnection(const std::shared_ptr<IMaterial>& Material, const std::shared_ptr<IGeometry>& Geometry, SGeometryDrawArgs GeometryDrawArgs = SGeometryDrawArgs()) = 0;
	virtual const std::vector<SConnection>&         GetConnections() const = 0;

	// Skeleton
	virtual const std::shared_ptr<ISkeleton>&       GetSkeleton() const = 0;

	// Animations
	virtual void                                    AddAnimation(const std::string& AnimationName, const std::shared_ptr<IAnimation>& Animation) = 0;
	virtual const Animations_t&                     GetAnimations() const = 0;

	virtual bool                                    Render() const = 0;

	// For IVisitor
	virtual void                                    Accept(IVisitor* visitor) = 0;
};


ZN_INTERFACE ZN_API IModelInternal
{
	virtual ~IModelInternal() {}

	virtual void SetFileName(const std::string& FileName) = 0;
	virtual void SetBounds(const BoundingBox& Bounds) = 0;
	virtual void SetSkeleton(std::shared_ptr<ISkeleton> Skeleton) = 0;
};

