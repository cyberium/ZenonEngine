#pragma once

class ZN_API CModelComponent
	: public IModelComponent
	, public CComponentBase
{
public:
	CModelComponent(const ISceneNode& OwnerNode);
    virtual ~CModelComponent();

	// IModelComponent
    void SetModel(const std::shared_ptr<IModel>& Model) override;
	void ResetModel() override;
    std::shared_ptr<IModel> GetModel() const override;
	void SetCastShadows(bool Value) override;
	bool IsCastShadows() const  override;

	// Skeleton functional
	std::shared_ptr<ISkeletonComponentBone3D> GetRootBone() const override;
	std::shared_ptr<ISkeletonComponentBone3D> GetBone(size_t Index) const override;
	const std::vector<std::shared_ptr<ISkeletonComponentBone3D>>& GetBones() const override;
	std::shared_ptr<IStructuredBuffer> GetBonesBuffer() const override;
	std::vector<glm::mat4> CreatePose(size_t BoneStartIndex = 0, size_t BonesCount = 0) const override;

	// Animation functional
	void PlayAnimation(const std::string& AnimationName, bool Loop) override;
	void SetAnimationEndedCallback(std::function<void(const IAnimation*)> Func) override;
	bool IsAnimationPlayed() const override;
	size_t GetCurrentAnimationIndex() const override;
	uint32 GetCurrentAnimationFrame() const override;

	// CComponentBase
	void Update(const UpdateEventArgs& e) override;
    void Accept(IVisitor* visitor) override;

	// IObjectLoadSave
	void CopyTo(std::shared_ptr<IObject> Destination) const override;
	void Load(const std::shared_ptr<IXMLReader>& Reader) override;
	void Save(const std::shared_ptr<IXMLWriter>& Writer) const override;

protected:
	virtual void InitializeBones();
	virtual void AddBone(std::shared_ptr<ISkeletonComponentBone3D> Bone);

	void ResetBones();
	void PauseAnimation();
	void ResetAnimation();

private:
	std::shared_ptr<IModel> m_Model;
	bool m_IsCastShadows;

	// Bones
	std::shared_ptr<ISkeletonComponentBone3D> m_RootBone;
	std::vector<std::shared_ptr<ISkeletonComponentBone3D>> m_Bones;
	std::vector<glm::mat4> m_BonesList;
	std::shared_ptr<IStructuredBuffer> m_StructuredBuffer;

	// Animation
	uint16                      m_CurrentAnimationIndex;
	const IAnimation*		    m_CurrentAnimation;
	bool						m_IsAnimationLooped;
	bool                        m_IsAnimationPaused;
	double						m_CurrentTime;
	std::function<void(const IAnimation*)> m_OnAnimationEnded;
};