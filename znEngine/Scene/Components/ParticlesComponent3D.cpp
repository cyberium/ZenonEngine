#include "stdafx.h"

// General
#include "ParticlesComponent3D.h"

CParticlesComponent3D::CParticlesComponent3D(const ISceneNode & SceneNode)
	: CComponentBase(SceneNode)
{
	GetProperties()->SetName("ParticlesComponent");
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

void CParticlesComponent3D::ClearParticles()
{
	m_Particles.clear();
}

const std::vector<SParticle>& CParticlesComponent3D::GetParticles() const
{
	return m_Particles;
}

void CParticlesComponent3D::SetMaterial(const std::shared_ptr<IMaterial>& Material)
{
	m_Material = Material;
}

std::shared_ptr<IMaterial> CParticlesComponent3D::GetMaterial() const
{
	return m_Material;
}

std::shared_ptr<IBlendState> CParticlesComponent3D::GetBlendState() const
{
	return std::shared_ptr<IBlendState>();
}



//
// ISceneNodeComponent
//
void CParticlesComponent3D::Update(const UpdateEventArgs & e)
{
	/*for (auto pIt = m_Particles.begin(); pIt != m_Particles.end(); )
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
	}*/
}

void CParticlesComponent3D::Accept(IVisitor * visitor)
{
	visitor->Visit((const IParticleSystem*)this);
}
