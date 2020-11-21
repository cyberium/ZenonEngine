#pragma once

class CSceneNodeRTSUnit
	: public CSceneNode
	, public ISceneNodeRTSUnit
{
public:
	CSceneNodeRTSUnit(IScene& Scene);
	virtual ~CSceneNodeRTSUnit();

	// ISceneNodeRTSUnit 
	void SetHealth(float Health) override;
	float GetHealth() const override;
	void SetMaxHealth(float MaxHealth) override;
	float GetMaxHealth() const override;
	void SetMovementSpeed(float Speed) override;
	float GetMovementSpeed() const override;

	void SetPath(std::shared_ptr<ISceneNodeRTSPath> Path) override;
	void DealDamage(float Damage) override;

	// ISceneNode
	void Initialize() override;
	void Update(const UpdateEventArgs& e) override;

private:
	float m_Health;
	float m_MaxHealth;
	float m_Speed;
	std::shared_ptr<ISceneNodeRTSPath> m_Path;
	size_t m_PathCurrentPoint;
};