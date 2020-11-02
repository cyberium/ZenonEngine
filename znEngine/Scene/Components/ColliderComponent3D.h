#pragma once

class __declspec(UUID_ColliderComponent) ZN_API CColliderComponent3D
	: public IColliderComponent3D
	, public CComponentBase
{
public:
	CColliderComponent3D(const ISceneNode3D& OwnerNode);
    virtual ~CColliderComponent3D();

	void Copy(std::shared_ptr<ISceneNodeComponent> Destination) const override;

    // IColliderComponent3D
	void SetCullStrategy(ECullStrategy CullStrategy) override;
	ECullStrategy GetCullStrategy() const override;
	void SetCullDistance(float Distance) override;
	float GetCullDistance() const override;
    void SetBounds(BoundingBox Bounds) override;
	void ExtendBounds(BoundingBox Bounds) override;
    cbbox GetBounds() const override;
	cbbox GetWorldBounds() const override;
	void SetDebugDrawMode(bool Value) override;
	bool GetDebugDrawMode() const override;

	bool IsCulled(const ICameraComponent3D* Camera) const override;
	bool IsCulledByFrustum(const ICameraComponent3D* Camera) const override;
	bool IsCulledByDistance2D(const ICameraComponent3D* Camera) const override;
	bool IsCulledByDistance(const ICameraComponent3D* Camera) const override;
	bool IsRayIntersects(const Ray& Ray) const override;

    // CComponentBase
    void OnMessage(const ISceneNodeComponent* Component, ComponentMessageType Message) override;

protected:
    virtual void UpdateBounds();

	// Properties helper
	virtual void SetMinBounds(const glm::vec3& Min);
	virtual glm::vec3 GetMinBounds() const;
	virtual void SetMaxBounds(const glm::vec3& Max);
	virtual glm::vec3 GetMaxBounds() const;

private:
	ECullStrategy  m_CullStrategy;
	float          m_CullDistance;
    BoundingBox    m_Bounds;
	BoundingBox    m_WorldBounds;
	bool           m_DebugDraw;
};