#pragma once

// FORWARD BEGIN
class RenderTechnique;
// FORWARD END

void OW_ENGINE_API Add3DPasses(std::shared_ptr<IRenderDevice> device, std::shared_ptr<IRenderTarget> _renderTarget, RenderTechnique * technique, const Viewport * viewport, std::shared_ptr<IScene> scene);
void OW_ENGINE_API AddUIPasses(std::shared_ptr<IRenderDevice> device, std::shared_ptr<IRenderTarget> _renderTarget, RenderTechnique * technique, const Viewport * viewport, std::shared_ptr<IScene> scene);
