#pragma once

// FORWARD BEGIN
ZN_INTERFACE ISceneNodeComponent;
ZN_INTERFACE IModel;
ZN_INTERFACE IStructuredBuffer;
ZN_INTERFACE IAnimation;
// FORWARD END


struct SBoneInstance
{
	SBoneInstance()
		: Bone(nullptr)
		, IsCalculated(false)
		, BoneMatrix(glm::mat4(1.0f))
	{}

	const ISkeletonBone* Bone;
	bool IsCalculated;
	glm::mat4 BoneMatrix;
};


ZN_INTERFACE ZN_API IModelComponent
{
	ZN_OBJECTCLASS(cSceneNodeModelsComponent);

	virtual ~IModelComponent() {}

	virtual void SetModel(const std::shared_ptr<IModel>& Model) = 0;
	virtual void ResetModel() = 0;
	virtual std::shared_ptr<IModel> GetModel() const = 0;

	virtual void SetCastShadows(bool Value) = 0;
	virtual bool IsCastShadows() const = 0;

	// Bones functional
	//virtual std::shared_ptr<ISkeletonComponentBone3D> GetRootBone() const = 0;
	virtual const SBoneInstance& GetCalculatedBone(size_t Index) const = 0;
	virtual const std::vector<SBoneInstance>& GetCalculatedBones() const = 0;
	virtual std::shared_ptr<IStructuredBuffer> GetBonesSkinBuffer() const = 0;
	virtual void CreatePose(size_t BoneStartIndex = 0, size_t BonesCount = 0) = 0;

	// Animation functional
	virtual void PlayAnimation(const std::string& AnimationName, bool Loop) = 0;
	virtual void SetAnimationEndedCallback(std::function<void(const IAnimation*)> Func) = 0;
	virtual bool IsAnimationPlayed() const = 0;
	virtual const IAnimation* GetCurrentAnimation() const = 0;
	virtual size_t GetCurrentAnimationIndex() const = 0;
	virtual uint32 GetCurrentAnimationFrame() const = 0;
};
const ComponentMessageType UUID_OnModelSetted = 30;