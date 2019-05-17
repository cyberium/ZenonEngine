#pragma once

class IRenderUIPass : public IVisitor, public Object
{
public:
	virtual void SetEnabled(bool enabled) = 0;
	virtual bool IsEnabled() const = 0;

	virtual void RenderUI(RenderUIEventArgs& e) = 0;

	// Update viewport (need for texture resizing)
	virtual void UpdateViewport(Viewport _viewport) = 0;
};