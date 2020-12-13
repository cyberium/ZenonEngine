#pragma once

namespace
{
	struct ZN_API SParticle
	{
		SParticle(float MaxLifeTimeMS_)
			: Color(glm::vec4(1.0f))
			, Size(glm::vec2(3.0f))
			, TexCoordBegin(glm::vec2(0.0f))
			, TexCoordEnd(glm::vec2(1.0f))

			, Position(glm::vec3(0.0f))
			, StartPosition(glm::vec3(0.0f))
			, Direction(glm::vec3(0.0f))
			, SpeedSEC(1.0f)

			, CurrentLifeTimeMS(0.0f)
			, MaxLifeTimeMS(MaxLifeTimeMS_)
		{}

		glm::vec4 Color;
		glm::vec2 Size;

		glm::vec2 TexCoordBegin;
		glm::vec2 TexCoordEnd;

		glm::vec3 Position;
		glm::vec3 StartPosition;
		glm::vec3 Direction;
		float     SpeedSEC;

		float     CurrentLifeTimeMS;
		float     MaxLifeTimeMS;

		SGPUParticle ToGPUParticle() const
		{
			return SGPUParticle(Position, Color, Size);
		}
	};
}


class ZN_API CParticleSystem
	: virtual public Object
	, public IParticleSystem
{
public:
	CParticleSystem(const IBaseManager& BaseManager);
	virtual ~CParticleSystem();

	// IParticleSystem
	void Update(const UpdateEventArgs& e) override;

	void SetNode(const ISceneNode* Node) override;
	const ISceneNode* GetNode() const override;

	void SetEnableCreatingNewParticles(bool Value) override;
	bool IsEnableCreatingNewParticles() const override;

	// Lifetime params
	void SetLifeTimeMS(float LifeTimeMS);
	float GetLifeTimeMS() const;
	void SetLifeTimeMiddlePoint(float LifeTimeMiddlePoint);
	float GetLifeTimeMiddlePoint() const;

	// Colors
	void SetStartColor(glm::vec4 Color);
	glm::vec4 GetStartColor() const;
	void SetMiddleColor(glm::vec4 Color);
	glm::vec4 GetMiddleColor() const;
	void SetEndColor(glm::vec4 Color);
	glm::vec4 GetEndColor() const;

	// Sizes
	void SetStartSize(glm::vec2 Size);
	glm::vec2 GetStartSize() const;
	void SetMiddleSize(glm::vec2 Size);
	glm::vec2 GetMiddleSize() const;
	void SetEndSize(glm::vec2 Size);
	glm::vec2 GetEndSize() const;
	
	// Texture
	void SetTextureFilename(std::string TextureFileName);
	std::string GetTextureFilename() const;
	void SetTexture(const std::shared_ptr<ITexture>& Texture) override;
	std::shared_ptr<ITexture> GetTexture() const override;
	
	// Gravity
	void SetGravityDirection(glm::vec3 GravityDirection);
	glm::vec3 GetGravityDirection() const;
	void SetGravityPowerSEC(float GravityPowerSEC);
	float GetGravityPowerSEC() const;

	// Deaccelerate
	void SetDeaccelerateSEC(float DeaccelerateSEC);
	float GetDeaccelerateSEC() const;

	std::shared_ptr<IPropertiesGroup> GetProperties() const override;
	const std::vector<SGPUParticle>& GetGPUParticles() const override;

	// IObjectLoadSave
	void Load(const std::shared_ptr<IXMLReader>& Reader) override;
	void Save(const std::shared_ptr<IXMLWriter>& Writer) const override;

protected:
	void CreateNewParticle();
	void UpdateParticle(SParticle& CPUParticle, const UpdateEventArgs& e);

private:
	std::vector<SParticle> m_Particles;
	std::vector<SGPUParticle> m_GPUParticles;

	bool m_IsEnableCreatingNewParticles;
	float m_LastParticleTime;

private:
	std::shared_ptr<IPropertiesGroup> m_PropertiesGroup;

	/**
	  * If this flag is set to true, particle transformation include owner node transformation
	*/
	bool m_IsAttachedToNode;

	/**
	  * If this flag is set to true, particle system will not be deleted after owner node
	*/
	bool m_IsWaitToDissapearAllParticles;

	// Emitter params
	
	float m_EmmiterSpawnIntervalMS;
	size_t m_EmitterMaxParticlesCount;

	// Particle params
	float m_LifetimeMS;
	float m_LifetimeMiddlePoint; // 0.0 - 1.0f  //Point that correspond m_Colors[1] and m_Sizes[1] in lifetime.  

	glm::vec4 m_Colors[3];
	glm::vec2 m_Sizes[3];

	std::shared_ptr<ITexture> m_Texture;

	/**
	  * Gravity
	  * GravityDirection - ...
	  * GravityPowerSEC - how position of particle change while one second
	*/
	glm::vec3 m_GravityDirection;
	float m_GravityPowerSEC;

	/**
	  * Deaccelerate inicates how speed increase (negative value) or decrease (positive value) every second
	*/
	float m_DeaccelerateSEC;

private:
	const IBaseManager& m_BaseManager;
	const ISceneNode* m_OwnerNode;
};