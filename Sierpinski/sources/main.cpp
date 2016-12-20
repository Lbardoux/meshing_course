#include "mainwindow.h"
#include <QApplication>
#include <ctime>
#include <cstdlib>
#include "Mesh.hpp"
#include "logs.hpp"

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
	srand(time(nullptr));
	mtl::log::Options::ENABLE_COLOR = true;
    MainWindow w;
    w.show();
    
    return a.exec();
}
