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
void CParticlesComponent::AddParticleSystem(std::shared_ptr<IParticleSystem> ParticleSystem)
{
	const auto& it = std::find(m_ParticleSystems.begin(), m_ParticleSystems.end(), ParticleSystem);
	if (it != m_ParticleSystems.end())
		throw CException("Particle system already exists.");

	ParticleSystem->SetNode(&GetOwnerNode());
	m_ParticleSystems.push_back(ParticleSystem);

	// Add proxy properties
	auto particleSystemPropertiesProxy = MakeShared(CPropertyGroupProxy, ParticleSystem->GetProperties());
	GetProperties()->AddProperty(particleSystemPropertiesProxy);
}

void CParticlesComponent::RemoveParticleSystem(std::shared_ptr<IParticleSystem> ParticleSystem)
{
	auto it = std::find(m_ParticleSystems.begin(), m_ParticleSystems.end(), ParticleSystem);
	if (it == m_ParticleSystems.end())
		throw CException("Particle system don't found.");

	(*it)->SetNode(nullptr);
	m_ParticleSystems.erase(it);

	GetProperties()->RemoveProperty(ParticleSystem->GetProperties());
}

void CParticlesComponent::DeleteAllParticleSystem()
{
	for (const auto& particleSystem : GetParticleSystems())
		RemoveParticleSystem(particleSystem);
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
// IObjectLoadSave
//
void CParticlesComponent::CopyTo(std::shared_ptr<IObject> Destination) const
{
	auto destinationAsParticlesComponent = std::dynamic_pointer_cast<IParticleComponent3D>(Destination);

	if (destinationAsParticlesComponent->GetParticleSystems().empty())
	{
		Log::Warn("Destination particles component already contains partice systems. They will be erased.");
		destinationAsParticlesComponent->DeleteAllParticleSystem();
	}

	for (const auto& existingParticleSystem : GetParticleSystems())
	{
		auto copiedParticleSystem = MakeShared(CParticleSystem, GetBaseManager());
		existingParticleSystem->CopyTo(copiedParticleSystem);
		destinationAsParticlesComponent->AddParticleSystem(copiedParticleSystem);
	}
}

void CParticlesComponent::Load(const std::shared_ptr<IXMLReader>& Reader)
{
	auto particleSystemsXMLReader = Reader->GetChild("ParticleSystems");
	if (particleSystemsXMLReader == nullptr)
		throw CException("'ParticleSystems' xml node don't found.");

	for (const auto& particleSystemXMLReader : particleSystemsXMLReader->GetChilds())
	{
		if (particleSystemXMLReader->GetName() != "ParticleSystem")
			throw CException("Paricle system xml node has name '%s', but expected name is 'ParticleSystem'.", particleSystemXMLReader->GetName().c_str());

		std::shared_ptr<IParticleSystem> particleSystem = MakeShared(CParticleSystem, GetBaseManager());
		particleSystem->Load(particleSystemXMLReader);
		AddParticleSystem(particleSystem);
	}
}

void CParticlesComponent::Save(const std::shared_ptr<IXMLWriter>& Writer) const
{
	auto particleSystemsXMLWriter = Writer->CreateChild("ParticleSystems");
	for (const auto& particleSystem : GetParticleSystems())
	{
		auto particleSystemXMLWriter = particleSystemsXMLWriter->CreateChild("ParticleSystem");
		particleSystem->Save(particleSystemXMLWriter);
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
			GetProperties()->RemoveProperty(particleSystem->GetProperties());
			continue;
		}

		it++;
	}
}
