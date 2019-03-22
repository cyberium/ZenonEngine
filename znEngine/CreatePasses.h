#pragma once

// FORWARD BEGIN
class RenderTechnique;
class RenderUITechnique;

class Scene3D;
class CUIScene;
// FORWARD END

void AddDebugPasses(std::shared_ptr<IRenderDevice> device, std::shared_ptr<IRenderTarget> _renderTarget, RenderTechnique * technique, const Viewport& viewport, std::shared_ptr<Scene3D> scene);
void AddFont3DPass(std::shared_ptr<IRenderDevice> device, std::shared_ptr<IRenderTarget> _renderTarget, RenderTechnique * technique, const Viewport& viewport, std::shared_ptr<Scene3D> scene);

void AddUIPasses(std::shared_ptr<IRenderDevice> device, std::shared_ptr<IRenderTarget> _renderTarget, RenderUITechnique * technique, const Viewport& viewport, std::shared_ptr<CUIScene> scene);