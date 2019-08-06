#include "stdafx.h"
#include "MainEditor.h"
#include <QtWidgets/QApplication>

int main(int argc, char *argv[])
{
	QApplication a(argc, argv);
	MainEditor w;
	w.show();
	return a.exec();
}
