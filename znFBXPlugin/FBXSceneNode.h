#pragma once

class ZN_API CFBXSceneNode
	: public SceneNode3D
{
public:
	CFBXSceneNode(const std::string& SceneFile, std::shared_ptr<IMaterial> DefaultMaterial);
	virtual ~CFBXSceneNode();

	// ISceneNode
	void Initialize() override;

private:
	std::string m_SceneFilename;
	std::shared_ptr<IMaterial> m_DefaultMaterial;
};