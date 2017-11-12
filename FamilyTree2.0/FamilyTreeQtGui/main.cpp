#include "mainwindow.h"
#include <memory>
#include <QApplication>
#include "FamilyTreeFramework.hpp"

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    auto ftf = std::make_shared<FamilyTreeFramework>();
    MainWindow w;
    w.setFamilyTreeFramework(ftf);
    w.show();
    return a.exec();
}
