#include "guicore.hpp"
#include "ui_guicore.h"

guicore::guicore(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::guicore)
{
    ui->setupUi(this);
    sysThread=new QThread(this);
    swc = new swcore(sysThread);
    connect(this,   &guicore::disconnect,           swc,    &swcore::slotDisconnected);
    connect(this,   &guicore::connected,            swc,    &swcore::slotRun);
    connect(swc,    &swcore::systemMsg,             this,   &guicore::slotAddMsgToStatusBar);
    connect(swc,    &swcore::connectedChanged,      this,   &guicore::setConnectedToServ);
    connect(swc,    &swcore::broadcastChanged,      this,   &guicore::setBroadCast);
    connect(swc,    &swcore::isReadyConnectChanged, this,   &guicore::slotReadyToConnect);
    connect(swc,    &swcore::addPointToPlat,        this,   &guicore::addPointToPlat);
    connect(this,   &guicore::setParamForConnect,   swc,    &swcore::setServParam);
    /// set def param
    ui->lineEditHostAddr->setText("127.0.0.1");
    ui->lineEditPort->setText("6000");
    ui->lineEditName->setText("User");
    setConnectedToServ(false);
    ui->pushButtonStartBroadCast->setVisible(false);
    swc->setServParam(ui->lineEditHostAddr->text(),ui->lineEditPort->text().toInt(),ui->lineEditName->text());
    /// move to thread
    swc->moveToThread(sysThread);
    sysThread->start();
    /// ReagEx validator:
    /// ip ^(25[0-5]|2[0-4][0-9]|[01]?[0-9][0-9]?)\\.(25[0-5]|2[0-4][0-9]|[01]?[0-9][0-9]?)\\.(25[0-5]|2[0-4][0-9]|[01]?[0-9][0-9]?)\\.(25[0-5]|2[0-4][0-9]|[01]?[0-9][0-9]?)$"

    QRegExp ipRegex ("^(25[0-5]|2[0-4][0-9]|[01]?[0-9][0-9]?)\\."
                     "(25[0-5]|2[0-4][0-9]|[01]?[0-9][0-9]?)\\."
                     "(25[0-5]|2[0-4][0-9]|[01]?[0-9][0-9]?)\\."
                     "(25[0-5]|2[0-4][0-9]|[01]?[0-9][0-9]?)$");

    QRegExpValidator *ipValidator = new QRegExpValidator(ipRegex, this);
    ui->lineEditHostAddr->setValidator(ipValidator);

    /// ip port: sys 0-1023      "^([0-9]{1,3}|10[0-2][0-3])?$"
    ///          usr 1024-49151  "^(102[4-9]|10[3-9][0-9]|[1-5][0-9]{4}|[2-9]{4}|2[0-9]{3})?$"
    ///          din 49152—65535 "^(4915[2-9]|491[6-9][0-9]|49[2-9][0-9]{2}|5[0-9]{4}|6[0-4][0-9]{3}|65[0-4][0-9]{2}|655[0-2][0-9]|6553[0-5])?$"
    ///          all 0 - 65535   "^([0-9]{1,4}|[1-5][0-9]{4}|6[0-4][0-9]{3}|65[0-4][0-9]{2}|655[0-2][0-9]|6553[0-5])?$"

    QRegExp portRegex ("^([0-9]{1,4}|[1-5][0-9]{4}|6[0-4][0-9]{3}|65[0-4][0-9]{2}|655[0-2][0-9]|6553[0-5])?$");
    QRegExpValidator *portValidator = new QRegExpValidator(portRegex, this);
    ui->lineEditPort->setValidator(portValidator);

    /// name "^([A-Za-zА-Яа-яЁё]{3,6})?$"

    QRegExp nameRegex ("^([A-Za-zА-Яа-яЁё]{3,6})?$");
    QRegExpValidator *nameValidator = new QRegExpValidator(nameRegex, this);
    ui->lineEditName->setValidator(nameValidator);

    /// graf
    setIsExistGraf(false);
    QCPCurve *verticalLine;     // Объявляем объект для вертикальной линии
    QCPItemTracer *tracer;      // Трасировщик по точкам графика
    // Добавляем график на полотно
    verticalLine = new QCPCurve(ui->widgetGrafik->xAxis, ui->widgetGrafik->yAxis);
    ui->widgetGrafik->addGraph(ui->widgetGrafik->xAxis, ui->widgetGrafik->yAxis);
    ui->widgetGrafik->graph(0)->addData(0.0,0.0);
    // Инициализируем трассировщик
    tracer = new QCPItemTracer(ui->widgetGrafik);
    tracer->setGraph(ui->widgetGrafik->graph(0));
    // Подписываем оси координат
    ui->widgetGrafik->xAxis->setLabel("x");
    ui->widgetGrafik->yAxis->setLabel("y");
    // Устанавливаем координаты точек графика
    ui->widgetGrafik->xAxis->setRange(0,100);
    ui->widgetGrafik->yAxis->setRange(0,100);
    // Отрисовываем содержимое полотна
    ui->widgetGrafik->replot();
    setIsExistGraf(true);

}

