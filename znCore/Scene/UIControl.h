#pragma once

#include "../ObjectsFactories/Object.h"

class ZN_API CUIControl 
	: virtual public IUIControl
	, public IUIControlInternal
	, public Object
{
public:
	CUIControl(IScene& Scene);
	virtual ~CUIControl();

	virtual void                                    Initialize() override;
	virtual void                                    Finalize() override;
	
	// Childs functional
	virtual void                                    AddChild(const std::shared_ptr<IUIControl>& childNode) override final;
	virtual void                                    RemoveChild(const std::shared_ptr<IUIControl>& childNode) override final;
	virtual void                                    MakeMeOrphan() override final;
	virtual std::shared_ptr<IUIControl>             GetParent() const override final;
	virtual const ControlsList&                     GetChilds() const override final;
	virtual std::shared_ptr<IUIControl>             GetChild(std::string Name) const override;

	// Actions & Properties
	virtual std::shared_ptr<IPropertiesGroup>       GetProperties() const final;
	virtual IScene&                                 GetScene() const final;

	void											SetTranslate(const glm::vec2& _translate) override;
	const glm::vec2&								GetTranslation() const override;
	glm::vec2										GetTranslationAbs() const override;
	void											SetRotation(const glm::vec3& _rotate) override;
	const glm::vec3&								GetRotation() const override;
	void											SetScale(const glm::vec2& _scale) override;
	const glm::vec2&								GetScale() const override;
	glm::vec2										GetScaleAbs() const override;
	virtual glm::mat4								GetLocalTransform() const override;
	virtual glm::mat4								GetWorldTransfom() const override;
	virtual glm::mat4								GetParentWorldTransform() const;

    virtual glm::vec2                               GetSize() const override;
    virtual BoundingRect                            GetBoundsAbs() override;
    virtual bool                                    IsPointInBoundsAbs(const glm::vec2& Point) override;

	virtual void                                    Accept(IVisitor* visitor) override;
	virtual void                                    AcceptMesh(IVisitor* visitor) override;

	// IObject
	void                                            SetName(const std::string& Name) override final { Object::SetName(Name); };

	// UI events
	void                                            SetOnClickCallback(std::function<void(const IUIControl* Node, glm::vec2)> OnClickCallback);

	// Input events
	virtual bool                                    OnKeyPressed(KeyEventArgs& e);
	virtual void                                    OnKeyReleased(KeyEventArgs& e);
	virtual void                                    OnMouseMoved(MouseMotionEventArgs& e);
	virtual bool                                    OnMouseButtonPressed(MouseButtonEventArgs& e);
	virtual void                                    OnMouseButtonReleased(MouseButtonEventArgs& e);
	virtual bool                                    OnMouseWheel(MouseWheelEventArgs& e);

	// Syntetic events
	virtual void                                    OnMouseEntered();
	virtual void                                    OnMouseLeaved();

	// IUIControlInternal
	void                                            AddChildInternal(std::shared_ptr<IUIControl> ChildNode) override;
	void                                            RemoveChildInternal(std::shared_ptr<IUIControl> ChildNode) override;
	void                                            RaiseOnParentChangedInternal() override;

protected:
	IBaseManager&                                   GetBaseManager() const;
	IRenderDevice&                                  GetRenderDevice() const;
	virtual glm::mat4                               CalculateLocalTransform() const;
	virtual void									UpdateLocalTransform();
	virtual void									UpdateWorldTransform();

public: // Syntetic events // TODO: Make private
	bool                                            IsMouseOnNode() const;
	void                                            DoMouseEntered();
	void                                            DoMouseLeaved();
	bool                                            m_IsMouseOnNode;

private:
	void                                            AddChildPrivate(std::shared_ptr<IUIControl> ChildNode);
	void                                            RemoveChildPrivate(std::shared_ptr<IUIControl> ChildNode);
	void                                            SetParentPrivate(std::weak_ptr<IUIControl> parentNode);

private:
	ControlsList                                    m_Children;
	std::weak_ptr<IUIControl>                       m_ParentNode;

	std::shared_ptr<IPropertiesGroup>               m_PropertiesGroup;
	IScene&                                         m_Scene;

private:
	glm::vec2										m_Translate;
	glm::vec3										m_Rotate;
	glm::vec2										m_Scale;
	glm::mat4										m_LocalTransform;
	glm::mat4										m_InverseLocalTransform;
	glm::mat4										m_WorldTransform;
	glm::mat4										m_InverseWorldTransform;

private:
	std::function<void(const IUIControl* Node, glm::vec2)> m_OnClickCallback;
};

