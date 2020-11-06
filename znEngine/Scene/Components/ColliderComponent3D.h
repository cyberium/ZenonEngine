#pragma once

class ZN_API CColliderComponent3D
	: public IColliderComponent3D
	, public CComponentBase
{
public:
	CColliderComponent3D(const ISceneNode& OwnerNode);
    virtual ~CColliderComponent3D();

    // IColliderComponent3D
	void SetCullStrategy(ECullStrategy CullStrategy) override;
	ECullStrategy GetCullStrategy() const override;
	void SetCullDistance(float Distance) override;
	float GetCullDistance() const override;
    void SetBounds(BoundingBox Bounds) override;
	void ExtendBounds(BoundingBox Bounds) override;
    const BoundingBox& GetBounds() const override;
	const BoundingBox& GetWorldBounds() const override;
	void SetDebugDrawMode(bool Value) override;
	bool GetDebugDrawMode() const override;

	bool IsCulled(const ICameraComponent3D* Camera) const override;
	bool IsCulledByFrustum(const ICameraComponent3D* Camera) const override;
	bool IsCulledByDistance2D(const ICameraComponent3D* Camera) const override;
	bool IsCulledByDistance(const ICameraComponent3D* Camera) const override;
	bool IsRayIntersects(const Ray& Ray) const override;

    // ISceneNodeComponent
    void OnMessage(const ISceneNodeComponent* Component, ComponentMessageType Message) override;

	// IObjectSaveLoad
	void CopyTo(std::shared_ptr<IObject> Destination) const override;
	void Load(const std::shared_ptr<IXMLReader>& Reader) override;
	void Save(const std::shared_ptr<IXMLWriter>& Writer) const override;

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