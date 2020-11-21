#pragma once

class CSceneNodeRTSTower
	: public CSceneNode
	, public ISceneNodeRTSTower
{
public:
	CSceneNodeRTSTower(IScene& Scene);
	virtual ~CSceneNodeRTSTower();

	// ISceneNodeRTSTower 
	void SetAttackDamage(float Value) override;
	float GetAttackDamage() const override;
	void SetAttackInterval(float Value) override;
	float GetAttackInterval() const override;
	void SetAttackRange(float Value) override;
	float GetAttackRange() const override;

	// ISceneNode
	void Initialize() override;
	void Update(const UpdateEventArgs& e) override;

private:
	float m_LastAttackTime;

private:
	float m_AttackDamage;
	float m_AttackInterval;
	float m_AttackRange;
	std::shared_ptr<IModel> m_MissileModel;
};