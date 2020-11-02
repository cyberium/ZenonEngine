#pragma once

class __declspec(UUID_ParticleComponent) ZN_API CParticlesComponent3D
	: public IParticleComponent3D
	, public IParticleSystem
	, public CComponentBase
{
public:
	CParticlesComponent3D(const ISceneNode& SceneNode);
	virtual ~CParticlesComponent3D();

	// IParticleComponent3D

	// IParticleSystem
	void AddParticle(const SParticle& Particle) override;
	void ClearParticles() override;
	const std::vector<SParticle>& GetParticles() const override;
	void SetMaterial(const std::shared_ptr<IMaterial>& Material) override;
	std::shared_ptr<IMaterial> GetMaterial() const override;
	std::shared_ptr<IBlendState> GetBlendState() const override;

	// ISceneNodeComponent
	virtual void Update(const UpdateEventArgs& e) override;
	virtual void Accept(IVisitor* visitor) override;

private:
	std::vector<SParticle> m_Particles;
	std::shared_ptr<IMaterial> m_Material;
};