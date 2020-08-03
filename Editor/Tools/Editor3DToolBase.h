#pragma once

class CEditor3DToolBase
	: public IEditorTool
{
public:
	CEditor3DToolBase(IEditor3DFrame& EditorFrame);
	virtual ~CEditor3DToolBase();

	// IEditorTool
	virtual void Initialize() = 0;
	virtual void Finalize() = 0;
	virtual void Enable() override;
	virtual void Disable() override;
	bool IsEnabled() const override final;

	// CEditor3DToolBase
	virtual bool OnMousePressed(const MouseButtonEventArgs& e, const Ray& RayToWorld) = 0;
	virtual void OnMouseReleased(const MouseButtonEventArgs& e, const Ray& RayToWorld) = 0;
	virtual void OnMouseMoved(const MouseMotionEventArgs& e, const Ray& RayToWorld) = 0;

protected:
	IEditor3DFrame& GetEditor3DFrame() const;
	IScene* GetScene() const;
	IBaseManager& GetBaseManager() const;
	IRenderDevice& GetRenderDevice() const;

private:
	IEditor3DFrame& m_Editor3DFrame;
	bool m_IsEnabled;
};