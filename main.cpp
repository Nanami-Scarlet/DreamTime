#include "MainWidget.h"
#include "DataManager.h"

#include <QtWidgets/QApplication>

int main(int argc, char* argv[])
{
	DataManager::getInstance()->readData();

	QApplication a(argc, argv);
	MainWidget w;
	w.show();
	return a.exec();
}
