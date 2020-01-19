#pragma once

class ZN_API SceneNodeBase 
	: public ISceneNode
	, public ISceneNodeInternal
	, public Object
{
    friend IScene;
public:
	explicit SceneNodeBase();
	virtual ~SceneNodeBase();

	virtual void									Initialize();
	virtual void									Finalize();
	

	// Name & Type
	std::string										GetType() const override;
	void                                            SetType(std::string Type) override;
	std::string										GetName() const override;
	void                                            SetName(std::string Name) override;


	// Transform functional
	virtual mat4									GetLocalTransform() const;
	virtual mat4									GetInverseLocalTransform() const;
	virtual void									SetLocalTransform(cmat4 localTransform);

	virtual mat4									GetWorldTransfom() const;
	virtual mat4									GetInverseWorldTransform() const;
	virtual mat4									GetParentWorldTransform() const;
	virtual void									SetWorldTransform(cmat4 worldTransform);

	virtual void									ForceRecalculateLocalTransform();


    // Components engine
    bool                                            IsComponentExists(GUID ComponentID) override;
    std::shared_ptr<ISceneNodeComponent>            GetComponent(GUID ComponentID) override;
    std::shared_ptr<ISceneNodeComponent>            AddComponent(GUID ComponentID, std::shared_ptr<ISceneNodeComponent> Component) override;
    const ComponentsMap&                            GetComponents() const override;
    void                                            RaiseComponentMessage(std::shared_ptr<ISceneNodeComponent> Component, ComponentMessageType Message) override;
    virtual void                                    RegisterComponents();


	// Actions & Properties
	std::shared_ptr<IActionsGroup>                  GetActions() const override;
	std::shared_ptr<IPropertiesGroup>				GetProperties() const override;


    // Scene access
    std::shared_ptr<IScene>                         GetScene() const override;


	// Childs functional
	virtual void                                    AddChild(std::shared_ptr<ISceneNode> childNode) override;
	virtual void                                    RemoveChild(std::shared_ptr<ISceneNode> childNode) override;
	virtual void                                    SetParent(std::weak_ptr<ISceneNode> parentNode) override;
	virtual std::shared_ptr<ISceneNode>             GetParent() const override;
	virtual NodeList                                GetChilds() override;


	// Called before all others calls
	virtual void                                    OnUpdate(UpdateEventArgs& e) override final;
	virtual void                                    DoUpdate(UpdateEventArgs& e) override;
	virtual void                                    UpdateCamera(const ICameraComponent3D* camera) override;
	virtual void                                    UpdateViewport(const Viewport* viewport) override;


	// Load & Save
	bool                                            Load(std::shared_ptr<IXMLReader> Reader);
	bool                                            Save(std::shared_ptr<IXMLWriter> Writer);


	// Allow a visitor to visit this node.
	virtual bool                                    Accept(IVisitor* visitor) override;



	//
	// ISceneNodeInternal
	//
	void                                            SetScene(std::shared_ptr<IScene> Scene) override;
	void                                            SetParentInternal(std::weak_ptr<ISceneNode> parentNode) override;


	// Callbacks caller
	void                                            RaiseOnParentChanged();





	//
	// Templates
	//
	template<typename T>
	inline std::shared_ptr<T> IsComponentExists()
	{
		return ISceneNode::IsComponentExists<T>();
	}
	template<typename T>
	inline std::shared_ptr<T> GetComponent()
	{
		return ISceneNode::GetComponent<T>();
	}
	template<typename T>
	inline std::shared_ptr<T> AddComponent(std::shared_ptr<T> Component)
	{
		return ISceneNode::AddComponent<T>(Component);
	}

	template<typename T, typename... Args>
	inline std::shared_ptr<T> CreateSceneNode(Args &&... _Args)
	{
		return ISceneNode::CreateSceneNode<T>(std::forward<Args>(_Args)...);
	}


protected:
	virtual void									UpdateLocalTransform();
	virtual void									UpdateWorldTransform();
	IBaseManager*                                   GetBaseManager() const;


protected:
	glm::mat4										m_LocalTransform;
	glm::mat4										m_InverseLocalTransform;
	glm::mat4										m_WorldTransform;
	glm::mat4										m_InverseWorldTransform;


private:
	std::string                                     m_Type;
	std::string                                     m_Name;

    ComponentsMap                                   m_Components;

	std::shared_ptr<IActionsGroup>                  m_ActionsGroup;
	std::shared_ptr<IPropertiesGroup>               m_PropertiesGroup;

    std::weak_ptr<IScene>                           m_Scene;
	std::weak_ptr<ISceneNode>                       m_ParentNode;
	NodeList                                        m_Children;
	NodeNameMap                                     m_ChildrenByName;
};
