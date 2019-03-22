#pragma once

#include "UIBaseNode.h"

namespace owUIUtils
{
    glm::vec2 CalculateChildsVerticalSize(const std::vector<std::shared_ptr<CUIBaseNode>>& Childs, float HorizontalSpace = 0.0f);
}