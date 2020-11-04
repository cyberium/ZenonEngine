#pragma once

class CEditorWindowsShell
	: public IEditorShell
{
public:
	CEditorWindowsShell(IEditor& Editor);
	virtual ~CEditorWindowsShell();

	// IEditorShell
	EDialogResult ShowYesNoCancelDialog(const std::string& Text, const std::string& Title = "") const override;
	std::string   ShowLoadFileDialog(std::string DefaultPathAndName) const;
	std::string   ShowSaveFileDialog(std::string DefaultPath) const;

private:
	IEditor& m_Editor;
};