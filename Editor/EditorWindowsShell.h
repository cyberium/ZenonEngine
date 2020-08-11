#pragma once

class CEditorWindowsShell
	: public IEditorShell
{
public:
	CEditorWindowsShell(IEditor& Editor);
	virtual ~CEditorWindowsShell();

	// IEditorShell
	std::string ShowLoadFileDialog(std::string DefaultName = "") const;
	std::string ShowSaveFileDialog(std::string DefaultName = "") const;

private:
	IEditor& m_Editor;
};