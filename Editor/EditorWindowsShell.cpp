#include "stdafx.h"

// General
#include "EditorWindowsShell.h"

// Additional
#include "EditorQtInterfaces.h"
#include <Commdlg.h>

CEditorWindowsShell::CEditorWindowsShell(IEditor& Editor)
	: m_Editor(Editor)
{
}

CEditorWindowsShell::~CEditorWindowsShell()
{
}

CEditorWindowsShell::EDialogResult CEditorWindowsShell::ShowYesNoCancelDialog(const std::string& Text, const std::string& Title) const
{
	auto& qtUIFrame = dynamic_cast<IEditorQtUIFrame&>(m_Editor.GetUIFrame());

	std::string title = Title;
	if (title.empty())
		title = "ZenonEngine Editor";

	switch (MessageBoxA(qtUIFrame.getHWND(), Text.c_str(), title.c_str(), MB_YESNOCANCEL | MB_ICONINFORMATION))
	{
	case IDYES:
		return EDialogResult::Yes;
	case IDNO: //кнопка NO ?
		return EDialogResult::No;
	case IDCANCEL:
		return EDialogResult::Cancel;
	}
	
	throw CException("ShowYesNoCancelDialog unexpected result.");
}

std::string CEditorWindowsShell::ShowLoadFileDialog(std::string DefaultName) const
{
	auto& qtUIFrame = dynamic_cast<IEditorQtUIFrame&>(m_Editor.GetUIFrame());

	char szFile[256];
	strcpy_s(szFile, sizeof(szFile), DefaultName.c_str());

	std::string title = "Load ZenonEngine scene";

	OPENFILENAMEA ofn = { 0 };
	ofn.lStructSize = sizeof(OPENFILENAMEA);
	ofn.hwndOwner = qtUIFrame.getHWND();
	ofn.lpstrFile = szFile;
	ofn.nMaxFile = sizeof(szFile);
	ofn.lpstrFilter = "All\0*.*\0Text\0*.TXT\0";
	ofn.nFilterIndex = 0;
	ofn.lpstrFileTitle = NULL;
	ofn.nMaxFileTitle = 0;
	ofn.lpstrInitialDir = "";
	ofn.Flags = OFN_PATHMUSTEXIST | OFN_FILEMUSTEXIST;

	if (FALSE == GetOpenFileNameA(&ofn))
		return "";

	return ofn.lpstrFile;
}

std::string CEditorWindowsShell::ShowSaveFileDialog(std::string DefaultName) const
{
	char szFile[256];
	strcpy_s(szFile, sizeof(szFile), DefaultName.c_str());

	// open a file name
	OPENFILENAMEA ofn = { 0 };
	ofn.lStructSize = sizeof(OPENFILENAMEA);
	ofn.hwndOwner = dynamic_cast<IEditorQtUIFrame&>(m_Editor.GetUIFrame()).getHWND();
	ofn.lpstrFile = szFile;
	ofn.nMaxFile = sizeof(szFile);
	ofn.lpstrFilter = "All\0*.*\0Text\0*.TXT\0";
	ofn.nFilterIndex = 0;
	ofn.lpstrFileTitle = NULL;
	ofn.nMaxFileTitle = 0;
	ofn.lpstrInitialDir = "";
	ofn.Flags = OFN_PATHMUSTEXIST;

	if (FALSE == GetSaveFileNameA(&ofn))
		return "";

	return ofn.lpstrFile;
}
