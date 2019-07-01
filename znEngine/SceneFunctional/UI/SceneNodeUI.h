#pragma once

#include "SceneFunctional/Base/SceneNode.h"

#include "TransformComponentUI.h"

// FORWARD BEGIN
class CUIBaseNode;
class CUIWindowNode;
// FORWARD END


class OW_ENGINE_API UIBaseNodeClickedEventArgs
{
public:
    UIBaseNodeClickedEventArgs(std::shared_ptr<CUIBaseNode> Initiator)
        : Initiator(Initiator)
    {}

    std::shared_ptr<CUIBaseNode> Initiator;
};
typedef Delegate<UIBaseNodeClickedEventArgs> UIBaseNodeClickedEvent;


class OW_ENGINE_API CUIBaseNode : public SceneNode
{
    typedef SceneNode base;
	friend CUIWindowNode;
public:
	explicit                                        CUIBaseNode();
	virtual                                         ~CUIBaseNode();

    std::shared_ptr<CUIBaseNode>                    shared_from_this();
    std::weak_ptr<CUIBaseNode>                      weak_from_this();

    // Components engine
    virtual void                                    RegisterComponents() override;
    // Components engine template access
    template<typename T>
    std::shared_ptr<T>                              GetComponent();

	// Size & bounds functional
    virtual glm::vec2                               GetSize();
    virtual BoundingRect                            GetBoundsAbs();
    virtual bool                                    IsPointInBoundsAbs(glm::vec2 Point) ;

	// Called before all others calls
	virtual void                                    UpdateViewport(const Viewport* viewport);

	// Allow a visitor to visit this node. 
	virtual bool                                    Accept(std::shared_ptr<IVisitor> visitor);
	virtual bool                                    AcceptMesh(std::shared_ptr<IVisitor> visitor);

	// Input events
	virtual bool                                    OnKeyPressed(KeyEventArgs& e);
	virtual void                                    OnKeyReleased(KeyEventArgs& e);
	virtual void                                    OnMouseMoved(MouseMotionEventArgs& e);
	virtual bool                                    OnMouseButtonPressed(MouseButtonEventArgs& e);
	virtual void                                    OnMouseButtonReleased(MouseButtonEventArgs& e);
	virtual bool                                    OnMouseWheel(MouseWheelEventArgs& e);

    UIBaseNodeClickedEvent Clicked;

	// Syntetic events
	virtual void                                    OnMouseEntered();
	virtual void                                    OnMouseLeaved();

public: // Syntetic events // TODO: Make private
	bool                                            IsMouseOnNode() const;
	void                                            DoMouseEntered();
	void                                            DoMouseLeaved();

protected:
    void                                            SetTransformComponent(std::shared_ptr<CTransformComponentUI> TransformComponent);

private:
    std::shared_ptr<CTransformComponentUI>          m_Components_Transform;

private: // Syntetic events
	bool                                            m_IsMouseOnNode;
};

#include "SceneNodeUI.inl"
