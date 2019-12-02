#pragma once

struct OW_ENGINE_API IznCamera
{
    virtual glm::mat4 GetViewMatrix() const = 0;
    virtual glm::mat4 GetProjectionMatrix() const = 0;
    virtual glm::mat4 GetViewProjectionInverseMatrix() const = 0;
};