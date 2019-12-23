#pragma once

#include "FBXDisplayCommon.h"

std::shared_ptr<ITexture> FindAndDisplayTextureInfoByProperty(std::shared_ptr<IRenderDevice> RenderDevice, FbxProperty pProperty);
std::shared_ptr<ITexture> DisplayTextureInfo(std::shared_ptr<IRenderDevice> RenderDevice, FbxTexture* pTexture);