guicore::~guicore(){
    sysThread->terminate();
    if (swc!=NULL) swc->deleteLater();
    if (sysThread!=NULL) sysThread->deleteLater();
    delete ui;
}

QString guicore::timeStamp(){
    return "| " + QDate::currentDate().toString("dd:MM:yy") + "-" + QTime::currentTime().toString() + " | ";
}

void guicore::slotAddMsgToStatusBar(QString msg){
    ui->statusbar->showMessage(tr(timeStamp().toUtf8() + msg.toUtf8()),10000);
    ui->textEditLogs->append(timeStamp()+msg);
}

void guicore::slotConnected() {
    setConnectedToServ(true);
}

void guicore::slotDisconnected(){
    setConnectedToServ(false);
}

void guicore::slotAddMsgToPlat(QString msg)
{
    ui->textEditMsqServ->append(timeStamp()+msg);
}

void guicore::setConnectedToServ(bool connectedToServ){
    if (m_connectedToServ == connectedToServ)
        return;
    if (m_connectedToServ) emit disconnect();
    m_connectedToServ = connectedToServ;
    ui->lineEditName->setDisabled(m_connectedToServ);
    ui->lineEditHostAddr->setDisabled(m_connectedToServ);
    ui->lineEditPort->setDisabled(m_connectedToServ);
    ui->labelStatus->setText(((m_connectedToServ) ? "OnLine" : "OffLine"));
    ui->labelStatus->setStyleSheet(((m_connectedToServ) ? "color: green" : "color: red"));
    ui->pushButtonStartBroadCast->setVisible( false /*connectedToServ TODO cmd*/);
    ui->labelHostAddr->setText((m_connectedToServ) ? ui->lineEditHostAddr->text() : "");
    ui->pushButtonConnetedTo->setText(((m_connectedToServ) ? "Disconnect" : "Connect"));
}

void guicore::addPointToPlat(int x, int y){
    ui->widgetGrafik->graph(0)->addData(x,y);
    ui->widgetGrafik->replot();
}

void guicore::setBroadCast(bool broadCast){
    if (m_broadCast == broadCast)
        return;

    m_broadCast = broadCast;

    (m_broadCast) ? ui->labelIsBusy->setText("ON AIR") : ui->labelIsBusy->setText("NO Signal");
    (m_broadCast) ? ui->labelIsBusy->setStyleSheet("color: green") : ui->labelIsBusy->setStyleSheet("color: red");
    if (m_broadCast)
        ui->toolBox->setCurrentIndex(0);
}

void guicore::serverSendAnswer(QString ans, QString req){
    ui->textEditLogs->setText(timeStamp()+ans+" - " + req);
    /// TODO PopUp
}

void guicore::slotReadyToConnect(bool status){
    (status) ? ui->labelStatus->setText("ready") : ui->labelStatus->setText("false") ;
}

void guicore::setIsExistGraf(bool isExistGraf){
    if (m_isExistGraf == isExistGraf)
        return;

    m_isExistGraf = isExistGraf;
    emit isExistGrafChanged(m_isExistGraf);
}

void guicore::on_pushButtonConnetedTo_clicked(){
    if (!swc->connected()){
        swc->setServParam(ui->lineEditHostAddr->text(),ui->lineEditPort->text().toInt(),ui->lineEditName->text().toUtf8());
        emit connected();
    } else {
        emit disconnect();
    }
}

void guicore::on_pushButtonStartBroadCast_clicked(){
    ui->toolBox->setCurrentIndex(0);
}

bool guicore::connectedToServ() const { return m_connectedToServ; }
bool guicore::broadCast() const { return m_broadCast; }
bool guicore::isExistGraf() const { return m_isExistGraf;}
