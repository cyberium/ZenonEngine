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
	: Object(BaseManager)
	, m_BaseManager(BaseManager)

	, m_IsEnableCreatingNewParticles(true)
	, m_Texture(nullptr)

	, m_GravityDirection(glm::vec3(0.0f, -1.0f, 0.0f))
	, m_GravityPowerSEC(0.0f)

	, m_DeaccelerateSEC(1.2f)

	, m_LastParticleTimeMS(0.0f)
	, m_EmmiterSpawnIntervalMS(5.0f)

	, m_OwnerNode(nullptr)
{
	GetProperties()->SetName("ParicleSystem");

	m_LifetimeMS = 700.0f;
	m_LifetimeMiddlePoint = 0.5f;

	m_Colors[0] = ColorRGBA(1.0f, 0.7f, 0.3f, 1.0f);
	m_Colors[1] = ColorRGBA(0.7f, 1.0f, 0.1f, 1.0f);
	m_Colors[2] = ColorRGBA(0.3f, 0.2f, 0.7f, 1.0f);

	m_Sizes[0] = glm::vec2(4.0f);
	m_Sizes[1] = glm::vec2(2.0f);
	m_Sizes[2] = glm::vec2(0.1f);


	// EmmiterSpawnIntervalMS
	{
		auto emmiterSpawnIntervalMS = MakeShared(CPropertyWrapped<float>, "SetEmmiterSpawnIntervalMS", "SetEmmiterSpawnIntervalMS", 5.0f);
		emmiterSpawnIntervalMS->SetValueSetter(std::bind(&CParticleSystem::SetEmmiterSpawnIntervalMS, this, std::placeholders::_1));
		emmiterSpawnIntervalMS->SetValueGetter(std::bind(&CParticleSystem::GetEmmiterSpawnIntervalMS, this));
		GetProperties()->AddProperty(emmiterSpawnIntervalMS);
	}

	// EmitterEmitterMaxParticlesCount
	{
		auto emitterMaxParticlesCount = MakeShared(CPropertyWrapped<float>, "EmitterEmitterMaxParticlesCount", "EmitterEmitterMaxParticlesCount", 150);
		emitterMaxParticlesCount->SetValueSetter(std::bind(&CParticleSystem::SetEmitterEmitterMaxParticlesCount, this, std::placeholders::_1));
		emitterMaxParticlesCount->SetValueGetter(std::bind(&CParticleSystem::GetEmitterEmitterMaxParticlesCount, this));
		GetProperties()->AddProperty(emitterMaxParticlesCount);
	}

	// InitialSpeedSEC
	{
		auto initialSpeedSEC = MakeShared(CPropertyWrapped<float>, "InitialSpeedSEC", "InitialSpeedSEC", 1.0f);
		initialSpeedSEC->SetValueSetter(std::bind(&CParticleSystem::SetInitialSpeedSEC, this, std::placeholders::_1));
		initialSpeedSEC->SetValueGetter(std::bind(&CParticleSystem::GetInitialSpeedSEC, this));
		GetProperties()->AddProperty(initialSpeedSEC);
	}

	// LifeTimeMS
	{
		auto lifeTimeMSProperty = MakeShared(CPropertyWrapped<float>, "LifeTimeMS", "LifeTimeMS", 1000.0f);
		lifeTimeMSProperty->SetValueSetter(std::bind(&CParticleSystem::SetLifeTimeMS, this, std::placeholders::_1));
		lifeTimeMSProperty->SetValueGetter(std::bind(&CParticleSystem::GetLifeTimeMS, this));
		GetProperties()->AddProperty(lifeTimeMSProperty);
	}

	// LifeTimeMiddlePoint
	{
		auto lifeTimeMiddlePointProperty = MakeShared(CPropertyWrapped<float>, "LifeTimeMiddlePoint", "LifeTimeMiddlePoint", 0.5f);
		lifeTimeMiddlePointProperty->SetValueSetter(std::bind(&CParticleSystem::SetLifeTimeMiddlePoint, this, std::placeholders::_1));
		lifeTimeMiddlePointProperty->SetValueGetter(std::bind(&CParticleSystem::GetLifeTimeMiddlePoint, this));
		GetProperties()->AddProperty(lifeTimeMiddlePointProperty);
	}

	// Colors
	{
		std::shared_ptr<IPropertiesGroup> colorsPropertiesGroup = MakeShared(CPropertiesGroup, "Colors", "Colors");
		GetProperties()->AddProperty(colorsPropertiesGroup);

		{
			auto startColorProperty = MakeShared(CPropertyWrapped<ColorRGBA>, "Start", "", ColorRGBA(1.0f));
			startColorProperty->SetValueSetter(std::bind(&CParticleSystem::SetStartColor, this, std::placeholders::_1));
			startColorProperty->SetValueGetter(std::bind(&CParticleSystem::GetStartColor, this));
			colorsPropertiesGroup->AddProperty(startColorProperty);
		}

		{
			auto middleColorProperty = MakeShared(CPropertyWrapped<ColorRGBA>, "Middle", "", ColorRGBA(1.0f));
			middleColorProperty->SetValueSetter(std::bind(&CParticleSystem::SetMiddleColor, this, std::placeholders::_1));
			middleColorProperty->SetValueGetter(std::bind(&CParticleSystem::GetMiddleColor, this));
			colorsPropertiesGroup->AddProperty(middleColorProperty);
		}

		{
			auto endColorProperty = MakeShared(CPropertyWrapped<ColorRGBA>, "End", "", ColorRGBA(1.0f));
			endColorProperty->SetValueSetter(std::bind(&CParticleSystem::SetEndColor, this, std::placeholders::_1));
			endColorProperty->SetValueGetter(std::bind(&CParticleSystem::GetEndColor, this));
			colorsPropertiesGroup->AddProperty(endColorProperty);
		}
	}

	// Sizes
	{
		std::shared_ptr<IPropertiesGroup> sizesPropertiesGroup = MakeShared(CPropertiesGroup, "Sizes", "Sizes");
		GetProperties()->AddProperty(sizesPropertiesGroup);

		{
			auto startSizeProperty = MakeShared(CPropertyWrappedVec2, "Start", "", glm::vec2(1.0f));
			startSizeProperty->SetValueSetter(std::bind(&CParticleSystem::SetStartSize, this, std::placeholders::_1));
			startSizeProperty->SetValueGetter(std::bind(&CParticleSystem::GetStartSize, this));
			sizesPropertiesGroup->AddProperty(startSizeProperty);
		}

		{
			auto middleSizeProperty = MakeShared(CPropertyWrappedVec2, "Middle", "", glm::vec2(1.0f));
			middleSizeProperty->SetValueSetter(std::bind(&CParticleSystem::SetMiddleSize, this, std::placeholders::_1));
			middleSizeProperty->SetValueGetter(std::bind(&CParticleSystem::GetMiddleSize, this));
			sizesPropertiesGroup->AddProperty(middleSizeProperty);
		}

		{
			auto endSizeProperty = MakeShared(CPropertyWrappedVec2, "End", "", glm::vec2(1.0f));
			endSizeProperty->SetValueSetter(std::bind(&CParticleSystem::SetEndSize, this, std::placeholders::_1));
			endSizeProperty->SetValueGetter(std::bind(&CParticleSystem::GetEndSize, this));
			sizesPropertiesGroup->AddProperty(endSizeProperty);
		}
	}


	// Texture
	{
		auto textureFileNameProperty = MakeShared(CPropertyWrapped<std::string>, "Texture", "Texture", "");
		textureFileNameProperty->SetValueSetter(std::bind(&CParticleSystem::SetTextureFilename, this, std::placeholders::_1));
		textureFileNameProperty->SetValueGetter(std::bind(&CParticleSystem::GetTextureFilename, this));
		GetProperties()->AddProperty(textureFileNameProperty);
	}


	// Gravity
	{
		std::shared_ptr<IPropertiesGroup> gravityPropertiesGroup = MakeShared(CPropertiesGroup, "Gravity", "Gravity");
		GetProperties()->AddProperty(gravityPropertiesGroup);

		// Direction
		{
			auto gravityDirectionProperty = MakeShared(CPropertyWrappedVec3, "Direction", "Direction", glm::vec3(0.0f, -1.0f, 0.0f));
			gravityDirectionProperty->SetValueSetter(std::bind(&CParticleSystem::SetGravityDirection, this, std::placeholders::_1));
			gravityDirectionProperty->SetValueGetter(std::bind(&CParticleSystem::GetGravityDirection, this));
			gravityPropertiesGroup->AddProperty(gravityDirectionProperty);
		}

		// PowerSEC
		{
			auto gravityPowerSECProperty = MakeShared(CPropertyWrapped<float>, "PowerSEC", "PowerSEC", 0.1f);
			gravityPowerSECProperty->SetValueSetter(std::bind(&CParticleSystem::SetGravityPowerSEC, this, std::placeholders::_1));
			gravityPowerSECProperty->SetValueGetter(std::bind(&CParticleSystem::GetGravityPowerSEC, this));
			gravityPropertiesGroup->AddProperty(gravityPowerSECProperty);
		}
	}

	// DeaccelerateSEC
	{
		auto deaccelerateSECProperty = MakeShared(CPropertyWrapped<float>, "DeaccelerateSEC", "DeaccelerateSEC", 0.01f);
		deaccelerateSECProperty->SetValueSetter(std::bind(&CParticleSystem::SetDeaccelerateSEC, this, std::placeholders::_1));
		deaccelerateSECProperty->SetValueGetter(std::bind(&CParticleSystem::GetDeaccelerateSEC, this));
		GetProperties()->AddProperty(deaccelerateSECProperty);
	}
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
		if (pIt->ParticleCreationTimeMS + pIt->MaxLifeTimeMS < e.TotalTime)
		{
			pIt = m_Particles.erase(pIt);
			continue;
		}
		pIt++;
	}

	for (auto& p : m_Particles)
	{
		UpdateParticle(p, e);
	}

	m_GPUParticles.resize(m_Particles.size());
	for (size_t i = 0; i < m_Particles.size(); i++)
		m_GPUParticles[i] = m_Particles.at(i).ToGPUParticle();

	if (IsEnableCreatingNewParticles() && (m_LastParticleTimeMS + m_EmmiterSpawnIntervalMS < e.TotalTime))
	{
		CreateNewParticle(e);
		m_LastParticleTimeMS = e.TotalTime;
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

// LifeTime

void CParticleSystem::SetEmmiterSpawnIntervalMS(float SpawnIntervalMS)
{
	m_EmmiterSpawnIntervalMS = SpawnIntervalMS;
}

float CParticleSystem::GetEmmiterSpawnIntervalMS() const
{
	return m_EmmiterSpawnIntervalMS;
}

void CParticleSystem::SetEmitterEmitterMaxParticlesCount(float EmitterEmitterMaxParticlesCount)
{
	m_EmitterMaxParticlesCount = EmitterEmitterMaxParticlesCount;
}

float CParticleSystem::GetEmitterEmitterMaxParticlesCount() const
{
	return m_EmitterMaxParticlesCount;
}

void CParticleSystem::SetInitialSpeedSEC(float InitialSpeedSEC)
{
	m_InitialSpeedSEC = InitialSpeedSEC;
}

float CParticleSystem::GetInitialSpeedSEC() const
{
	return m_InitialSpeedSEC;
}

void CParticleSystem::SetLifeTimeMS(float LifeTimeMS)
{
	m_LifetimeMS = LifeTimeMS;
}

float CParticleSystem::GetLifeTimeMS() const
{
	return m_LifetimeMS;
}

void CParticleSystem::SetLifeTimeMiddlePoint(float LifeTimeMiddlePoint)
{
	m_LifetimeMiddlePoint = LifeTimeMiddlePoint;
}

float CParticleSystem::GetLifeTimeMiddlePoint() const
{
	return m_LifetimeMiddlePoint;
}

// Colors

void CParticleSystem::SetStartColor(ColorRGBA Color)
{
	m_Colors[0] = Color;
}

ColorRGBA CParticleSystem::GetStartColor() const
{
	return m_Colors[0];
}

void CParticleSystem::SetMiddleColor(ColorRGBA Color)
{
	m_Colors[1] = Color;
}

ColorRGBA CParticleSystem::GetMiddleColor() const
{
	return m_Colors[1];
}

void CParticleSystem::SetEndColor(ColorRGBA Color)
{
	m_Colors[2] = Color;
}

ColorRGBA CParticleSystem::GetEndColor() const
{
	return m_Colors[2];
}

// Sizes

void CParticleSystem::SetStartSize(glm::vec2 Size)
{
	m_Sizes[0] = Size;
}

glm::vec2 CParticleSystem::GetStartSize() const
{
	return m_Sizes[0];
}

void CParticleSystem::SetMiddleSize(glm::vec2 Size)
{
	m_Sizes[1] = Size;
}

glm::vec2 CParticleSystem::GetMiddleSize() const
{
	return m_Sizes[1];
}

void CParticleSystem::SetEndSize(glm::vec2 Size)
{
	m_Sizes[2] = Size;
}

glm::vec2 CParticleSystem::GetEndSize() const
{
	return m_Sizes[2];
}

// Texture

void CParticleSystem::SetTextureFilename(std::string TextureFileName)
{
	auto texturesFactory = m_BaseManager.GetManager<IznTexturesFactory>();
	auto texture = texturesFactory->LoadTexture2D(TextureFileName);
	if (texture == texturesFactory->GetDefaultTexture())
	{
		Log::Warn("ParticlesSystem: Unable to set '%s' texture", TextureFileName.c_str());
		return;
	}

	SetTexture(texture);
}

std::string CParticleSystem::GetTextureFilename() const
{
	if (m_Texture == nullptr)
		return "";
	return m_Texture->GetFilename();
}

void CParticleSystem::SetTexture(const std::shared_ptr<ITexture>& Material)
{
	m_Texture = Material;
}

std::shared_ptr<ITexture> CParticleSystem::GetTexture() const
{
	return m_Texture;
}

// Gravity

void CParticleSystem::SetGravityDirection(glm::vec3 GravityDirection)
{
	m_GravityDirection = GravityDirection;
}

glm::vec3 CParticleSystem::GetGravityDirection() const
{
	return m_GravityDirection;
}

void CParticleSystem::SetGravityPowerSEC(float GravityPowerSEC)
{
	m_GravityPowerSEC = GravityPowerSEC;
}

float CParticleSystem::GetGravityPowerSEC() const
{
	return m_GravityPowerSEC;
}

// Deaccelerate

void CParticleSystem::SetDeaccelerateSEC(float DeaccelerateSEC)
{
	m_DeaccelerateSEC = DeaccelerateSEC;
}

float CParticleSystem::GetDeaccelerateSEC() const
{
	return m_DeaccelerateSEC;
}



//
// IObjectLoadSave
//
void CParticleSystem::CopyTo(std::shared_ptr<IObject> Destination) const
{
	auto destinationAsparticleSystem = std::dynamic_pointer_cast<IParticleSystem>(Destination);

	GetProperties()->CopyTo(destinationAsparticleSystem->GetProperties());
}

void CParticleSystem::Load(const std::shared_ptr<IXMLReader>& Reader)
{
	GetProperties()->Load(Reader);
}

void CParticleSystem::Save(const std::shared_ptr<IXMLWriter>& Writer) const
{
	 GetProperties()->Save(Writer);
}



//
// Protected
//
void CParticleSystem::CreateNewParticle(const UpdateEventArgs& e)
{
	SParticle p(e.TotalTime, m_LifetimeMS);
	p.Color = m_Colors[0];
	p.Size = m_Sizes[0];

	p.Position = m_OwnerNode->GetPosition();
	p.StartPosition = p.Position;
	p.Direction = glm::normalize(glm::vec3(0.0f, 1.0f, 0.0f));
	p.Direction = Random::Hemisphere(glm::vec3(0.0f, 1.0f, 0.0f)); //Random::UnitVector3f(); //CalcSpreadMatrix(glm::pi<float>(), 0.0f, 1.0, 1.0f) * glm::vec4(1.0f, 1.0f, 1.0f, 0.0f);
	p.SpeedSEC = m_InitialSpeedSEC;

	UpdateParticle(p, e);

	m_Particles.push_back(p);
}

void CParticleSystem::UpdateParticle(SParticle& P, const UpdateEventArgs & e)
{
	float l = (e.TotalTime - P.ParticleCreationTimeMS) / P.MaxLifeTimeMS;
	P.Color = InterpolateParticleValue(l, m_LifetimeMiddlePoint, m_Colors[0], m_Colors[1], m_Colors[2]);
	P.Size = InterpolateParticleValue(l, m_LifetimeMiddlePoint, m_Sizes[0], m_Sizes[1], m_Sizes[2]);

	// Speed influence only by deaccelerate
	{
		float deaccelerateByFrame = (m_DeaccelerateSEC / 60.0f) * float(e.DeltaTimeMultiplier);

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

