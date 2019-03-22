#pragma once

#include "SceneNode3D.h"

class Scene3D : public Object
{
public:
	Scene3D();
	virtual ~Scene3D();

	std::shared_ptr<SceneNode3D> GetRootNode() const;

	void Accept(IVisitor& visitor);

	void OnUpdate(UpdateEventArgs& e);

private:
	std::shared_ptr<SceneNode3D> m_pRootNode;
};
