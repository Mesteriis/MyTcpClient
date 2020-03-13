#include "guicore.hpp"
#include "ui_guicore.h"

guicore::guicore(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::guicore)
{
    ui->setupUi(this);


}

guicore::~guicore()
{
    delete ui;
}

QString guicore::timeStamp()
{
    return "| " + QDate::currentDate().toString("dd:MM:yy") + "-" + QTime::currentTime().toString() + " | ";
}

void guicore::slotAddMsgToStatusBar(QString msg)
{
    ui->statusbar->showMessage(tr(msg.toUtf8()));
    ui->textEditLogs->append(msg);

}

