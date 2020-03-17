#include "swcore.hpp"

swcore::swcore( QObject *parent){
    setConnected(false);
    m_socket = new QTcpSocket(this);
    connect(m_socket, &QTcpSocket::connected, this, &swcore::slotConnected);
    connect(m_socket, &QTcpSocket::readyRead, this, &swcore::slotReadyRead);
    connect(m_socket, &QTcpSocket::disconnected,this,&swcore::slotDisconnected);
    connect(m_socket,   static_cast<void(QTcpSocket::*)(QAbstractSocket::SocketError)>  (&QTcpSocket::error),
            this,       static_cast<void(swcore::*)(QAbstractSocket::SocketError)>      (&swcore::slotError));
}

swcore::~swcore(){
    if (m_socket!=NULL){
        m_socket->deleteLater();
    }
    deleteLater();
}

quint16 swcore::port() const{return m_portServ;}
QString swcore::nameClient() const{return m_nameClient;}
QString swcore::timeStamp(){return "| " + QDate::currentDate().toString("dd:MM:yy") + "-" + QTime::currentTime().toString() + " | ";}
bool swcore::connected() const{return m_connected;}
bool swcore::broadcast() const{return m_broadcast;}

void swcore::conn(const QString &strHost, int nPort, QString clientName){
    setNameClient(clientName);
    if (isReadyConnect()){
        m_socket->connectToHost(strHost, nPort);
        if (m_socket->isOpen() | m_socket->isValid()) setConnected(true);
    } else {
        emit systemMsg(timeStamp() + "Server parameters are not set or not correct! Current parameters:\nip   : "+
                       addresServ() + "\nPort : " + port() + "\nName : " + nameClient());
    }
}

QString swcore::addresServ() const{return m_addresServ;}

bool swcore::isReadyConnect() const{return m_isReadyConnect;}

void swcore::setPortServ(quint16 portServ){
    if (m_portServ == portServ)
        return;

    m_portServ = portServ;
    emit portServChanged(m_portServ);
}

void swcore::setNameClient(QString nameClient){
    if (m_nameClient == nameClient)
        return;

    m_nameClient = nameClient;
    emit nameClientChanged(m_nameClient);
}

void swcore::setServParam(QString addr, quint16 port, QString name)
{
    setPortServ(port);
    setNameClient(name);
    setAddresServ(addr);
    setIsReadyConnect(true);
}

void swcore::slotRun()
{
    conn(addresServ(),port(),nameClient());
}

void swcore::slotDisconnected()
{
    setConnected(false);
    if (m_broadcast) setBroadcast(false);
}

void swcore::slotReadyRead(){
    QString msgFromServer = m_socket->readAll();
    /// удаляем символ новой строки
    /// "\n" по умолчанию думаем что этот


    bool isModify = (msgFromServer.lastIndexOf('\n') == -1) ? false : true;
    if (isModify) {
        int pos = msgFromServer.lastIndexOf(QChar('\n'));
        msgFromServer = msgFromServer.left(pos);
        isModify = false;
    }
    isModify = (msgFromServer.lastIndexOf('\r') == -1) ? false : true;
    if (isModify) {
        int pos = msgFromServer.lastIndexOf(QChar('\r'));
        msgFromServer = msgFromServer.left(pos);
        isModify = false;
    }

    QStringList listFirst = msgFromServer.split('%');



    if (listFirst.size()==2){

        switch (listFirst.at(0).toInt()) {

            case 1:{

                break;
            }

            case 2:{
                QStringList listSecond = listFirst.at(1).split('#');
                if (listSecond.size() == 2){
                    setBroadcast(true);
                    emit addPointToPlat(listSecond.at(0).toInt(),listSecond.at(1).toInt());

                }
                break;
            }

            case 3:{
                QStringList listSecond = listFirst.at(1).split('#');
                if (listSecond.size() == 2){
                    qDebug () << "MS";

                }
                break;
            }

            default:{
                break;
            }
        }
    } else {

        emit systemMsg (timeStamp() + "Unidentified Signature: "+ msgFromServer);
    }
}

void swcore::slotError(QAbstractSocket::SocketError err){
    QString strError =  "Error: " + (err == QAbstractSocket::HostNotFoundError ?
                         "The host was not found." :
                         err == QAbstractSocket::RemoteHostClosedError ?
                         "The remote host is closed." :
                         err == QAbstractSocket::ConnectionRefusedError ?
                         "The connection was refused." :
                         QString(m_socket->errorString()));
    emit systemMsg(strError);
    if (connected()) setConnected(false);
}

void swcore::slotSendToServer(QString str){
    QTextStream os(m_socket);
    os.setAutoDetectUnicode(true);
    os << str;
    m_socket->flush();
}

void swcore::slotConnected(){
    setConnected(true);
    emit systemMsg("Received the connected");
}

void swcore::setConnected(bool connected)
{
    if (m_connected == connected)
        return;
    m_connected = connected;
    emit connectedChanged(m_connected);
    if (m_connected == false){
        if (m_socket->isOpen()) m_socket->close();
    }
}

void swcore::setBroadcast(bool broadcast)
{
    if (m_broadcast == broadcast)
        return;

    m_broadcast = broadcast;
    emit broadcastChanged(m_broadcast);
}

void swcore::setAddresServ(QString addresServ)
{
    if (m_addresServ == addresServ)
        return;

    m_addresServ = addresServ;
    emit addresServChanged(m_addresServ);
}

void swcore::setIsReadyConnect(bool isReadyConnect)
{
    if (m_isReadyConnect == isReadyConnect)
        return;

    m_isReadyConnect = isReadyConnect;
    emit isReadyConnectChanged(m_isReadyConnect);
}
