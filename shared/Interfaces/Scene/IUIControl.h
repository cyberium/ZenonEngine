#pragma once

// FORWARD BEGIN
ZN_INTERFACE ISceneNodeComponent;
ZN_INTERFACE IScene;
ZN_INTERFACE ICameraComponent3D;
class Viewport;
ZN_INTERFACE IVisitor;
class UpdateEventArgs;
ZN_INTERFACE IManager;
// FORWARD END

ZN_INTERFACE ZN_API IUIControl
	: public virtual IObject
    , public std::enable_shared_from_this<IUIControl>
{
	typedef std::vector<std::shared_ptr<IUIControl>> ControlsList;

	ZN_OBJECTCLASS(cUIControl);
	virtual ~IUIControl() {}

	virtual void Initialize() = 0;
	virtual void Finalize() = 0;
	
	// Childs functional
	virtual void                                    AddChild(const std::shared_ptr<IUIControl>& childNode) = 0;
	virtual void                                    RemoveChild(const std::shared_ptr<IUIControl>& childNode) = 0;
	virtual void                                    MakeMeOrphan() = 0;
	virtual std::shared_ptr<IUIControl>             GetParent() const = 0;
	virtual const ControlsList&                     GetChilds() const = 0;
	virtual std::shared_ptr<IUIControl>             GetChild(std::string Name) const = 0;

	virtual std::shared_ptr<IPropertiesGroup>       GetProperties() const = 0;
	virtual IScene&                                 GetScene() const = 0;

	virtual void                                    SetTranslate(const glm::vec2& _translate) = 0;
	virtual const glm::vec2&                        GetTranslation() const = 0;
	virtual glm::vec2                               GetTranslationAbs() const = 0;
	virtual void                                    SetRotation(const glm::vec3& _rotate) = 0;
	virtual const glm::vec3&                        GetRotation() const = 0;
	virtual void                                    SetScale(const glm::vec2& _scale) = 0;
	virtual const glm::vec2&                        GetScale() const = 0;
	virtual glm::vec2                               GetScaleAbs() const = 0;
	virtual glm::mat4                               GetLocalTransform() const = 0;
	virtual glm::mat4                               GetWorldTransfom() const = 0;
	virtual glm::mat4								GetParentWorldTransform() const = 0;
	virtual glm::vec2                               GetSize() const = 0;
	virtual BoundingRect                            GetBoundsAbs() = 0;
	virtual bool                                    IsPointInBoundsAbs(const glm::vec2& Point) = 0;

	// Allow a visitor to visit this node
	virtual void                                    Accept(IVisitor* visitor) = 0;
};

ZN_INTERFACE ZN_API IUIControlInternal
{
	virtual ~IUIControlInternal() {}

	virtual void AddChildInternal(std::shared_ptr<IUIControl> ChildNode) = 0;     // Called from scene
	virtual void RemoveChildInternal(std::shared_ptr<IUIControl> ChildNode) = 0;  // Called from scene
	virtual void RaiseOnParentChangedInternal() = 0;
};
