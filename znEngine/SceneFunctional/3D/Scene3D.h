#pragma once

#include "SceneFunctional/Base/Scene.h"

#include "SceneNode3D.h"

class OW_ENGINE_API Scene3D : public Scene
{
public:
	explicit                                        Scene3D();
	virtual                                         ~Scene3D();

    void                                            CreateRootNode();
	std::shared_ptr<SceneNode3D>                    GetRootNode() const;

	virtual void                                    Accept(std::shared_ptr<IVisitor> visitor) override;

    virtual void                                    OnUpdate(UpdateEventArgs& e) override;

private:
	std::shared_ptr<SceneNode3D>                    m_RootNode;
};
