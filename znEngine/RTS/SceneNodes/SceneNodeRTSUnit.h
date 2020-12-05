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
	void SetLastPathPointReached(std::function<void(const ISceneNodeRTSUnit* Unit)> Func);
	void DealDamage(float Damage) override;
	bool IsDead() override;

	// ISceneNode
	void Initialize() override;
	void Update(const UpdateEventArgs& e) override;

protected:
	void OnDeath();

private:
	// Stats
	float m_Health;
	float m_MaxHealth;
	float m_Speed;

	// Path
	std::shared_ptr<ISceneNodeRTSPath> m_Path;
	std::function<void(const ISceneNodeRTSUnit* Unit)> m_OnLastPathPointReached;
	size_t m_PathCurrentPoint;

	// Death
	bool m_IsNeedMoveDown;
	float m_MoveDownHeight;
};