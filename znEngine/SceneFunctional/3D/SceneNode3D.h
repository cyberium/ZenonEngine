#pragma once

#include "../Base/SceneNodeBase.h"

class OW_ENGINE_API SceneNode3D 
	: public SceneNodeBase
	, public ISceneNode3D
{
	typedef SceneNodeBase base;
    friend IScene;
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
	virtual void                                    UpdateCamera(const ICamera* camera) override;

	// Allow a visitor to visit this node.
	virtual bool                                    Accept(IVisitor* visitor) override;

	// Updatable
	virtual void                                    OnUpdate(UpdateEventArgs& e) override;

protected:
    void                                            SetTransformComponent(std::shared_ptr<ITransformComponent3D> TransformComponent);
	void                                            SetMeshComponent(std::shared_ptr<IMeshComponent3D> MeshComponent);
    void                                            SetColliderComponent(std::shared_ptr<IColliderComponent3D> ColliderComponent);

private:
    std::shared_ptr<ITransformComponent3D>          m_Components_Transform;
	std::shared_ptr<IMeshComponent3D>				m_Components_Mesh;
    std::shared_ptr<IColliderComponent3D>           m_Components_Collider;
};

#include "SceneNode3D.inl"
