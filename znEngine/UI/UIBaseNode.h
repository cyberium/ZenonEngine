#pragma once

// FORWARD BEGIN
class CUIBaseNode;
class CUIWindowNode;
// FORWARD END


class UIBaseNodeMovedEventArgs
{};
typedef Delegate<UIBaseNodeMovedEventArgs> UIBaseNodeMovedEvent;


class UIBaseNodeClickedEventArgs
{
public:
    UIBaseNodeClickedEventArgs(std::shared_ptr<CUIBaseNode> Initiator)
        : Initiator(Initiator)
    {}

    std::shared_ptr<CUIBaseNode> Initiator;
};
typedef Delegate<UIBaseNodeClickedEventArgs> UIBaseNodeClickedEvent;


class CUIBaseNode : public Object, public std::enable_shared_from_this<CUIBaseNode>
{
	friend CUIWindowNode;

	typedef Object base;
public:
	explicit CUIBaseNode();
	virtual ~CUIBaseNode();

	virtual cstring GetName() const;
	virtual void SetName(cstring name);

    // Translate functional
	void SetTranslate(cvec2 _translate);
	glm::vec2 GetTranslation() const;
	glm::vec2 GetTranslationAbs() const;
    UIBaseNodeMovedEvent Moved;

    // Rotate functional
	void SetRotation(cvec3 _rotate);
	glm::vec3 GetRotation() const;

    // Scale functional
	void SetScale(cvec2 _scale);
	glm::vec2 GetScale() const;
    glm::vec2 GetScaleAbs() const;

	// Size & bounds functional
    virtual glm::vec2 GetSize() const;
    virtual BoundingRect GetBoundsAbs() const;
    virtual bool IsPointInBoundsAbs(glm::vec2 Point) const;

	mat4 GetLocalTransform() const;
	void SetLocalTransform(cmat4 localTransform);

	mat4 GetWorldTransfom() const;
	void SetWorldTransform(cmat4 worldTransform);

	// Parent & childs functional
	virtual void SetParent(std::weak_ptr<CUIBaseNode> parent);         // Call this from CUIWindowNode
	virtual void SetParentInternal(std::weak_ptr<CUIBaseNode> parent); // Call this from others nodes
    
    virtual std::shared_ptr<CUIBaseNode> GetParent() const;
    virtual std::vector<std::shared_ptr<CUIBaseNode>> GetChilds() const;

	// Called before all others calls
	virtual void UpdateViewport(const Viewport* viewport);

	// Allow a visitor to visit this node. 
	virtual bool Accept(IVisitor& visitor);
	virtual bool AcceptMesh(IVisitor& visitor);

	// Input events
	virtual bool OnKeyPressed(KeyEventArgs& e);
	virtual void OnKeyReleased(KeyEventArgs& e);
	virtual void OnMouseMoved(MouseMotionEventArgs& e);
	virtual bool OnMouseButtonPressed(MouseButtonEventArgs& e);
	virtual void OnMouseButtonReleased(MouseButtonEventArgs& e);
	virtual bool OnMouseWheel(MouseWheelEventArgs& e);

    UIBaseNodeClickedEvent Clicked;

	// Syntetic events
	virtual void OnMouseEntered();
	virtual void OnMouseLeaved();

protected:
	virtual mat4 GetParentWorldTransform() const;

	virtual void UpdateLocalTransform();

public: // Syntetic events // TODO: Make private
	bool IsMouseOnNode() const;
	void DoMouseEntered();
	void DoMouseLeaved();

protected:
    std::weak_ptr<CUIBaseNode>	m_pParentNode;

private:
	std::string                 m_Name;

	vec2                        m_Translate;
	vec3				        m_Rotate;
	vec2                        m_Scale;

	mat4                        m_LocalTransform;

private: // Syntetic events
	bool                        m_IsMouseOnNode;
};