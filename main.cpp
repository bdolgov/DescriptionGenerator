#include <QtGui/QApplication>
#include "descriptiongenerator.hpp"

int main(int argc, char *argv[])
{
	QApplication a(argc, argv);
	DescriptionGenerator w;
	w.show();
	
	return a.exec();
}
