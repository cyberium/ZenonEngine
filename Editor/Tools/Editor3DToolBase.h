#pragma once

class CEditor3DToolBase
{
public:
	CEditor3DToolBase(IEditor3DFrame& EditorFrame);
	virtual ~CEditor3DToolBase();

	virtual void Initialize() = 0;
	virtual void Finalize() = 0;
	virtual void Enable();
	virtual bool IsEnabled();
	virtual void Disable();
	virtual bool OnMouseClickToWorld(const MouseButtonEventArgs & e, const Ray& RayToWorld) = 0;
	virtual void OnMouseReleaseToWorld(const MouseButtonEventArgs & e, const Ray& RayToWorld) = 0;
	virtual void OnMouseMoveToWorld(const MouseMotionEventArgs& e, const Ray& RayToWorld) = 0;

protected:
	IEditor3DFrame& GetEditor3DFrame() const;
	IScene* GetScene() const;
	IBaseManager& GetBaseManager() const;
	IRenderDevice& GetRenderDevice() const;

private:
	IEditor3DFrame& m_Editor3DFrame;
	bool m_IsEnabled;
};