#pragma once

class ZN_API CParticlesComponent
	: public IParticleComponent3D
	, public CComponentBase
{
public:
	CParticlesComponent(const ISceneNode& SceneNode);
	virtual ~CParticlesComponent();

	// IParticleComponent3D
	const std::shared_ptr<IParticleSystem>& GetParticleSystem() const override;

	// ISceneNodeComponent
	virtual void Update(const UpdateEventArgs& e) override;
	virtual void Accept(IVisitor* visitor) override;

private:
	std::shared_ptr<IParticleSystem> m_ParticleSystem;
};