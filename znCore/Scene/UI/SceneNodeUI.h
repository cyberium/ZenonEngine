#pragma once

class ZN_API UIBaseNodeClickedEventArgs
{
public:
    UIBaseNodeClickedEventArgs(std::shared_ptr<ISceneNodeUI> Initiator)
        : Initiator(Initiator)
    {}

    std::shared_ptr<ISceneNodeUI> Initiator;
};
typedef Delegate<UIBaseNodeClickedEventArgs> UIBaseNodeClickedEvent;



class ZN_API SceneNodeUI 
	: public ISceneNodeUI
{
	friend IScene;

public:
	typedef std::vector<std::shared_ptr<ISceneNodeUI>>                NodeUIList;
	typedef std::multimap<std::string, std::shared_ptr<ISceneNodeUI>> NodeUINameMap;

public:
	SceneNodeUI();
	virtual ~SceneNodeUI();

	virtual void                                    Initialize() override;
	virtual void                                    Finalize() override;

	// Name & Type
	void                                            SetType(std::string Type) override;
	std::string										GetType() const override;
	void                                            SetName(std::string Name) override;
	std::string										GetName() const override;
	
	// Childs functional
	virtual void                                    AddChild(const std::shared_ptr<ISceneNodeUI>& childNode) override;
	virtual void                                    RemoveChild(const std::shared_ptr<ISceneNodeUI>& childNode) override;
	virtual std::weak_ptr<ISceneNodeUI>             GetParent() const override;
	virtual const NodeUIList&                       GetChilds() override;
	void                                            RaiseOnParentChanged() override final;

	// Actions & Properties
	virtual IActionsGroup*                          GetActions() const;
	virtual IPropertiesGroup*                       GetProperties() const;
	virtual IScene*                                 GetScene() const;

	// ISceneNodeUI
	void											SetTranslate(cvec2 _translate);
	cvec2											GetTranslation() const;
	glm::vec2										GetTranslationAbs() const;
	void											SetRotation(cvec3 _rotate);
	cvec3											GetRotation() const;
	void											SetScale(cvec2 _scale);
	cvec2											GetScale() const;
	glm::vec2										GetScaleAbs() const;
	virtual mat4									GetLocalTransform() const;
	virtual mat4									GetWorldTransfom() const;

	// Size & bounds functional
    virtual glm::vec2                               GetSize();
    virtual BoundingRect                            GetBoundsAbs();
    virtual bool                                    IsPointInBoundsAbs(glm::vec2 Point) ;

	// Allow a visitor to visit this node. 
	virtual void                                    Accept(IVisitor* visitor);
	virtual void                                    AcceptMesh(IVisitor* visitor);

	// Input events
	virtual bool                                    OnKeyPressed(KeyEventArgs& e);
	virtual void                                    OnKeyReleased(KeyEventArgs& e);
	virtual void                                    OnMouseMoved(MouseMotionEventArgs& e);
	virtual bool                                    OnMouseButtonPressed(MouseButtonEventArgs& e);
	virtual void                                    OnMouseButtonReleased(MouseButtonEventArgs& e);
	virtual bool                                    OnMouseWheel(MouseWheelEventArgs& e);

    UIBaseNodeClickedEvent							Clicked;

	// Syntetic events
	virtual void                                    OnMouseEntered();
	virtual void                                    OnMouseLeaved();

private:
	void                                            SetSceneInternal(const std::weak_ptr<IScene>& Scene);
	void                                            AddChildInternal(const std::shared_ptr<ISceneNodeUI>& ChildNode);
	void                                            RemoveChildInternal(const std::shared_ptr<ISceneNodeUI>& ChildNode);
	void                                            SetParentInternal(const std::weak_ptr<ISceneNodeUI>& parentNode);

protected:
	virtual void									UpdateLocalTransform();
	virtual void									UpdateWorldTransform();
	IBaseManager&                                   GetBaseManager() const;

public: // Syntetic events // TODO: Make private
	bool                                            IsMouseOnNode() const;
	void                                            DoMouseEntered();
	void                                            DoMouseLeaved();
	bool                                            m_IsMouseOnNode;

private:
	std::string                                     m_Type;
	std::string                                     m_Name;

	NodeUIList                                      m_Children;
	NodeUINameMap                                   m_ChildrenByName;
	std::weak_ptr<ISceneNodeUI>                     m_ParentNode;

	std::shared_ptr<IActionsGroup>                  m_ActionsGroup;
	std::shared_ptr<IPropertiesGroup>               m_PropertiesGroup;
	std::weak_ptr<IScene>                           m_Scene;

private:
	glm::vec2										m_Translate;
	glm::vec3										m_Rotate;
	glm::vec2										m_Scale;
	glm::mat4										m_LocalTransform;
	glm::mat4										m_InverseLocalTransform;
	glm::mat4										m_WorldTransform;
	glm::mat4										m_InverseWorldTransform;
};

