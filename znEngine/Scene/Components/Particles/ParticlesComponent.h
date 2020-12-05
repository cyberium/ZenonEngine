#pragma once

class ZN_API CParticlesComponent
	: public IParticleComponent3D
	, public CComponentBase
{
public:
	CParticlesComponent(const ISceneNode& SceneNode);
	virtual ~CParticlesComponent();

	// IParticleComponent3D
	void Attach(std::shared_ptr<IParticleSystem> ParticleSystem) override;
	std::shared_ptr<IParticleSystem> Detach(std::shared_ptr<IParticleSystem> ParticleSystem) override;
	const std::vector<std::shared_ptr<IParticleSystem>>& GetParticleSystems() const override;

	// ISceneNodeComponent
	virtual void Update(const UpdateEventArgs& e) override;
	virtual void Accept(IVisitor* visitor) override;

private:
	void ClearUnusedParticlesSystem();

private:
	std::vector<std::shared_ptr<IParticleSystem>> m_ParticleSystems;
};