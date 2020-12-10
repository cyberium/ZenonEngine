#pragma once

// FORWARD BEGIN

// FORWARD END


struct __declspec(align(16)) ZN_API SGPUParticle
{
	SGPUParticle()
		: Position(glm::vec3(0.0f))
		, Color(glm::vec4(1.0f))
		, Size(glm::vec2(10.0f))
		, TexCoordBegin(glm::vec2(0.0f))
		, TexCoordEnd(glm::vec2(1.0f))
	{}

	explicit SGPUParticle(const glm::vec3& Position_, const glm::vec4& Color_, const glm::vec2& Size_)
		: Position(Position_)
		, Color(Color_)
		, Size(Size_)
		, TexCoordBegin(glm::vec2(0.0f))
		, TexCoordEnd(glm::vec2(1.0f))
	{}

	glm::vec3 Position;
	float __padding0;
	//--------------------------------------------------------------( 16 bytes )

	glm::vec2 TexCoordBegin;
	glm::vec2 TexCoordEnd;
	//--------------------------------------------------------------( 16 bytes )

	glm::vec4 Color;
	//--------------------------------------------------------------( 16 bytes )

	glm::vec2 Size;
	float __padding1[2];
	//--------------------------------------------------------------( 16 bytes )
};


struct ZN_API SCPUParticle
{
	SCPUParticle(float MaxLifeTime_)
		: Color(glm::vec4(1.0f))
		, Size(glm::vec2(3.0f))
		, TexCoordBegin(glm::vec2(0.0f))
		, TexCoordEnd(glm::vec2(1.0f))

		, Position(glm::vec3(0.0f))
		, StartPosition(glm::vec3(0.0f))
		, Direction(glm::vec3(0.0f))
		, Speed(1.0f)

		, CurrentLifeTime(0.0f)
		, MaxLifeTime(MaxLifeTime_)
	{}

	glm::vec4 Color;
	glm::vec2 Size;

	glm::vec2 TexCoordBegin;
	glm::vec2 TexCoordEnd;

	glm::vec3 Position;
	glm::vec3 StartPosition;
	glm::vec3 Direction;
	float     Speed;

	float     CurrentLifeTime;
	float     MaxLifeTime;

	SGPUParticle ToGPUParticle() const
	{
		return SGPUParticle(Position, Color, Size);
	}
};


ZN_INTERFACE ZN_API IParticleSystem
{
	virtual ~IParticleSystem() {}

	virtual void Update(const UpdateEventArgs& e) = 0;

	virtual const std::vector<SGPUParticle>& GetGPUParticles() const = 0;

	virtual void SetEnableCreatingNewParticles(bool Value) = 0;
	virtual bool IsEnableCreatingNewParticles() const = 0;

	virtual void SetTexture(const std::shared_ptr<ITexture>& Texture) = 0;
	virtual std::shared_ptr<ITexture> GetTexture() const = 0;

	virtual std::shared_ptr<IBlendState> GetBlendState() const = 0;
};

ZN_INTERFACE ZN_API IParticleComponent3D
{
	ZN_OBJECTCLASS(cSceneNodeParticleComponent);
	virtual ~IParticleComponent3D() {}

	virtual void Attach(std::shared_ptr<IParticleSystem> ParticleSystem) = 0;
	virtual std::shared_ptr<IParticleSystem> Detach(std::shared_ptr<IParticleSystem> ParticleSystem) = 0;
	virtual const std::vector<std::shared_ptr<IParticleSystem>>& GetParticleSystems() const = 0;
};
