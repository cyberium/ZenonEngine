#pragma once

class ZN_API CFBXSceneNode
	: public SceneNode3D
{
public:
	CFBXSceneNode(const std::string& SceneFile);
	virtual ~CFBXSceneNode();

	// ISceneNode
	void Initialize() override;

private:
	std::string m_SceneFilename;
};