#include "stdafx.h"

// General
#include "UIUtils.h"

glm::vec2 owUIUtils::CalculateChildsVerticalSize(const std::vector<std::shared_ptr<CUIBaseNode>>& Childs, float HorizontalSpace)
{
    glm::vec2 size = glm::vec2(0.0f, 0.0f);

    for (auto child : Childs)
    {
        const glm::vec2& childSize = child->GetSize();

        size.x = std::max(childSize.x, size.x);
        size.y += childSize.y + HorizontalSpace;
    }

    return size;
}
