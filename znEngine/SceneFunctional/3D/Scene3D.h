#pragma once

#include "SceneFunctional/Base/Scene.h"

#include "SceneNode3D.h"

class OW_ENGINE_API Scene3D : public Scene
{
public:
	explicit                                        Scene3D(std::shared_ptr<IBaseManager> BaseManager);
	virtual                                         ~Scene3D();

    void                                            CreateRootNode();
	std::shared_ptr<SceneNode3D>                    GetRootNode() const;

	virtual void                                    Accept(IVisitor* visitor) override;

    virtual void                                    OnUpdate(UpdateEventArgs& e) override;

private:
	std::shared_ptr<SceneNode3D>                    m_RootNode;
};
