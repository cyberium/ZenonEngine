#include "stdafx.h"

// General
#include "FBXSceneNode.h"

// Additional
#include "FBX.h"

CFBXSceneNode::CFBXSceneNode(const std::string& SceneFile)
	: m_SceneFilename(SceneFile)
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
