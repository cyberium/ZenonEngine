#pragma once

#include "../../SceneFunctional/Base/SceneNode.h"

#include "TransformComponent3D.h"
#include "MeshComponent3D.h"
#include "ColliderComponent3D.h"
#include "LightComponent3D.h"

// FORWARD BEGIN
class Scene3D;
// FORWARD END

class SceneNode3D : public SceneNode, public ILoadable
{
	typedef SceneNode base;
    friend Scene3D;
public:
    explicit                                        SceneNode3D();
	virtual                                         ~SceneNode3D();

    std::shared_ptr<SceneNode3D>                    shared_from_this();
    std::weak_ptr<SceneNode3D>                      weak_from_this();

    // Components engine
    virtual void                                    RegisterComponents() override;
    // Components engine template access
    template<typename T>
    std::shared_ptr<T>                              GetComponent();

	// Called before all others calls
	virtual void                                    UpdateCamera(const Camera* camera) override;

	// Allow a visitor to visit this node.
	virtual bool                                    Accept(IVisitor& visitor) override;

	// Updatable
	virtual void                                    OnUpdate(UpdateEventArgs& e) override;

	// ILoadableObject
	virtual bool                                    PreLoad()             override;
	virtual bool                                    Load()                override;
	virtual bool                                    Delete()              override;
	void                                            setLoadingBegin()   override final;
	bool                                            isLoadingBegin()    const override final;
	void                                            setLoaded()         override final;
	bool                                            isLoaded()              const override final;
	virtual uint32                                  getPriority() const override;

protected:
    void                                            SetTransformComponent(std::shared_ptr<CTransformComponent3D> TransformComponent);
    void                                            SetColliderComponent(std::shared_ptr<CColliderComponent3D> ColliderComponent);

private:
    std::shared_ptr<CTransformComponent3D>          m_Components_Transform;
    std::shared_ptr<CColliderComponent3D>           m_Components_Collider;

private:
	std::atomic<bool>                               m_IsLoadingBegin;
	std::atomic<bool>                               m_IsLoaded;
};

#include "SceneNode3D.inl"
