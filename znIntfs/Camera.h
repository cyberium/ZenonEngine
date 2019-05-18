#pragma once

// FORWARD BEGIN
class SceneNode3D;
// FORWARD END

struct IznCamera
{
    virtual glm::mat4 GetViewMatrix() const = 0;
    virtual glm::mat4 GetProjectionMatrix() const = 0;
    virtual glm::mat4 GetViewProjectionInverseMatrix() const = 0;
};