#include "mainwindow.h"

#include <QApplication>
#include "flwidget_linux.h"

int main(int argc, char *argv[])
{
	QApplication a(argc, argv);
//	MainWindow w;
//	w.show();

	FLWidget_Linux w;
	w.show();
	return a.exec();
}
