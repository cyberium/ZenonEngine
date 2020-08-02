#pragma once

namespace owUIUtils
{
    glm::vec2 CalculateChildsVerticalSize(const std::vector<std::shared_ptr<SceneNodeUI>>& Childs, float HorizontalSpace = 0.0f);
}