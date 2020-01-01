#pragma once

#include "FBXDisplayCommon.h"

std::shared_ptr<IMesh> DisplayMesh(FbxNode* pNode);
std::shared_ptr<IMaterial> DisplayMaterialMapping(FbxMesh* pMesh);
void DisplayMaterialConnections(FbxMesh* pMesh);