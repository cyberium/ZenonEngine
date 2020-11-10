#pragma once

class ZN_API CParticlesComponent
	: public IParticleComponent3D
	, public IParticleSystem
	, public CComponentBase
{
public:
	CParticlesComponent(const ISceneNode& SceneNode);
	virtual ~CParticlesComponent();

	// IParticleComponent3D

	// IParticleSystem
	void AddParticle(const SParticle& Particle) override;
	void ClearParticles() override;
	const std::vector<SParticle>& GetParticles() const override;
	void SetTexture(const std::shared_ptr<ITexture>& Texture) override;
	std::shared_ptr<ITexture> GetTexture() const override;
	std::shared_ptr<IBlendState> GetBlendState() const override;

	// ISceneNodeComponent
	virtual void Update(const UpdateEventArgs& e) override;
	virtual void Accept(IVisitor* visitor) override;

private:
	std::vector<SParticle> m_Particles;
	std::shared_ptr<ITexture> m_Texture;
	float m_LastParticleTime;
	float m_ParticleInterval;
};