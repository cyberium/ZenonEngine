#include "stdafx.h"

// General
#include "GroupVideo.h"


CGroupVideo::CGroupVideo()
{}

CGroupVideo::~CGroupVideo()
{}

void CGroupVideo::AddDefaultSettings()
{
	AddSetting("WindowSize", std::make_shared<CSettingBase<glm::vec2>>(glm::vec2(1280, 1024)));
}
