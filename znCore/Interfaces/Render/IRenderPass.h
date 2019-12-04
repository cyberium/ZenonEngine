#pragma once

// FORWARD BEGIN
class RenderEventArgs;
class Viewport;
// FORWARD END

ZN_INTERFACE OW_ENGINE_API IRenderPass : public IVisitor
{
    // Enable or disable the pass. If a pass is disabled, the technique will skip it.
    virtual void SetEnabled(bool enabled) = 0;
    virtual bool IsEnabled() const = 0;

    // Render the pass. This should only be called by the RenderTechnique.
    virtual void PreRender(RenderEventArgs& e) = 0;
    virtual void Render(RenderEventArgs& e) = 0;
    virtual void PostRender(RenderEventArgs& e) = 0;

    // Update viewport (need for texture resizing)
    virtual void UpdateViewport(const Viewport * _viewport) = 0;
};

typedef std::vector<std::shared_ptr<IRenderPass>> RenderPassList;