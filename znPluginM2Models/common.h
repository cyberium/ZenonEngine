#pragma once

#define WOW_CLASSIC_1_12_1 1
#define WOW_BC_2_4_3       2  
#define WOW_WOTLK_3_3_5    3

#define WOW_CLIENT_VERSION WOW_WOTLK_3_3_5

#include "WowConsts.h"
#include "WowTypes.h"

ZN_MAKE_OBJECTCLASS(cDBCManager, 50);
ZN_MAKE_OBJECTCLASS(cWoWManager, 51);
ZN_MAKE_OBJECTCLASS(cSceneNodeM2, 150);