#pragma once

class OW_ENGINE_API CFBX
{
public:
	CFBX(std::shared_ptr<SceneNode3D> ParentNode);
	virtual ~CFBX();

	std::shared_ptr<SceneNode3D> GetSceneNode();
	std::shared_ptr<IMaterial> GetDefaultMaterial();

private:
	std::shared_ptr<SceneNode3D> m_Node;
	std::shared_ptr<IMaterial> m_DefaultMaterial;
};