#pragma once

class ZN_API CParticleSystem
	: public IParticleSystem
{
public:
	CParticleSystem(const ISceneNode& SceneNode);
	virtual ~CParticleSystem();

	// IParticleSystem
	void Update(const UpdateEventArgs& e) override;

	const std::vector<SGPUParticle>& GetGPUParticles() const override;

	void SetEnableCreatingNewParticles(bool Value) override;
	bool IsEnableCreatingNewParticles() const override;

	void SetTexture(const std::shared_ptr<ITexture>& Texture) override;
	std::shared_ptr<ITexture> GetTexture() const override;

	std::shared_ptr<IBlendState> GetBlendState() const override;

protected:
	void CreateNewParticle();
	void UpdateParticle(SCPUParticle& CPUParticle, const UpdateEventArgs & e);

public:
	float m_Lifetime;
	float m_LifeTimeMiddlePoint;

	glm::vec4 m_Colors[3];
	glm::vec2 m_Sizes[3];
	
private:
	std::vector<SCPUParticle> m_CPUParticles;
	std::vector<SGPUParticle> m_GPUParticles;

	bool m_IsEnableCreatingNewParticles;
	std::shared_ptr<ITexture> m_Texture;

	glm::vec3 m_GravityDirection;
	float m_GravityValue;

	float m_Deaccelerate;

	float m_LastParticleTime;
	float m_ParticleInterval;

private:
	const ISceneNode& m_OwnerNode;
};