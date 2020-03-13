#include "swcore.hpp"

swcore::swcore(const QString &strHost, int nPort,QString nameClient, QObject *parent){
    m_socket = new QTcpSocket(parent);
    setPortServ(nPort);
    setNameClient(nameClient);

    m_socket->connectToHost(strHost, nPort);
    connect(m_socket, &QTcpSocket::connected, this, &swcore::slotConnected);
    connect(m_socket, &QTcpSocket::readyRead, this, &swcore::slotReadyRead);
    connect(m_socket, SIGNAL(error(QAbstractSocket::SocketError)), this, SLOT(QAbstractSocket::SocketError));
//    connect(m_socket, &QAbstractSocket::error, this, &swcore::slotError);
}

swcore::~swcore(){
    this->deleteLater();
}

quint16 swcore::port() const{
    return m_portServ;
}

QString swcore::nameClient() const{
    return m_nameClient;
}

quint16 swcore::nextBlockSize() const{
    return m_nextBlockSize;
}

QString swcore::timeStamp()
{
    return "| " + QDate::currentDate().toString("dd:MM:yy") + "-" + QTime::currentTime().toString() + " | ";
}

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

void swcore::slotReadyRead(){
    QDataStream in(m_socket);
        in.setVersion(QDataStream::Qt_4_2);
        while (true) {
            if (!m_nextBlockSize) {
                if (m_socket->bytesAvailable() < sizeof(quint16)) {
                    break;
                }
            in >> m_nextBlockSize;
        }
        if (m_socket->bytesAvailable() < m_nextBlockSize)
            break;
        QTime   time;
        QString str;
        in >> time >> str;
        emit sentFromServer(str);
        m_nextBlockSize = 0;
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
}

void swcore::slotSendToServer(QString str){
    QByteArray  arrBlock;
    QDataStream out(&arrBlock, QIODevice::WriteOnly);
    out.setVersion(QDataStream::Qt_4_2);
    out << quint16(0) << str;
    out.device()->seek(0);
    out << quint16(arrBlock.size() - sizeof(quint16));
    m_socket->write(arrBlock);
}

void swcore::slotConnected(){
    emit systemMsg("Received the connected() signal");
}

void swcore::slotConnectToServ(){
    m_socket=new QTcpSocket;
//    m_socket->connectToHost(m_addresServ,m_portServ);
}

void swcore::slotSetNextBlockSize(quint16 nextBlockSize){
    if (m_nextBlockSize == nextBlockSize)
        return;

    m_nextBlockSize = nextBlockSize;
    emit nextBlockSizeChanged(m_nextBlockSize);
}


