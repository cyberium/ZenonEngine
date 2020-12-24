#pragma once

#include "EditorInterfaces.h"
#include "EditorQtInterfaces.h"

#include <QMainWindow>

class CEditorTextureSelector
	: public QMainWindow
{
	Q_OBJECT

public:
	CEditorTextureSelector(QWidget * Parent, IEditor& Editor);
	virtual ~CEditorTextureSelector();

private:
	IEditor& m_Editor;
	Ui::EditorTextureSelector m_UI;

	// UI
	float m_MoverValue;
	float m_RotatorValue;
};
