#pragma once

// FORWARD BEGIN
class RenderTechnique;
class Scene3D;
class SceneUI;
// FORWARD END

void AddUIPasses(std::shared_ptr<IRenderDevice> device, std::shared_ptr<IRenderTarget> _renderTarget, RenderTechnique * technique, const Viewport& viewport, std::shared_ptr<SceneUI> scene);
