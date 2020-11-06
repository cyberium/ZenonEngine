#pragma once

#include "EngineSceneIntfs.h"

class CSceneNodeRTSUnit
	: public CSceneNode
	, public ISceneNodeRTSUnit
{
public:
	CSceneNodeRTSUnit(IScene& Scene);
	virtual ~CSceneNodeRTSUnit();

	// ISceneNodeRTSUnit 
	void SetHealth(float Health) override;
	void SetMaxHealth(float MaxHealth) override;
	float GetHealth() const override;
	void SetPath(std::shared_ptr<ISceneNodeRTSPath> Path) override;
	void SetSpeed(float Speed) override;
	float GetSpeed() const override;

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