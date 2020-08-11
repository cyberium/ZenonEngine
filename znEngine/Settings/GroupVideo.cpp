#include "stdafx.h"

// General
#include "GroupVideo.h"


CGroupVideo::CGroupVideo()
{}

CGroupVideo::~CGroupVideo()
{}

void CGroupVideo::AddDefaultSettings()
{
	AddSetting(MakeShared(CSettingBase<glm::vec2>, "WindowSize", "Size of ZenonEngine renderWindow.", glm::vec2(1280, 1024)));
}
