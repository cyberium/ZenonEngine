#pragma once

#include "Interfaces/Render/Types/RenderTypes.h"
#include "Interfaces/Render/ISkeleton.h"

// FORWARD BEGIN
ZN_INTERFACE IMaterial;
ZN_INTERFACE IGeometry;
class RenderEventArgs;
ZN_INTERFACE IModelsComponent3D;
// FORWARD END

struct ZN_API SSkeletonAnimation
{
	glm::mat4 RootBoneLocalTransform;
};

ZN_INTERFACE ZN_API ISkeletonBone
{
	virtual ~ISkeletonBone() {}

	virtual void MergeWithOther(std::shared_ptr<ISkeletonBone> OtherBone) = 0;

	virtual std::string GetName() const = 0;
	virtual int32 GetParentIndex() const = 0;
	
	virtual void SetLocalMatrix(const glm::mat4& Matrix) = 0;
	virtual glm::mat4 GetLocalMatrix() const = 0;

	virtual void SetPivotMatrix(const glm::mat4& Matrix) = 0;
	virtual glm::mat4 GetPivotMatrix() const = 0;

	virtual void SetFuckingMatrix(const glm::mat4& Matrix) = 0;
	virtual glm::mat4 GetFuckingMatrix() const = 0;

	virtual glm::mat4 CalcMatrix(const IModelsComponent3D* ModelsComponent) const = 0;
	virtual glm::mat4 CalcRotateMatrix(const IModelsComponent3D* ModelsComponent) const = 0;
};


ZN_INTERFACE ZN_API IAnimation
{
	virtual ~IAnimation() {}

	virtual const std::string& GetName() const = 0;
	virtual uint32 GetFrameStart() const = 0;
	virtual uint32 GetFrameEnd() const = 0;

	virtual uint16 GetIndexInSequences() const = 0; // Index in bones animated value
};


ZN_INTERFACE ZN_API IAnimationInternal
{
	virtual ~IAnimationInternal() {}

	virtual void SetName(const std::string& Name) = 0;
};


typedef std::unordered_map<std::string, std::shared_ptr<IAnimation>> Animations_t;

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

	virtual void                                    SetBounds(const BoundingBox& Bounds) = 0;
	virtual BoundingBox                             GetBounds() const = 0;

	virtual void                                    SetFileName(const std::string& FileName) = 0;
	virtual std::string                             GetFileName() const = 0;



	/**
	 * Add pair geometry[+GeometryPart] - Material
	*/
	virtual void									AddConnection(const std::shared_ptr<IMaterial>& Material, const std::shared_ptr<IGeometry>& Geometry, SGeometryDrawArgs GeometryDrawArgs = SGeometryDrawArgs()) = 0;
	virtual const std::vector<SConnection>&         GetConnections() const = 0;

	// Skeleton
	virtual void                                    AddSkeletonAnimation(std::shared_ptr<IModel> SkeletonAnimation) = 0;
	virtual const SSkeletonAnimation&               GetSkeletonAnimation(size_t Index) = 0;
	virtual const std::vector<SSkeletonAnimation>&  GetSkeletonAnimations() const = 0;

	virtual void                                    ApplyOtherSkeleton(std::shared_ptr<IModel> other) = 0;
	virtual void                                    AddBone(const std::shared_ptr<ISkeletonBone> Bone) = 0;
	virtual void                                    SetFixSkeleton(const glm::mat4& Matrix) = 0;
	virtual glm::mat4                               GetFixSkeleton() const = 0;
	virtual std::shared_ptr<ISkeletonBone>          GetRootBone() const = 0;
	virtual std::shared_ptr<ISkeletonBone>          GetBone(size_t Index) const = 0;
	virtual size_t                                  GetBoneIndexByName(const std::string& BoneName) const = 0;
	virtual std::shared_ptr<ISkeletonBone>          GetBoneByName(const std::string& BoneName) const = 0;
	virtual const std::vector<std::shared_ptr<ISkeletonBone>>& GetBones() const = 0;

	// Animations
	virtual void                                    AddAnimation(const std::string& AnimationName, const std::shared_ptr<IAnimation>& Animation) = 0;
	virtual const Animations_t&                     GetAnimations() const = 0;

	virtual bool                                    Render() const = 0;

	// For IVisitor

	virtual void                                    Accept(IVisitor* visitor) = 0;
};
typedef std::shared_ptr<IModel> IModelPtr;


ZN_INTERFACE ZN_API IModelInternal
{
	virtual ~IModelInternal() {}

	virtual void AddSkeletonAnimationInternal(const SSkeletonAnimation& SkeletonAnimation) = 0;
};

