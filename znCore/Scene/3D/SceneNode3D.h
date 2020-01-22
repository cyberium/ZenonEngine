#pragma once

class ZN_API SceneNode3D 
	: public ISceneNode3D
	, public ISceneNode3DInternal
{
    friend IScene;
public:
	SceneNode3D();
	virtual ~SceneNode3D();

	//
	// ISceneNode3D
	//
	virtual void                                    Initialize() override;
	virtual void                                    Finalize() override;

	// Name & Type
	void                                            SetType(std::string Type) override;
	std::string										GetType() const override;
	void                                            SetName(std::string Name) override;
	std::string										GetName() const override;

	// Childs functional
	virtual void                                    AddChild(std::shared_ptr<ISceneNode3D> childNode) override;
	virtual void                                    RemoveChild(std::shared_ptr<ISceneNode3D> childNode) override;
	virtual void                                    SetParent(ISceneNode3D* parentNode) override;
	virtual ISceneNode3D*                           GetParent() const override;
	virtual const Node3DList&                       GetChilds() override;
	void                                            RaiseOnParentChanged() override;

	// Actions & Properties
	IActionsGroup*									GetActions() const override;
	IPropertiesGroup*								GetProperties() const override;
	IScene*											GetScene() const override;

	//
	// Transform functional
	//
	void											SetTranslate(cvec3 Translate) override;
	void                                            AddTranslate(vec3 Translate) override;
	cvec3											GetTranslation() const override;
	void											SetRotation(cvec3 _rotate) override;
	cvec3											GetRotation() const override;
	void											SetRotationQuaternion(cquat _rotate) override;
	cquat											GetRotationQuaternion() const override;
	void											SetScale(cvec3 _scale) override;
	cvec3											GetScale() const override;

	virtual mat4									GetLocalTransform() const;
	virtual mat4									GetInverseLocalTransform() const;
	virtual void									SetLocalTransform(cmat4 localTransform);

	virtual mat4									GetWorldTransfom() const;
	virtual mat4									GetInverseWorldTransform() const;
	virtual mat4									GetParentWorldTransform() const;
	virtual void									SetWorldTransform(cmat4 worldTransform);

	virtual void									ForceRecalculateLocalTransform() override;


	//
	// Components engine
	//


	bool                                            IsComponentExists(GUID ComponentID) override;
	ISceneNodeComponent*                            GetComponent(GUID ComponentID) override;
	ISceneNodeComponent*                            AddComponent(GUID ComponentID, std::shared_ptr<ISceneNodeComponent> Component) override;
	const ComponentsMap&                            GetComponents() const override;
	void                                            RaiseComponentMessage(ISceneNodeComponent* Component, ComponentMessageType Message) override;
	virtual void                                    RegisterComponents() override;
    
	template<typename T> inline T* GetComponent()
	{
		return ISceneNode3D::GetComponent<T>();
	}
	template<> inline IMeshComponent3D* GetComponent<IMeshComponent3D>()
	{
		return m_Components_Mesh;
	}
	template<> inline IColliderComponent3D* GetComponent<IColliderComponent3D>()
	{
		return m_Components_Collider;
	}


	//
	// Others
	//


	// Called before all others calls
	virtual void                                    OnUpdate(UpdateEventArgs& e) override final;
	virtual void                                    DoUpdate(UpdateEventArgs& e) override;
	virtual void                                    UpdateCamera(const ICameraComponent3D* camera) override;
	virtual void                                    UpdateViewport(const Viewport* viewport) override;

	// Load & Save
	bool                                            Load(std::shared_ptr<IXMLReader> Reader) override;
	bool                                            Save(std::shared_ptr<IXMLWriter> Writer) override;

	// Allow a visitor to visit this node.
	virtual bool                                    Accept(IVisitor* visitor) override;


public:
	void                                            SetScene(std::weak_ptr<IScene> Scene);
	void                                            SetParentInternal(std::weak_ptr<ISceneNode3D> parentNode);


protected:
	virtual void									UpdateLocalTransform();
	virtual void									UpdateWorldTransform();
	IBaseManager*                                   GetBaseManager() const;

	void                                            SetMeshComponent(IMeshComponent3D* MeshComponent);
    void                                            SetColliderComponent(IColliderComponent3D* ColliderComponent);

private:
	std::string                                     m_Type;
	std::string                                     m_Name;

	Node3DList                                      m_Children;
	Node3DNameMap                                   m_ChildrenByName;
	std::weak_ptr<ISceneNode3D>                     m_ParentNode;

	std::shared_ptr<IActionsGroup>                  m_ActionsGroup;
	std::shared_ptr<IPropertiesGroup>               m_PropertiesGroup;
	std::weak_ptr<IScene>                           m_Scene;

	vec3											m_Translate;
	vec3											m_Rotate;
	quat											m_RotateQuat;
	bool											m_IsRotateQuat;
	vec3											m_Scale;

	glm::mat4										m_LocalTransform;
	glm::mat4										m_InverseLocalTransform;
	glm::mat4										m_WorldTransform;
	glm::mat4										m_InverseWorldTransform;

	ComponentsMap                                   m_Components;
	IMeshComponent3D*								m_Components_Mesh;
    IColliderComponent3D*							m_Components_Collider;
};
