#include "stdafx.h"

// General
#include "GroupVideo.h"

CGroupVideo::CGroupVideo()
{}

CGroupVideo::~CGroupVideo()
{}

void CGroupVideo::AddDefaultSettings()
{
	AddSetting(MakeShared(CSettingBase<glm::vec2>, "WindowSize", "Engine window size.", glm::vec2(1280, 1024)));
}
