#include "guicore.hpp"

#include <QApplication>
#include <QString>
#include <QDebug>
int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    guicore w; 

    w.show();
    return a.exec();
}
