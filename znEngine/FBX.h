#pragma once

#include "SceneFunctional/3D/SceneNode3D.h"

class OW_ENGINE_API CFBX
{
public:
	CFBX(std::shared_ptr<SceneNode3D> ParentNode);
	virtual ~CFBX();

	std::shared_ptr<SceneNode3D> GetSceneNode();
	std::shared_ptr<Material> GetDefaultMaterial();

private:
	std::shared_ptr<SceneNode3D> m_Node;
	std::shared_ptr<Material> m_DefaultMaterial;
};