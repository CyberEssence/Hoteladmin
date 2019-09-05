#include "hoteladmin.h"
#include <QtWidgets/QApplication>

int main(int argc, char *argv[])
{
	QApplication a(argc, argv);
	hoteladmin w;
	w.show();
	return a.exec();
}
