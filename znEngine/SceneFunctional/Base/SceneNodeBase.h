#pragma once

class OW_ENGINE_API SceneNodeBase 
	: public ISceneNode
	, public ISceneNodeInternal
	, public Object
{
    friend IScene;
public:
	explicit SceneNodeBase();
	virtual                                         ~SceneNodeBase();

	virtual void									Initialize();
	virtual void									Finalize();
	
	const std::string&                              GetName() const override;
	void                                            SetName(const std::string& name) override;

    // Components engine
    bool                                            IsComponentExists(GUID ComponentID) override;
    std::shared_ptr<ISceneNodeComponent>            GetComponent(GUID ComponentID) override;
    std::shared_ptr<ISceneNodeComponent>            AddComponent(GUID ComponentID, std::shared_ptr<ISceneNodeComponent> Component) override;
    const ComponentsMap&                            GetComponents() const override;
    void                                            RaiseComponentMessage(std::shared_ptr<ISceneNodeComponent> Component, ComponentMessageType Message) override;
    virtual void                                    RegisterComponents();

    // Scene access
    void                                            SetScene(std::shared_ptr<IScene> Scene) override;
    std::shared_ptr<IScene>                         GetScene() const override;

	// Childs functional
	virtual void                                    AddChild(std::shared_ptr<ISceneNode> childNode) override;
	virtual void                                    RemoveChild(std::shared_ptr<ISceneNode> childNode) override;
	virtual void                                    SetParent(std::weak_ptr<ISceneNode> parentNode) override;
	virtual std::shared_ptr<ISceneNode>             GetParent() const override;
	virtual NodeList                                GetChilds() override;

	// Called before all others calls
	virtual void                                    UpdateCamera(const ICamera* camera) override;

	// Allow a visitor to visit this node.
	virtual bool                                    Accept(IVisitor* visitor) override;

	// Updatable
	virtual void                                    OnUpdate(UpdateEventArgs& e) override;



	//
	// ISceneNodeInternal
	//
	virtual void                                    SetParentInternal(std::weak_ptr<ISceneNode> parentNode);



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
	IBaseManager*				GetBaseManager() const;

    // Callbacks caller
    void                                            RaiseOnParentChanged();

private:
	std::string                                     m_Name;
    ComponentsMap                                   m_Components;

    std::weak_ptr<IScene>                           m_Scene;
	std::weak_ptr<ISceneNode>                       m_ParentNode;
	NodeList                                        m_Children;
	NodeNameMap                                     m_ChildrenByName;
};
