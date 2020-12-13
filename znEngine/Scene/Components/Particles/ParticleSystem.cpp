#include "stdafx.h"

// General
#include "ParticleSystem.h"

// Additional
#include "Scene/Components/Animations/AnimatedValueSingle.h" // For interpolations
namespace
{
	glm::mat4 CalcSpreadMatrix(float Spread1, float Spread2, float w, float l)
	{
		float a[2], c[2], s[2];

		a[0] = Random::Range(-Spread1, Spread1) / 2.0f;
		a[1] = Random::Range(-Spread2, Spread2) / 2.0f;

		for (size_t i = 0; i < 2; i++)
		{
			c[i] = glm::cos(a[i]);
			s[i] = glm::sin(a[i]);
		}

		glm::mat4 Temp(1.0f);
		Temp[1][1] = c[0];
		Temp[2][1] = s[0];
		Temp[2][2] = c[0];
		Temp[1][2] = -s[0];

		glm::mat4 SpreadMat = Temp;

		Temp = glm::mat4(1.0f);
		Temp[0][0] = c[1];
		Temp[1][0] = s[1];
		Temp[1][1] = c[1];
		Temp[0][1] = -s[1];

		SpreadMat *= Temp;

		float Size = glm::abs(c[0]) * l + glm::abs(s[0]) * w;
		for (size_t i = 0; i < 3; i++)
			for (size_t j = 0; j < 3; j++)
				SpreadMat[i][j] *= Size;

		return SpreadMat;
	}

	template<class T>
	inline T InterpolateParticleValue(float currentTime, float mid, const T& a, const T& b, const T& c)
	{
		if (currentTime <= mid)
			return interpolateLinear<T>(currentTime / mid, a, b);
		else
			return interpolateLinear<T>((currentTime - mid) / (1.0f - mid), b, c);
	}
}



CParticleSystem::CParticleSystem(const IBaseManager& BaseManager)
	: m_BaseManager(BaseManager)

	, m_IsEnableCreatingNewParticles(true)
	, m_Texture(nullptr)

	, m_GravityDirection(glm::vec3(0.0f, -1.0f, 0.0f))
	, m_GravityPowerSEC(0.0f)

	, m_DeaccelerateSEC(1.2f)

	, m_LastParticleTime(0.0f)
	, m_EmmiterSpawnIntervalMS(15.0f)

	, m_OwnerNode(nullptr)
{
	m_LifetimeMS = 700.0f;
	m_LifetimeMiddlePoint = 0.5f;

	m_Colors[0] = glm::vec4(1.0f, 0.7f, 0.3f, 1.0f);
	m_Colors[1] = glm::vec4(0.7f, 1.0f, 0.1f, 1.0f);
	m_Colors[2] = glm::vec4(0.3f, 0.2f, 0.7f, 1.0f);

	m_Sizes[0] = glm::vec2(4.0f);
	m_Sizes[1] = glm::vec2(2.0f);
	m_Sizes[2] = glm::vec2(0.1f);
}

CParticleSystem::~CParticleSystem()
{
}



//
// IParticleSystem
//
void CParticleSystem::Update(const UpdateEventArgs & e)
{
	for (auto pIt = m_Particles.begin(); pIt != m_Particles.end(); )
	{
		auto& p = *pIt;
		if (p.CurrentLifeTimeMS > p.MaxLifeTimeMS)
		{
			pIt = m_Particles.erase(pIt);
			continue;
		}

		p.CurrentLifeTimeMS += e.DeltaTime;
		UpdateParticle(p, e);
		pIt++;
	}

	m_GPUParticles.resize(m_Particles.size());
	for (size_t i = 0; i < m_Particles.size(); i++)
		m_GPUParticles[i] = m_Particles.at(i).ToGPUParticle();

	if (IsEnableCreatingNewParticles() && (m_LastParticleTime + m_EmmiterSpawnIntervalMS < e.TotalTime))
	{
		CreateNewParticle();
		m_LastParticleTime = e.TotalTime;
	}
}

void CParticleSystem::SetNode(const ISceneNode* Node)
{
	m_OwnerNode = Node;
}

const ISceneNode* CParticleSystem::GetNode() const
{
	return m_OwnerNode;
}

const std::vector<SGPUParticle>& CParticleSystem::GetGPUParticles() const
{
	return m_GPUParticles;
}

void CParticleSystem::SetEnableCreatingNewParticles(bool Value)
{
	m_IsEnableCreatingNewParticles = Value;
}

bool CParticleSystem::IsEnableCreatingNewParticles() const
{
	return m_IsEnableCreatingNewParticles;
}

void CParticleSystem::SetTexture(const std::shared_ptr<ITexture>& Material)
{
	m_Texture = Material;
}

std::shared_ptr<ITexture> CParticleSystem::GetTexture() const
{
	return m_Texture;
}


