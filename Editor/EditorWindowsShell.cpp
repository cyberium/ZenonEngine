#include "stdafx.h"

// General
#include "EditorWindowsShell.h"

// Additional
#include "EditorQtInterfaces.h"
#include <Commdlg.h>


const char* cDialogTitle = "ZenonEngine Editor";


namespace
{
	std::string ArgsToString(const char* Text, const va_list& Args)
	{
		int len = vsnprintf(NULL, 0, Text, Args);
		if (len > 0)
		{
			std::string buff;
			buff.resize(len + 1);
			vsnprintf(&buff[0], len + 1, Text, Args);
			buff.resize(len);
			return buff;
		}
		throw CException("ArgsToString format error.");
	}
}


CEditorWindowsShell::CEditorWindowsShell(IEditor& Editor)
	: m_Editor(Editor)
{
}

CEditorWindowsShell::~CEditorWindowsShell()
{
}

CEditorWindowsShell::EDialogResult CEditorWindowsShell::ShowYesNoCancelDialog(const char* Text, ...) const
{
	auto& qtUIFrame = dynamic_cast<IEditorQtUIFrame&>(m_Editor.GetUIFrame());

	va_list args;
	va_start(args, Text);
	std::string formattedText = ArgsToString(Text, args);
	va_end(args);

	switch (MessageBoxA(qtUIFrame.getHWND(), formattedText.c_str(), cDialogTitle, MB_YESNOCANCEL | MB_ICONINFORMATION))
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

void CEditorWindowsShell::ShowErrorDialog(const char* Text, ...) const
{
	auto& qtUIFrame = dynamic_cast<IEditorQtUIFrame&>(m_Editor.GetUIFrame());

	va_list args;
	va_start(args, Text);
	std::string formattedText = ArgsToString(Text, args);
	va_end(args);

	MessageBoxA(qtUIFrame.getHWND(), formattedText.c_str(), cDialogTitle, MB_ICONERROR);
}

std::string CEditorWindowsShell::ShowLoadFileDialog(std::string DefaultName) const
{
	auto& qtUIFrame = dynamic_cast<IEditorQtUIFrame&>(m_Editor.GetUIFrame());

	char szFile[MAX_PATH];
	strcpy_s(szFile, sizeof(szFile), DefaultName.c_str());

	OPENFILENAMEA ofn = { 0 };
	ofn.lStructSize = sizeof(OPENFILENAMEA);
	ofn.hwndOwner = qtUIFrame.getHWND();
	ofn.lpstrFile = szFile;
	ofn.nMaxFile = sizeof(szFile);
	ofn.lpstrFilter = "All\0*.*\0XML\0*.XML\0";
	ofn.nFilterIndex = 0;
	ofn.lpstrFileTitle = NULL;
	ofn.nMaxFileTitle = 0;
	ofn.lpstrInitialDir = "";
	ofn.Flags = OFN_PATHMUSTEXIST | OFN_FILEMUSTEXIST;

	if (FALSE == GetOpenFileNameA(&ofn))
		throw CException("Unable show OpenFileName dialog. FileName '%s'.", szFile);

	return ofn.lpstrFile;
}

std::string CEditorWindowsShell::ShowSaveFileDialog(std::string DefaultName) const
{
	char szFile[MAX_PATH];
	strcpy_s(szFile, sizeof(szFile), DefaultName.c_str());

	// open a file name
	OPENFILENAMEA ofn = { 0 };
	ofn.lStructSize = sizeof(OPENFILENAMEA);
	ofn.hwndOwner = dynamic_cast<IEditorQtUIFrame&>(m_Editor.GetUIFrame()).getHWND();
	ofn.lpstrFile = szFile;
	ofn.nMaxFile = sizeof(szFile);
	ofn.lpstrFilter = "All\0*.*\0XML\0*.XML\0";
	ofn.nFilterIndex = 0;
	ofn.lpstrFileTitle = NULL;
	ofn.nMaxFileTitle = 0;
	ofn.lpstrInitialDir = "";
	ofn.Flags = OFN_PATHMUSTEXIST;

	if (FALSE == GetSaveFileNameA(&ofn))
		throw CException("Unable show SaveFileName dialog. FileName '%s'.", szFile);

	return ofn.lpstrFile;
}
