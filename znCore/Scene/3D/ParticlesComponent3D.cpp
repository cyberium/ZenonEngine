#include "stdafx.h"

// General
#include "ParticlesComponent3D.h"

CParticlesComponent3D::CParticlesComponent3D(const ISceneNode3D & SceneNode)
	: CComponentBase(SceneNode)
{
}

CParticlesComponent3D::~CParticlesComponent3D()
{
}


//
// IParticleSystem
//
void CParticlesComponent3D::AddParticle(const SParticle& Particle)
{
	m_Particles.push_back(Particle);
}

const std::vector<SParticle>& CParticlesComponent3D::GetParticles() const
{
	return m_Particles;
}



//
// ISceneNodeComponent
//
void CParticlesComponent3D::Update(const UpdateEventArgs & e)
{
	for (auto pIt = m_Particles.begin(); pIt != m_Particles.end(); )
	{
		auto& p = *pIt;
		if (glm::length(p.Position) < 5.0f)
		{
			//pIt = m_Particles.erase(pIt);
			//continue;
		}

		glm::vec3 diff = - glm::normalize(p.Position);
		diff *= e.DeltaTime;
		diff /= 10.0f;
		p.Position += diff;

		pIt++;
	}
}

void CParticlesComponent3D::Accept(IVisitor * visitor)
{
	visitor->Visit((const IParticleSystem*)this);
}
