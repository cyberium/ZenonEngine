#pragma once

#include "../SceneNodeBase.h"

class ZN_API SceneNode3D 
	: public SceneNodeBase
	, public ISceneNode3D
{

    friend IScene;
public:
	explicit                                        SceneNode3D();
	virtual                                         ~SceneNode3D();

    std::shared_ptr<SceneNode3D>                    shared_from_this();
    std::weak_ptr<SceneNode3D>                      weak_from_this();

	
	virtual void									SetLocalTransform(cmat4 localTransform) override;

	// ISceneNode3D
	void											SetTranslate(cvec3 Translate) override;
	void                                            AddTranslate(vec3 Translate) override;
	cvec3											GetTranslation() const override;
	void											SetRotation(cvec3 _rotate) override;
	cvec3											GetRotation() const override;
	void											SetRotationQuaternion(cquat _rotate) override;
	cquat											GetRotationQuaternion() const override;
	void											SetScale(cvec3 _scale) override;
	cvec3											GetScale() const override;


    // Components engine
    virtual void                                    RegisterComponents() override;
    // Components engine template access
    template<typename T>
    std::shared_ptr<T>                              GetComponent();

	// Load & Save
	bool                                            Load(std::shared_ptr<IXMLReader> Reader) override;
	bool                                            Save(std::shared_ptr<IXMLWriter> Writer) override;

	// Allow a visitor to visit this node.
	virtual bool                                    Accept(IVisitor* visitor) override;


protected:
	virtual void									UpdateLocalTransform() override;
	virtual void									UpdateWorldTransform() override;

	void                                            SetMeshComponent(std::shared_ptr<IMeshComponent3D> MeshComponent);
    void                                            SetColliderComponent(std::shared_ptr<IColliderComponent3D> ColliderComponent);


private:
	vec3											m_Translate;
	vec3											m_Rotate;
	quat											m_RotateQuat;
	bool											m_IsRotateQuat;
	vec3											m_Scale;

	std::shared_ptr<IMeshComponent3D>				m_Components_Mesh;
    std::shared_ptr<IColliderComponent3D>           m_Components_Collider;
};

#include "SceneNode3D.inl"
