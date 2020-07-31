#pragma once



class __declspec(UUID_SkeletonAnimationComponent) CAnimatorComponent3D
	: public CComponentBase
	, public ISkeletonAnimationComponent
{
public:
	static ObjectClass GetClassT() { return UINT16_MAX - 559u; }

public:
	CAnimatorComponent3D(const ISceneNode3D& Owner);
	virtual ~CAnimatorComponent3D();

	void AddAnimation(uint16 AnimationId, const SAnimation& Animation);
	void PlayAnimation(uint16 AnimationId, bool Loop);
	uint16 getSequenceIndex() const override;
	uint32 getCurrentTime() const override;

	// CComponentBase
	void Update(const UpdateEventArgs& e) override;

private:
	std::unordered_map<uint16, SAnimation> m_Animations;
	uint16                      m_CurrentAnimationIndex;
	const SAnimation*		    m_CurrentAnimation;
	bool						m_IsLoop;
	bool						m_IsStopped;
	double						animtime;
	uint32						m_CurrentTime;
};