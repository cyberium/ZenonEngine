#pragma once

// FORWARD BEGIN

// FORWARD END


struct __declspec(align(16)) ZN_API SGPUParticle
{
	SGPUParticle()
		: Position(glm::vec3(0.0f))
		, Color(ColorRGBA(1.0f))
		, Size(glm::vec2(10.0f))
		, TexCoordBegin(glm::vec2(0.0f))
		, TexCoordEnd(glm::vec2(1.0f))
	{}

	explicit SGPUParticle(const glm::vec3& Position_, const ColorRGBA& Color_, const glm::vec2& Size_)
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

	ColorRGBA Color;
	//--------------------------------------------------------------( 16 bytes )

	glm::vec2 Size;
	float __padding1[2];
	//--------------------------------------------------------------( 16 bytes )
};


ZN_INTERFACE ZN_API IParticleSystem
	: public virtual IObject
{
	virtual ~IParticleSystem() {}

	virtual void Update(const UpdateEventArgs& e) = 0;

	virtual void SetNode(const ISceneNode* Node) = 0;
	virtual const ISceneNode* GetNode() const = 0;

	virtual void SetEnableCreatingNewParticles(bool Value) = 0;
	virtual bool IsEnableCreatingNewParticles() const = 0;

	virtual void SetTexture(const std::shared_ptr<ITexture>& Texture) = 0;
	virtual std::shared_ptr<ITexture> GetTexture() const = 0;

	virtual const std::vector<SGPUParticle>& GetGPUParticles() const = 0;
};

ZN_INTERFACE ZN_API IParticleComponent3D
{
	ZN_OBJECTCLASS(cSceneNodeParticleComponent);
	virtual ~IParticleComponent3D() {}

	virtual void AddParticleSystem(std::shared_ptr<IParticleSystem> ParticleSystem) = 0;
	virtual void RemoveParticleSystem(std::shared_ptr<IParticleSystem> ParticleSystem) = 0;
	virtual void DeleteAllParticleSystem() = 0;
	virtual const std::vector<std::shared_ptr<IParticleSystem>>& GetParticleSystems() const = 0;
};