//
// IObjectLoadSave
//
void CParticleSystem::Load(const std::shared_ptr<IXMLReader>& Reader)
{
	auto lifeTimeXMLReader = Reader->GetChild("LifeTime");
	m_LifetimeMS = lifeTimeXMLReader->GetFloat();

	auto lifeTimeMiddlePointXMLReader = Reader->GetChild("LifeTimeMiddlePoint");
	m_LifetimeMiddlePoint = lifeTimeMiddlePointXMLReader->GetFloat();

	auto colorsXMLReader = Reader->GetChild("Colors");
	m_Colors[0] = colorsXMLReader->GetChild("Start")->GetVec4();
	m_Colors[1] = colorsXMLReader->GetChild("Middle")->GetVec4();
	m_Colors[1] = colorsXMLReader->GetChild("End")->GetVec4();

	auto sizesXMLReader = Reader->GetChild("Sizes");
	m_Sizes[0] = sizesXMLReader->GetChild("Start")->GetVec2();
	m_Sizes[1] = sizesXMLReader->GetChild("Middle")->GetVec2();
	m_Sizes[1] = sizesXMLReader->GetChild("End")->GetVec2();

	if (auto textureXMLReader = Reader->GetChild("Texture"))
	{
		auto texture = m_BaseManager.GetManager<IznTexturesFactory>()->LoadTexture2D(textureXMLReader->GetValue());
		SetTexture(texture);
	}

	auto gravityXMLReader = Reader->GetChild("Gravity");
	m_GravityDirection = gravityXMLReader->GetChild("Direction")->GetVec3();
	m_GravityPowerSEC = gravityXMLReader->GetChild("Power")->GetFloat();

	m_DeaccelerateSEC = Reader->GetChild("Deaccelerate")->GetFloat();
}

void CParticleSystem::Save(const std::shared_ptr<IXMLWriter>& Writer) const
{
	auto lifeTimeXMLWriter = Writer->CreateChild("LifeTime");
	lifeTimeXMLWriter->SetFloat(m_LifetimeMS);

	auto lifeTimeMiddlePointXMLWriter = Writer->CreateChild("LifeTimeMiddlePoint");
	lifeTimeMiddlePointXMLWriter->SetFloat(m_LifetimeMiddlePoint);

	auto colorsGroupXMLWriter = Writer->CreateChild("Colors");
	{
		auto startColorXMLWriter = colorsGroupXMLWriter->CreateChild("Start");
		startColorXMLWriter->SetVec4(m_Colors[0]);

		auto middleColorXMLWriter = colorsGroupXMLWriter->CreateChild("Middle");
		middleColorXMLWriter->SetVec4(m_Colors[1]);

		auto endColorXMLWriter = colorsGroupXMLWriter->CreateChild("End");
		endColorXMLWriter->SetVec4(m_Colors[2]);
	}

	auto sizesGroupXMLWriter = Writer->CreateChild("Sizes");
	{
		auto startSizeXMLWriter = sizesGroupXMLWriter->CreateChild("Start");
		startSizeXMLWriter->SetVec2(m_Sizes[0]);

		auto middleSizeXMLWriter = sizesGroupXMLWriter->CreateChild("Middle");
		middleSizeXMLWriter->SetVec2(m_Sizes[1]);

		auto endSizeXMLWriter = sizesGroupXMLWriter->CreateChild("End");
		endSizeXMLWriter->SetVec2(m_Sizes[2]);
	}

	if (m_Texture)
	{
		auto textureXMLWriter = Writer->CreateChild("Texture");
		textureXMLWriter->SetValue(m_Texture->GetFilename());
	}

	auto gravityXMLWriter = Writer->CreateChild("Gravity");
	{
		auto gravityDirection = gravityXMLWriter->CreateChild("Direction");
		gravityDirection->SetVec3(m_GravityDirection);

		auto gravityPowerXMLWriter = gravityXMLWriter->CreateChild("Power");
		gravityPowerXMLWriter->SetFloat(m_GravityPowerSEC);
	}

	auto deaccelerateXMLWriter = Writer->CreateChild("Deaccelerate");
	deaccelerateXMLWriter->SetFloat(m_DeaccelerateSEC);
}



//
// Protected
//
void CParticleSystem::CreateNewParticle()
{
	SParticle p(m_LifetimeMS);
	p.Color = m_Colors[0];
	p.Size = m_Sizes[0];

	p.Position = m_OwnerNode->GetPosition();
	p.StartPosition = p.Position;
	p.Direction = glm::normalize(glm::vec3(0.0f, 1.0f, 0.0f));
	//p.Direction = Random::UnitVector3f();// CalcSpreadMatrix(glm::two_pi<float>(), 0.0f, 1.0, 1.0f) * glm::vec4(1.0f, 1.0f, 1.0f, 0.0f);
	p.SpeedSEC = 0.0 * Random::Range(0.5f, 1.5f);

	m_Particles.push_back(p);
}

void CParticleSystem::UpdateParticle(SParticle& P, const UpdateEventArgs & e)
{
	float l = P.CurrentLifeTimeMS / P.MaxLifeTimeMS;
	P.Color = InterpolateParticleValue(l, m_LifetimeMiddlePoint, m_Colors[0], m_Colors[1], m_Colors[2]);
	P.Size = InterpolateParticleValue(l, m_LifetimeMiddlePoint, m_Sizes[0], m_Sizes[1], m_Sizes[2]);

	// Speed influence only by deaccelerate
	{
		float deaccelerateByFrame = (P.SpeedSEC / 60.0f) * float(e.DeltaTimeMultiplier);

		P.SpeedSEC -= deaccelerateByFrame;
		if (P.SpeedSEC < 0.0f)
			P.SpeedSEC = 0.0f;
	}

	glm::vec3 positionChange = glm::vec3(0.0f);

	// Affect by speed
	{
		float speedByFrame = (P.SpeedSEC / 60.0f) * float(e.DeltaTimeMultiplier);
		positionChange += (P.Direction * speedByFrame);
	}

	// Affect by gravity
	{
		float gravityPowerByFrame = (m_GravityPowerSEC / 60.0f) * float(e.DeltaTimeMultiplier);
		positionChange += (m_GravityDirection * gravityPowerByFrame);
	}

	P.Position += positionChange;
}

