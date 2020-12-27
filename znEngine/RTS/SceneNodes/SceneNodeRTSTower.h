#pragma once

class CSceneNodeRTSTower
	: public CSceneNode
	, public ISceneNodeRTSTower
{
public:
	CSceneNodeRTSTower(IScene& Scene);
	virtual ~CSceneNodeRTSTower();

	// ISceneNodeRTSTower 
	void SetAttackPoint(glm::vec3 AttackPoint);
	glm::vec3 GetAttackPoint() const;
	void SetAttackDamageMin(float AttackDamageMin) override;
	float GetAttackDamageMin() const override;
	void SetAttackDamageMax(float AttackDamageMax) override;
	float GetAttackDamageMax() const override;
	void SetAttackSpeed(float AttackSpeed) override;
	float GetAttackSpeed() const override;
	void SetAttackRange(float Value) override;
	float GetAttackRange() const override;

	// ISceneNode
	void Initialize() override;
	void Update(const UpdateEventArgs& e) override;

protected:
	float GetAttackIntervalMS() const;

private:
	std::shared_ptr<ISceneNodeRTSUnit> GetCurrentTarget();

private:
	std::shared_ptr<ISceneNodeRTSUnit> m_CurrentTarget;
	float m_LastAttackTime;

private:
	glm::vec3 m_AttackPoint;

	float m_AttackDamageMin;
	float m_AttackDamageMax;
	float m_AttackSpeed;
	float m_AttackRange;
};