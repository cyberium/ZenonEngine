#pragma once



class ZN_API CAnimationComponent3D
	: public CComponentBase
	, public ISkeletonAnimationComponent
{
public:
	CAnimationComponent3D(const ISceneNode& Owner);
	virtual ~CAnimationComponent3D();

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