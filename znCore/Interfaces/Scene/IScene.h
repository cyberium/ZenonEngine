#pragma once

// FORWARD BEGIN
ZN_INTERFACE ISceneNode;
ZN_INTERFACE IVisitor;
class UpdateEventArgs;
// FORWARD END

ZN_INTERFACE OW_ENGINE_API IScene : public std::enable_shared_from_this<IScene>
{
	virtual std::shared_ptr<ISceneNode>				GetRootNode() const = 0;

	virtual void                                    Accept(IVisitor* visitor) = 0;
	virtual void                                    OnUpdate(UpdateEventArgs& e) = 0;


	// Templates


	// Creates new SceneNode and initialize it. You !must! call this method instead of creating nodes in code
	template<class T, typename... Args>
	inline std::shared_ptr<T> CreateSceneNode(std::weak_ptr<ISceneNode> Parent, Args &&... _Args)
	{
		//static_assert(std::is_convertible<T, ISceneNode>::value, "T must inherit ISceneNode as public.");

		std::shared_ptr<T> newNode = std::make_shared<T>(std::forward<Args>(_Args)...);
		newNode->SetScene(shared_from_this());
		newNode->SetParent(Parent);
		newNode->RegisterComponents();
		newNode->Initialize();
		newNode->RaiseOnParentChanged();

		return newNode;
	}
};
