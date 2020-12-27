#include "stdafx.h"

// General
#include "EditorTextureSelector.h"

CEditorTextureSelector::CEditorTextureSelector(QWidget * Parent, IEditor& Editor)
	: QMainWindow(Parent)
	, m_Editor(Editor)
{
	m_UI.setupUi(this);
}

CEditorTextureSelector::~CEditorTextureSelector()
{
}



//
// Protected
//
void CEditorTextureSelector::closeEvent(QCloseEvent * event)
{
	Log::Error("Closed");
}
