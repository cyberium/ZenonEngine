#pragma once

#include "SceneFunctional/Base/SceneBase.h"

class OW_ENGINE_API Scene3D : public SceneBase
{
public:
	explicit                                        Scene3D(std::shared_ptr<IBaseManager> BaseManager);
	virtual                                         ~Scene3D();

    void                                            CreateRootNode();
};
