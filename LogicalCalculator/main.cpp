#include "logicalcalculator.h"
#include <QtWidgets/QApplication>

int main(int argc, char *argv[])
{
	QApplication a(argc, argv);
	LogicalCalculator w;
	w.show();
	return a.exec();
}
