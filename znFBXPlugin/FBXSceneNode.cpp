#include "stdafx.h"

// General
#include "FBXSceneNode.h"

// Additional
#include "FBX.h"

CFBXSceneNode::CFBXSceneNode(const std::string& SceneFile, std::shared_ptr<IMaterial> DefaultMaterial)
	: m_SceneFilename(SceneFile)
	, m_DefaultMaterial(DefaultMaterial)
{
}

CFBXSceneNode::~CFBXSceneNode()
{
}



//
// ISceneNode
//
void CFBXSceneNode::Initialize()
{
	CFBX fbx(m_SceneFilename, shared_from_this());
}
