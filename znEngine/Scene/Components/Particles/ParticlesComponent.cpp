#include "stdafx.h"

// General
#include "ParticlesComponent.h"

CParticlesComponent::CParticlesComponent(const ISceneNode & SceneNode)
	: CComponentBase(SceneNode)
	, m_LastParticleTime(0.0f)
	, m_ParticleInterval(256.0f)
{
	GetProperties()->SetName("ParticlesComponent");
}

CParticlesComponent::~CParticlesComponent()
{
}


//
// IParticleSystem
//
void CParticlesComponent::AddParticle(const SParticle& Particle)
{
	m_Particles.push_back(Particle);
}

void CParticlesComponent::ClearParticles()
{
	m_Particles.clear();
}

const std::vector<SParticle>& CParticlesComponent::GetParticles() const
{
	return m_Particles;
}

void CParticlesComponent::SetTexture(const std::shared_ptr<ITexture>& Material)
{
	m_Texture = Material;
}

std::shared_ptr<ITexture> CParticlesComponent::GetTexture() const
{
	return m_Texture;
}

std::shared_ptr<IBlendState> CParticlesComponent::GetBlendState() const
{
	return std::shared_ptr<IBlendState>();
}



//
// ISceneNodeComponent
//
void CParticlesComponent::Update(const UpdateEventArgs & e)
{
	__super::Update(e);

	
	for (auto pIt = m_Particles.begin(); pIt != m_Particles.end(); )
	{
		auto& p = *pIt;
		if (glm::length(p.Position) < 1.0f)
		{
			pIt = m_Particles.erase(pIt);
			continue;
		}

		glm::vec3 diff = -glm::normalize(p.Position);
		diff *= e.DeltaTime;
		diff /= 100.0f;
		p.Position += diff;

		pIt++;
	}

	if (m_LastParticleTime + m_ParticleInterval > e.TotalTime)
		return;

	float areaSize = 35.0f;
	Random r(time(0));
	for (size_t i = 0; i < 25; i++)
	{
		SParticle particle;
		particle.Position = r.NextVec3f() * areaSize - glm::vec3(areaSize) / 2.0f;
		float size = r.NextFloat() * 1.0f + 1.0f;
		particle.Size = glm::vec2(size, size);
		particle.Color = glm::vec4(r.NextVec3f(), 1.0f);
		AddParticle(particle);
	}

	m_LastParticleTime = e.TotalTime;
}

void CParticlesComponent::Accept(IVisitor * visitor)
{
	visitor->Visit((const IParticleSystem*)this);
}
