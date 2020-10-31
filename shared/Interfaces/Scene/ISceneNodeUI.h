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

const ObjectClass cSceneNodeUI = UINT16_MAX - 200u;
const ObjectClass cSceneNodeUI_Text = UINT16_MAX - 201u;
const ObjectClass cSceneNodeUI_Color = UINT16_MAX - 202u;

ZN_INTERFACE ZN_API ISceneNodeUI
	: public IObject
    , public std::enable_shared_from_this<ISceneNodeUI>
{
	typedef std::vector<std::shared_ptr<ISceneNodeUI>>                NodeUIList;
	typedef std::multimap<std::string, std::shared_ptr<ISceneNodeUI>> NodeUINameMap;

	virtual ~ISceneNodeUI() {}

	virtual void Initialize() = 0;
	virtual void Finalize() = 0;
	
	// Childs functional
	virtual void AddChild(const std::shared_ptr<ISceneNodeUI>& childNode) = 0;
	virtual void RemoveChild(const std::shared_ptr<ISceneNodeUI>& childNode) = 0;
	virtual std::weak_ptr<ISceneNodeUI> GetParent() const = 0;
	virtual const NodeUIList& GetChilds() = 0;
	virtual void RaiseOnParentChanged() = 0;

	//template<typename T, typename... Args> inline std::shared_ptr<T> CreateSceneNode(Args &&... _Args)
	//{
	//	return GetScene()->CreateSceneNodeUI<T>(shared_from_this(), std::forward<Args>(_Args)...);
	//}

	// Actions & Properties
	virtual std::shared_ptr<IPropertiesGroup> GetProperties() const = 0;
	virtual IScene* GetScene() const = 0;

	//
	// Transform functional
	//
	virtual void SetTranslate(const glm::vec2& _translate) = 0;
	virtual const glm::vec2& GetTranslation() const = 0;
	virtual glm::vec2 GetTranslationAbs() const = 0;

	virtual void SetRotation(const glm::vec3& _rotate) = 0;
	virtual const glm::vec3& GetRotation() const = 0;

	virtual void SetScale(const glm::vec2& _scale) = 0;
	virtual const glm::vec2& GetScale() const = 0;
	virtual glm::vec2 GetScaleAbs() const = 0;

	virtual glm::mat4 GetLocalTransform() const = 0;
	virtual glm::mat4 GetWorldTransfom() const = 0;

	// Size & bounds functional
	virtual glm::vec2 GetSize() const = 0;
	virtual BoundingRect GetBoundsAbs() = 0;
	virtual bool IsPointInBoundsAbs(const glm::vec2& Point) = 0;

	// Allow a visitor to visit this node.
	virtual void Accept(IVisitor* visitor) = 0;
	virtual void AcceptMesh(IVisitor* visitor) = 0;
};
