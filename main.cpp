#include "guicore.hpp"

#include <QApplication>
#include <QString>
#include <QDebug>
int main(int argc, char *argv[]){
    QApplication a(argc, argv);
    guicore w; 
    QIcon ico ("qrc:/pic/client.icns");
    w.setWindowIcon(ico);
    w.move(1200,20);
    w.show();
    return a.exec();
}
