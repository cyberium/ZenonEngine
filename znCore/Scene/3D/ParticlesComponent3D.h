#pragma once

#include "../ComponentBase.h"

class __declspec(UUID_ParticleComponent) ZN_API CParticlesComponent3D
	: public IParticleComponent3D
	, public IParticleSystem
	, public CComponentBase
{
public:
	CParticlesComponent3D(const ISceneNode3D& SceneNode);
	virtual ~CParticlesComponent3D();

	// IParticleComponent3D

	// IParticleSystem
	void AddParticle(const SParticle& Particle) override;
	const std::vector<SParticle>& GetParticles() const override;

	// ISceneNodeComponent
	virtual void Update(const UpdateEventArgs& e) override;
	virtual void Accept(IVisitor* visitor) override;

private:
	std::vector<SParticle> m_Particles;
};