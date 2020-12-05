#include "stdafx.h"

// General
#include "ParticlesComponent.h"

// Additional
#include "ParticleSystem.h"

CParticlesComponent::CParticlesComponent(const ISceneNode & SceneNode)
	: CComponentBase(SceneNode)

{
	GetProperties()->SetName("ParticlesComponent");
}

CParticlesComponent::~CParticlesComponent()
{
}



//
// IParticleComponent3D
//
void CParticlesComponent::Attach(std::shared_ptr<IParticleSystem> ParticleSystem)
{
	auto it = std::find(m_ParticleSystems.begin(), m_ParticleSystems.end(), ParticleSystem);
	if (it != m_ParticleSystems.end())
		throw CException("Particle system already exists.");

	m_ParticleSystems.push_back(ParticleSystem);
}

std::shared_ptr<IParticleSystem> CParticlesComponent::Detach(std::shared_ptr<IParticleSystem> ParticleSystem)
{
	auto it = std::find(m_ParticleSystems.begin(), m_ParticleSystems.end(), ParticleSystem);
	if (it == m_ParticleSystems.end())
		return nullptr;

	auto particleSystem = *it;
	m_ParticleSystems.erase(it);
	return particleSystem;
}

const std::vector<std::shared_ptr<IParticleSystem>>& CParticlesComponent::GetParticleSystems() const
{
	return m_ParticleSystems;
}




//
// ISceneNodeComponent
//
void CParticlesComponent::Update(const UpdateEventArgs & e)
{
	__super::Update(e);

	ClearUnusedParticlesSystem();

	for (const auto& particleSystem : m_ParticleSystems)
	{
		_ASSERT(particleSystem != nullptr);
		particleSystem->Update(e);
	}
}

void CParticlesComponent::Accept(IVisitor * visitor)
{
	for (const auto& particleSystem : m_ParticleSystems)
	{
		_ASSERT(particleSystem != nullptr);
		visitor->Visit(particleSystem);
	}	
}



//
// Private
//
void CParticlesComponent::ClearUnusedParticlesSystem()
{
	for (auto it = m_ParticleSystems.begin(); it != m_ParticleSystems.end(); )
	{
		const auto& particleSystem = *it;
		if (false == particleSystem->IsEnableCreatingNewParticles() && particleSystem->GetGPUParticles().empty())
		{
			it = m_ParticleSystems.erase(it);
			continue;
		}

		it++;
	}
}
