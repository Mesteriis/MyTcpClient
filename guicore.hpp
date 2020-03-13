#ifndef GUICORE_HPP
#define GUICORE_HPP

#include <QMainWindow>

#include "swcore.hpp"
#include "qcustomplot.hpp"

QT_BEGIN_NAMESPACE
namespace Ui { class guicore; }
QT_END_NAMESPACE

class guicore : public QMainWindow
{
    Q_OBJECT

public:
    guicore(QWidget *parent = nullptr);
    ~guicore();

    QString timeStamp();

public slots:
    void slotAddMsgToStatusBar (QString msg);

private:
    Ui::guicore *ui;
    swcore *swc;
    QThread *sysThread;
};
#endif // GUICORE_HPP
