#include "stdafx.h"

// General
#include "ParticleSystem.h"

// Additional
#include "Scene/Components/ModelComponent/AnimatedValue.h"

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



CParticleSystem::CParticleSystem(const ISceneNode& SceneNode)
	: m_GravityDirection(glm::vec3(0.0f, -1.0f, 0.0f))
	, m_GravityValue(0.1f)
	, m_Deaccelerate(0.02f)
	, m_OwnerNode(SceneNode)

	, m_LastParticleTime(0.0f)
	, m_ParticleInterval(15.0f)
{
	m_Lifetime = 1000.0f;
	m_LifeTimeMiddlePoint = 0.5f;

	m_Colors[0] = glm::vec4(10.0f, 0.0f, 0.0f, 1.0f);
	m_Colors[1] = glm::vec4(0.0f, 10.0f, 0.0f, 1.0f);
	m_Colors[2] = glm::vec4(0.0f, 0.0f, 10.0f, 1.0f);

	m_Sizes[0] = glm::vec2(3.0f);
	m_Sizes[1] = glm::vec2(1.0f);
	m_Sizes[2] = glm::vec2(0.5f);
}

CParticleSystem::~CParticleSystem()
{
}



//
// IParticleSystem
//
void CParticleSystem::Update(const UpdateEventArgs & e)
{
	for (auto pIt = m_CPUParticles.begin(); pIt != m_CPUParticles.end(); )
	{
		auto& p = *pIt;
		if (p.CurrentLifeTime > p.MaxLifeTime)
		{
			pIt = m_CPUParticles.erase(pIt);
			continue;
		}

		p.CurrentLifeTime += e.DeltaTime;
		UpdateParticle(p, e);
		pIt++;
	}

	m_GPUParticles.resize(m_CPUParticles.size());
	for (size_t i = 0; i < m_CPUParticles.size(); i++)
		m_GPUParticles[i] = m_CPUParticles.at(i).ToGPUParticle();

	if (m_LastParticleTime + m_ParticleInterval < e.TotalTime)
	{
		CreateNewParticle();
		m_LastParticleTime = e.TotalTime;
	}

}

const std::vector<SGPUParticle>& CParticleSystem::GetGPUParticles() const
{
	return m_GPUParticles;
}

void CParticleSystem::SetTexture(const std::shared_ptr<ITexture>& Material)
{
	m_Texture = Material;
}

std::shared_ptr<ITexture> CParticleSystem::GetTexture() const
{
	return m_Texture;
}

std::shared_ptr<IBlendState> CParticleSystem::GetBlendState() const
{
	return std::shared_ptr<IBlendState>();
}



//
// Protected
//
void CParticleSystem::CreateNewParticle()
{
	SCPUParticle p(m_Lifetime);
	p.Color = m_Colors[0];
	p.Size = m_Sizes[0];

	p.Position = m_OwnerNode.GetTranslationAbs();
	p.StartPosition = p.Position;
	p.Direction = glm::vec3(0.0f, 1.0f, 0.0f);
	//p.Direction = Random::UnitVector3f();// CalcSpreadMatrix(glm::two_pi<float>(), 0.0f, 1.0, 1.0f) * glm::vec4(1.0f, 1.0f, 1.0f, 0.0f);
	p.Direction = glm::normalize(p.Direction);
	p.Speed = 0.0 * Random::Range(0.5f, 1.5f);

	m_CPUParticles.push_back(p);
}

void CParticleSystem::UpdateParticle(SCPUParticle& P, const UpdateEventArgs & e)
{
	float l = P.CurrentLifeTime / P.MaxLifeTime;
	P.Color = InterpolateParticleValue(l, m_LifeTimeMiddlePoint, m_Colors[0], m_Colors[1], m_Colors[2]);
	P.Size = InterpolateParticleValue(l, m_LifeTimeMiddlePoint, m_Sizes[0], m_Sizes[1], m_Sizes[2]);

	{
		float speed = P.Speed;
		speed -= m_Deaccelerate * float(e.DeltaTimeMultiplier);
		if (speed < 0.0f)
			speed = 0.0f;
		P.Speed = speed;
	}

	if (P.Position.y > 0.0f)
		P.Position += (P.Direction * P.Speed * float(e.DeltaTimeMultiplier)) + (m_GravityDirection * m_GravityValue * float(e.DeltaTimeMultiplier));
}

