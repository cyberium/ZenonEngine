#pragma once

class CEditorUIToolBase
	: public IEditorTool
{
public:
	CEditorUIToolBase(IEditorUIFrame& EditorFrame);
	virtual ~CEditorUIToolBase();

	// IEditorTool
	virtual void Initialize() = 0;
	virtual void Finalize() = 0;
	virtual void Enable() override;
	virtual void Disable() override;
	bool IsEnabled() const override final;

protected:
	IEditorUIFrame& GetEditorUIFrame() const;
	IScene* GetScene() const;
	IBaseManager& GetBaseManager() const;
	IRenderDevice& GetRenderDevice() const;

private:
	IEditorUIFrame& m_EditorUIFrame;
	bool m_IsEnabled;
};