#include "stdafx.h"

// Include
#include "M2.h"
#include "M2_Base_Instance.h"

// General
#include "M2_ParticlesComponent.h"

//
// CM2ParticleSystem
//
CM2ParticleSystem::CM2ParticleSystem(IRenderDevice& RenderDevice, const std::shared_ptr<SM2_ParticleSystem_Wrapper>& M2ParticleSystem)
	: m_M2ParticleSystem(M2ParticleSystem)
	, rem(0.0f)
{
	if (m_M2ParticleSystem->GetTexture())
	{
	}

	m_BlendState = RenderDevice.GetObjectsFactory().CreateBlendState();
	m_BlendState->SetBlendMode(m_M2ParticleSystem->GetBlendMode());
}

CM2ParticleSystem::~CM2ParticleSystem()
{
}



//
// CM2ParticleSystem
//
void CM2ParticleSystem::Update(const CM2_Base_Instance * M2Instance, const UpdateEventArgs & e)
{
	m_M2ParticleSystem->update(M2Instance, e, &rem, m_M2ParticleObjects);

	m_ParticleObjects.clear();
	for (size_t i = 0; i < 100; i++)
	{
		const auto& m2P = m_M2ParticleObjects[i];
		if (!m2P.Active)
			continue;

		SGPUParticle particle;
		particle.Position = m2P.pos;
		_ASSERT(m2P.tile < m_M2ParticleSystem->GetTiles().size());
		particle.TexCoordBegin = m_M2ParticleSystem->GetTiles()[m2P.tile].tc[1];
		particle.TexCoordEnd = m_M2ParticleSystem->GetTiles()[m2P.tile].tc[3];
		particle.Color = m2P.color;
		particle.Size = glm::vec2(m2P.size);
		m_ParticleObjects.push_back(particle);
	}
}



//
// IParticleSystem
//
void CM2ParticleSystem::Update(const UpdateEventArgs & e)
{
}

const std::vector<SGPUParticle>& CM2ParticleSystem::GetGPUParticles() const
{
	return m_ParticleObjects;
}

void CM2ParticleSystem::SetTexture(const std::shared_ptr<ITexture>& Texture)
{
	_ASSERT(false);
}

std::shared_ptr<ITexture> CM2ParticleSystem::GetTexture() const
{
	return m_Texture;
}

std::shared_ptr<IBlendState> CM2ParticleSystem::GetBlendState() const
{
	return m_BlendState;
}



//
// CM2ParticlesComponent3D
//
CM2ParticlesComponent3D::CM2ParticlesComponent3D(const CM2_Base_Instance& SceneNode)
	: CComponentBase(SceneNode)
{

	//for (size_t i = 0; i < 1; i++)
	//{
	//	const auto& p = GetM2OwnerNode().getM2().getMiscellaneous().GetParticles().at(i);
	//	m_ParticleSystems.push_back(std::make_shared<CM2ParticleSystem>(GetM2OwnerNode().getM2().GetRenderDevice(), p));
	//}

	for (const auto& m2ParticleSystem : GetM2OwnerNode().getM2().getMiscellaneous().GetParticles())
	{
		m_ParticleSystems.push_back(std::make_shared<CM2ParticleSystem>(GetM2OwnerNode().getM2().GetRenderDevice(), m2ParticleSystem));
	}
}

CM2ParticlesComponent3D::~CM2ParticlesComponent3D()
{
}

//
// ISceneNodeComponent
//
void CM2ParticlesComponent3D::Update(const UpdateEventArgs& e)
{
	for (auto& it : m_ParticleSystems)
	{
		it->Update(&GetM2OwnerNode(), e);
	}
}

void CM2ParticlesComponent3D::Accept(IVisitor * visitor)
{
	for (auto& it : m_ParticleSystems)
	{
		visitor->Visit((const IParticleSystem*)it.get());
	}
}

const std::shared_ptr<IParticleSystem>& CM2ParticlesComponent3D::GetParticleSystem() const
{
	return m_ParticleSystem;
}



//
// Protected
//
const CM2_Base_Instance& CM2ParticlesComponent3D::GetM2OwnerNode() const
{
	return dynamic_cast<const CM2_Base_Instance&>(GetOwnerNode());
}
