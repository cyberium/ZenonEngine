#pragma once

#include "FBXDisplayCommon.h"

class CFBX;

std::shared_ptr<IMesh> DisplayMesh(CFBX * FBX, FbxManager* Manager, FbxNode* pNode);
