#pragma once

const glm::vec4 COLOR_EMPTY = glm::vec4(0.0f, 0.0f, 0.0f, 0.0f);

const glm::vec4 COLOR_BLACK = glm::vec4(0.0f, 0.0f, 0.0f, 1.0f);
const glm::vec4 COLOR_DARKGRAY = glm::vec4(0.3f, 0.3f, 0.3f, 1.0f);
const glm::vec4 COLOR_GRAY = glm::vec4(0.5f, 0.5f, 0.5f, 1.0f);
const glm::vec4 COLOR_LIGHTGRAY = glm::vec4(0.7f, 0.7f, 0.7f, 1.0f);
const glm::vec4 COLOR_WHITE = glm::vec4(1.0f, 1.0f, 1.0f, 1.0f);

const glm::vec4 COLOR_RED = glm::vec4(1.0f, 0.0f, 0.0f, 1.0f);
const glm::vec4 COLOR_GREEN = glm::vec4(0.0f, 1.0f, 0.0f, 1.0f);
const glm::vec4 COLOR_BLUE = glm::vec4(0.0f, 0.0f, 1.0f, 1.0f);

const glm::vec4 COLOR_LIGHTBLUE = glm::vec4(0.0f, 1.0f, 1.0f, 1.0f);
const glm::vec4 COLOR_PINK = glm::vec4(1.0f, 0.0f, 1.0f, 1.0f);
const glm::vec4 COLOR_YELLOW = glm::vec4(1.0f, 1.0f, 0.0f, 1.0f);

inline glm::vec4 fromARGB(uint32 color)
{
    const float a = ((color & 0xFF000000) >> 24) / 255.0f;
    const float r = ((color & 0x00FF0000) >> 16) / 255.0f;
    const float g = ((color & 0x0000FF00) >> 8) / 255.0f;
    const float b = ((color & 0x000000FF)) / 255.0f;
    return glm::vec4(r, g, b, a);
}

inline glm::vec3 fromRGB(uint32 color)
{
    const float r = ((color & 0xFF0000) >> 16) / 255.0f;
    const float g = ((color & 0x00FF00) >> 8) / 255.0f;
    const float b = ((color & 0x0000FF)) / 255.0f;
    return glm::vec3(r, g, b);
}

inline glm::vec4 fromBGRA(uint32 color)
{
    const float b = ((color & 0xFF000000) >> 24) / 255.0f;
    const float g = ((color & 0x00FF0000) >> 16) / 255.0f;
    const float r = ((color & 0x0000FF00) >> 8) / 255.0f;
    const float a = ((color & 0x000000FF)) / 255.0f;
    return glm::vec4(r, g, b, a);
}

inline glm::vec4 fromABGR(uint32 color)
{
    const float a = ((color & 0xFF000000) >> 24) / 255.0f;
    const float b = ((color & 0x00FF0000) >> 16) / 255.0f;
    const float g = ((color & 0x0000FF00) >> 8) / 255.0f;
    const float r = ((color & 0x000000FF)) / 255.0f;
    return glm::vec4(r, g, b, a);
}
