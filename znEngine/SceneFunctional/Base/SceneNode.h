#pragma once

// FORWARD BEGIN
class Scene;
// FORWARD END

class OW_ENGINE_API SceneNode : public Object, public virtual std::enable_shared_from_this<SceneNode>
{
	typedef Object base;
    friend Scene;
public:
    typedef std::map<GUID, std::shared_ptr<ISceneNodeComponent>>        ComponentsMap;
	typedef std::vector<std::shared_ptr<SceneNode>>                     NodeList;
	typedef std::multimap<std::string, std::shared_ptr<SceneNode>>      NodeNameMap;

public:
                                                    SceneNode();
	virtual                                         ~SceneNode();

	const std::string&                              GetName() const;
	void                                            SetName(const std::string& name);

    // Components engine
    bool                                            IsComponentExists(GUID ComponentID);
    std::shared_ptr<ISceneNodeComponent>            GetComponent(GUID ComponentID);
    std::shared_ptr<ISceneNodeComponent>            AddComponent(GUID ComponentID, std::shared_ptr<ISceneNodeComponent> Component);
    const ComponentsMap&                            GetComponents() const;
    void                                            RaiseComponentMessage(std::shared_ptr<ISceneNodeComponent> Component, ComponentMessageType Message);
    virtual void                                    RegisterComponents() = 0;

    // Components engine template access
    template<typename T>
    std::shared_ptr<T>                              IsComponentExists();
    template<typename T>
    std::shared_ptr<T>                              GetComponent();
    template<typename T>
    std::shared_ptr<T>                              AddComponent(std::shared_ptr<T> Component);

    // Scene access
    void                                            SetScene(std::shared_ptr<Scene> Scene);
    std::shared_ptr<Scene>                          GetScene() const;

    template<typename T, typename... Args>
    std::shared_ptr<T>                              CreateSceneNode(Args &&... _Args);

	// Childs functional
	virtual void                                    AddChild(std::shared_ptr<SceneNode> childNode);
	virtual void                                    RemoveChild(std::shared_ptr<SceneNode> childNode);
	virtual void                                    SetParent(std::weak_ptr<SceneNode> parentNode);
	virtual std::shared_ptr<SceneNode>              GetParent() const;
	virtual NodeList                                GetChilds();

	// Called before all others calls
	virtual void                                    UpdateCamera(const Camera* camera);

	// Allow a visitor to visit this node.
	virtual bool                                    Accept(IVisitor* visitor);

	// Updatable
	virtual void                                    OnUpdate(UpdateEventArgs& e);

protected:
    // Callbacks caller
    void                                            RaiseOnParentChanged();

private:
	std::string                                     m_Name;
    ComponentsMap                                   m_Components;

    std::weak_ptr<Scene>                            m_Scene;
	std::weak_ptr<SceneNode>                        m_ParentNode;
	NodeList                                        m_Children;
	NodeNameMap                                     m_ChildrenByName;
};

#include "SceneNode.inl"
