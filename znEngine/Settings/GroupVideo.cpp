#include "stdafx.h"

// General
#include "GroupVideo.h"


CGroupVideo::CGroupVideo()
{}

CGroupVideo::~CGroupVideo()
{}

void CGroupVideo::AddDefaultSettings()
{
	AddSetting("WindowSize", MakeShared(CSettingBase<glm::vec2>, glm::vec2(1280, 1024)));
}
