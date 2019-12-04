#pragma once

class OW_ENGINE_API SceneBase 
	: public IScene
	, public IBaseManagerHolder
	, public Object
{
public:
	SceneBase(std::shared_ptr<IBaseManager> BaseManager);
	virtual ~SceneBase();

	// IScene
	std::shared_ptr<ISceneNode>                     GetRootNode() const;
	void                                            Accept(IVisitor* visitor);
    void                                            OnUpdate(UpdateEventArgs& e);

	// IBaseManagerHolder
	std::shared_ptr<IBaseManager>                   GetBaseManager() const override final;

protected:
	std::shared_ptr<ISceneNode>                     m_RootNode;

private:
	std::shared_ptr<IBaseManager>                   m_BaseManager;
};
