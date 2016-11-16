#include "dialog.h"
#include <QApplication>

int main(int argc, char *argv[])
{
    //test!
	QApplication a(argc, argv);
	Dialog w;
	w.show();

	return a.exec();
}
