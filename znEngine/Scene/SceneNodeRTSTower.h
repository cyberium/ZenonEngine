#pragma once

#include "EngineSceneIntfs.h"


//
// CSceneNodeRTSPoint
//
class CSceneNodeRTSBullet
	: public CSceneNode
	, public ISceneNodeRTSBullet
{
public:
	CSceneNodeRTSBullet(IScene& Scene);
	virtual ~CSceneNodeRTSBullet();

	// ISceneNodeRTSBullet
	void SetTarget(std::shared_ptr<ISceneNodeRTSUnit> Target) override;
	std::shared_ptr<ISceneNode> GetTarget() const override;
	void SetDamage(float Damage) override;
	float GetDamage() const override;
	void SetSpeed(float Speed) override;
	float GetSpeed() const override;

	// ISceneNode
	void Initialize() override;
	void Update(const UpdateEventArgs& e) override;

private:
	glm::vec3 GetDestinationPoint() const;

private:
	std::shared_ptr<ISceneNodeRTSUnit> m_Target;
	float m_Damage;
	float m_Speed;
};



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