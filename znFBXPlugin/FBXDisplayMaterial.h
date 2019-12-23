#pragma once

#include "FBXDisplayCommon.h"

std::shared_ptr<IMaterial> DisplayMaterial(std::shared_ptr<IRenderDevice> RenderDevice, FbxGeometry* pGeometry);
