#pragma once

#define ONLY_SIMPLE_UI
#define ZN_FBX_SDK_ENABLE

#include "FBX/FBXInterfaces.h"

ZN_INTERFACE ZN_API IMaterialModelPass
{
	virtual ~IMaterialModelPass() {}

	virtual IShaderParameter* GetLightsShaderParameter() const = 0;
};
