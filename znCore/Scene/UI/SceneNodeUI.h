#pragma once

#include "../SceneNodeBase.h"

// FORWARD BEGIN
class CUIBaseNode;
class CUIWindowNode;
// FORWARD END


class ZN_API UIBaseNodeClickedEventArgs
{
public:
    UIBaseNodeClickedEventArgs(std::shared_ptr<CUIBaseNode> Initiator)
        : Initiator(Initiator)
    {}

    std::shared_ptr<CUIBaseNode> Initiator;
};
typedef Delegate<UIBaseNodeClickedEventArgs> UIBaseNodeClickedEvent;


class ZN_API CUIBaseNode 
	: public SceneNodeBase
	, public ISceneNodeUI
{
	friend IScene;
public:
	explicit CUIBaseNode();
	virtual ~CUIBaseNode();

    std::shared_ptr<CUIBaseNode>                    shared_from_this();
    std::weak_ptr<CUIBaseNode>                      weak_from_this();

	// ISceneNodeUI
	void											SetTranslate(cvec2 _translate);
	cvec2											GetTranslation() const;
	glm::vec2										GetTranslationAbs() const;
	void											SetRotation(cvec3 _rotate);
	cvec3											GetRotation() const;
	void											SetScale(cvec2 _scale);
	cvec2											GetScale() const;
	glm::vec2										GetScaleAbs() const;


    // Components engine
    virtual void                                    RegisterComponents() override;


	// Size & bounds functional
    virtual glm::vec2                               GetSize();
    virtual BoundingRect                            GetBoundsAbs();
    virtual bool                                    IsPointInBoundsAbs(glm::vec2 Point) ;


	// Allow a visitor to visit this node. 
	virtual bool                                    Accept(IVisitor* visitor) override;
	virtual bool                                    AcceptMesh(IVisitor* visitor);


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


protected:
	virtual void									UpdateLocalTransform() override;
	virtual void									UpdateWorldTransform() override;


public: // Syntetic events // TODO: Make private
	bool                                            IsMouseOnNode() const;
	void                                            DoMouseEntered();
	void                                            DoMouseLeaved();


private:
	vec2											m_Translate;
	vec3											m_Rotate;
	vec2											m_Scale;

private: // Syntetic events
	bool                                            m_IsMouseOnNode;
};

