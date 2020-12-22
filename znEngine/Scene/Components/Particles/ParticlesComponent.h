#pragma once

class ZN_API CParticlesComponent
	: public IParticleComponent3D
	, public CComponentBase
{
public:
	CParticlesComponent(const ISceneNode& SceneNode);
	virtual ~CParticlesComponent();

	// IParticleComponent3D
	void AddParticleSystem(std::shared_ptr<IParticleSystem> ParticleSystem) override;
	void RemoveParticleSystem(std::shared_ptr<IParticleSystem> ParticleSystem) override;
	void DeleteAllParticleSystem() override;
	const std::vector<std::shared_ptr<IParticleSystem>>& GetParticleSystems() const override;

	// ISceneNodeComponent
	void Update(const UpdateEventArgs& e) override;
	void Accept(IVisitor* visitor) override;

	// IObjectLoadSave
	void CopyTo(std::shared_ptr<IObject> Destination) const override;
	void Load(const std::shared_ptr<IXMLReader>& Reader) override;
	void Save(const std::shared_ptr<IXMLWriter>& Writer) const override;

private:
	void ClearUnusedParticlesSystem();

private:
	std::vector<std::shared_ptr<IParticleSystem>> m_ParticleSystems;
};