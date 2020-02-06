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

ZN_INTERFACE ZN_API __declspec(novtable) ISceneNodeUI
	: public std::enable_shared_from_this<ISceneNodeUI>
{
	typedef std::vector<std::shared_ptr<ISceneNodeUI>>                NodeUIList;
	typedef std::multimap<std::string, std::shared_ptr<ISceneNodeUI>> NodeUINameMap;

	virtual ~ISceneNodeUI() {}

	virtual void Initialize() = 0;
	virtual void Finalize() = 0;

	// Name & Type
	virtual void SetType(std::string Type) = 0;
	virtual std::string GetType() const = 0;
	virtual void SetName(std::string Name) = 0;
	virtual std::string	GetName() const = 0;
	
	// Childs functional
	virtual void AddChild(std::shared_ptr<ISceneNodeUI> childNode) = 0;
	virtual void RemoveChild(std::shared_ptr<ISceneNodeUI> childNode) = 0;
	virtual void SetParent(ISceneNodeUI* parentNode) = 0;
	virtual ISceneNodeUI* GetParent() const = 0;
	virtual const NodeUIList& GetChilds() = 0;

	template<typename T, typename... Args> inline T* CreateSceneNode(Args &&... _Args)
	{
		return GetScene()->CreateSceneNodeUI<T>(this, std::forward<Args>(_Args)...);
	}

	// Actions & Properties
	virtual IActionsGroup* GetActions() const = 0;
	virtual IPropertiesGroup* GetProperties() const = 0;
	virtual IScene* GetScene() const = 0;

	//
	// Transform functional
	//
	virtual void SetTranslate(cvec2 _translate) = 0;
	virtual cvec2 GetTranslation() const = 0;
	virtual glm::vec2 GetTranslationAbs() const = 0;

	virtual void SetRotation(cvec3 _rotate) = 0;
	virtual cvec3 GetRotation() const = 0;

	virtual void SetScale(cvec2 _scale) = 0;
	virtual cvec2 GetScale() const = 0;
	virtual glm::vec2 GetScaleAbs() const = 0;

	virtual mat4 GetLocalTransform() const = 0;
	virtual mat4 GetWorldTransfom() const = 0;

	// Allow a visitor to visit this node.
	virtual void Accept(IVisitor* visitor) = 0;
};

ZN_INTERFACE ZN_API __declspec(novtable) ISceneNodeUIWithWrapper
{
	virtual ~ISceneNodeUIWithWrapper() {}

	virtual void SetWrapper(const ISceneNodeUI* WrapperNode) = 0;
};
