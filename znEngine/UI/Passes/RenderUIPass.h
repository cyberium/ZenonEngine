#pragma once

class IRenderUIPass : public IVisitor, public Object
{
public:
	virtual void SetEnabled(bool enabled) = 0;
	virtual bool IsEnabled() const = 0;

	virtual void RenderUI(RenderUIEventArgs& e) = 0;

	// Inherited from Visitor
	virtual bool Visit(SceneNode3D& node3D) = 0;
	virtual bool Visit(CUIBaseNode& nodeUI) = 0;
	virtual bool Visit(IMesh& Mesh, UINT IndexStartLocation = 0, UINT IndexCnt = 0, UINT VertexStartLocation = 0, UINT VertexCnt = 0) = 0;
	virtual bool Visit(CLight3D& light) = 0;

	// Update viewport (need for texture resizing)
	virtual void UpdateViewport(Viewport _viewport) = 0;
};