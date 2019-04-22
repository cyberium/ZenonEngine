#pragma once

class Render3DEventArgs;

class SceneNode3D;
class CUIBaseNode;
class IMesh;
class CLight3D;

class IRenderPass : public IVisitor, public Object
{
public:
	// Enable or disable the pass. If a pass is disabled, the technique will skip it.
	virtual void SetEnabled(bool enabled) = 0;
	virtual bool IsEnabled() const = 0;

	// Render the pass. This should only be called by the RenderTechnique.
	virtual void PreRender(Render3DEventArgs& e) = 0;
	virtual void Render(Render3DEventArgs& e) = 0;
	virtual void PostRender(Render3DEventArgs& e) = 0;

	// Update viewport (need for texture resizing)
	virtual void UpdateViewport(Viewport _viewport) = 0;
};