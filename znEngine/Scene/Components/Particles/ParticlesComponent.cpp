#include "stdafx.h"

// General
#include "ParticlesComponent.h"

// Additional
#include "ParticleSystem.h"

CParticlesComponent::CParticlesComponent(const ISceneNode & SceneNode)
	: CComponentBase(SceneNode)

{
	GetProperties()->SetName("ParticlesComponent");

	m_ParticleSystem = MakeShared(CParticleSystem, GetOwnerNode());
	m_ParticleSystem->SetTexture(GetBaseManager().GetManager<IznTexturesFactory>()->LoadTexture2D("Particles/smoke_05.png"));
}

CParticlesComponent::~CParticlesComponent()
{
}



//
// IParticleComponent3D
//
const std::shared_ptr<IParticleSystem>& CParticlesComponent::GetParticleSystem() const
{
	return m_ParticleSystem;
}



//
// ISceneNodeComponent
//
void CParticlesComponent::Update(const UpdateEventArgs & e)
{
	__super::Update(e);

	_ASSERT(m_ParticleSystem);
	m_ParticleSystem->Update(e);
}

void CParticlesComponent::Accept(IVisitor * visitor)
{
	_ASSERT(m_ParticleSystem);
	visitor->Visit(m_ParticleSystem.get());
}
