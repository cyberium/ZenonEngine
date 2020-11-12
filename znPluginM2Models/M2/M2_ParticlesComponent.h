#pragma once

class CM2;
class CM2_Base_Instance;
#include "M2/M2_ParticleSystem.h"

//
// CM2ParticleSystem
//
class CM2ParticleSystem
	: public IParticleSystem
{
public:
	CM2ParticleSystem(IRenderDevice& RenderDevice, const std::shared_ptr<SM2_ParticleSystem_Wrapper>& M2ParticleSystem);
	virtual ~CM2ParticleSystem();

	// CM2ParticleSystem
	void Update(const CM2_Base_Instance* M2Instance, const UpdateEventArgs& e);

	// IParticleSystem
	void Update(const UpdateEventArgs& e) override;
	const std::vector<SGPUParticle>& GetGPUParticles() const override;
	void SetTexture(const std::shared_ptr<ITexture>& Texture) override;
	std::shared_ptr<ITexture> GetTexture() const override;
	std::shared_ptr<IBlendState> GetBlendState() const override;

private:
	std::vector<SGPUParticle> m_ParticleObjects;
	std::shared_ptr<ITexture> m_Texture;
	std::shared_ptr<IBlendState> m_BlendState;

private:
	float rem;
	std::shared_ptr<SM2_ParticleSystem_Wrapper> m_M2ParticleSystem;
	CM2_ParticleObject m_M2ParticleObjects[MAX_PARTICLES];
};



//
// CM2ParticlesComponent3D
//
class CM2ParticlesComponent3D
	: public IParticleComponent3D
	, public CComponentBase
{
public:
	CM2ParticlesComponent3D(const CM2_Base_Instance& SceneNode);
	virtual ~CM2ParticlesComponent3D();

	// ISceneNodeComponent
	void Update(const UpdateEventArgs& e) override final;
	void Accept(IVisitor* visitor) override final;
	const std::shared_ptr<IParticleSystem>& GetParticleSystem() const override;

protected:
	const CM2_Base_Instance& GetM2OwnerNode() const;

private:
	std::vector<std::shared_ptr<CM2ParticleSystem>> m_ParticleSystems;
	std::shared_ptr<IParticleSystem> m_ParticleSystem;
};