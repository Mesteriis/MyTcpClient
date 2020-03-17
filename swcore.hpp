#ifndef SWCORE_HPP
#define SWCORE_HPP

#include <QObject>
#include <QTcpSocket>
#include <QDataStream>
#include <QTime>
#include <QDate>
#include <QThread>

enum msgType{
    SystemCall = 1,
    BroadCast = 2,
    MessageServer = 3
};

class swcore : public QObject{
    Q_OBJECT
    Q_PROPERTY(QString nameClient READ nameClient WRITE setNameClient NOTIFY nameClientChanged)
    Q_PROPERTY(quint16 portServ READ port WRITE setPortServ NOTIFY portServChanged)
    Q_PROPERTY(QString addresServ READ addresServ WRITE setAddresServ NOTIFY addresServChanged)
    Q_PROPERTY(bool connected READ connected WRITE setConnected NOTIFY connectedChanged)
    Q_PROPERTY(bool broadcast READ broadcast WRITE setBroadcast NOTIFY broadcastChanged)
    Q_PROPERTY(bool isReadyConnect READ isReadyConnect WRITE setIsReadyConnect NOTIFY isReadyConnectChanged)

public:
    swcore (QObject *parent = 0);
    ~swcore();
    quint16 port() const;
    QString nameClient() const;
    bool connected() const;
    bool broadcast() const;  
    QString addresServ() const;
    bool isReadyConnect() const;
public slots:
    void setServParam(QString addr,quint16 port, QString name);
    void slotRun();
    void slotDisconnected();
    void slotSendToServer (QString str); // FIXME SEND TO SERV
signals:
    void sendFromServer (QString);
    void systemMsg (QString);
    void addPointToPlat(int,int);
    /// NOTIFY
    void isReadyConnectChanged(bool isReadyConnect);
    void addresServChanged(QString addresServ);
    void connectedChanged(bool connected);
    void broadcastChanged(bool broadcast);
    void portServChanged (quint16 portServ);
    void nameClientChanged (QString nameClient);

private:
    /// METODS
    void setAddresServ(QString addresServ);
    void setPortServ(quint16 portServ);
    void setNameClient(QString nameClient);
    void conn(const QString& strHost, int nPort, QString clientName);
    void setIsReadyConnect(bool isReadyConnect);
    QString timeStamp();
    ///
    void slotReadyRead ();
    void slotError (QAbstractSocket::SocketError err);
    void setConnected(bool connected);
    void setBroadcast(bool broadcast);
    void slotConnected ();

    QTcpSocket *m_socket;
    quint16 m_portServ;
    QString m_nameClient;
    bool m_connected = false;
    bool m_broadcast = false;
    int m_serverRequestCounter = 0;
//    ServerRequest sr;
//    QMap <int,ServerRequest> arrRequestToServ;
    QString m_addresServ = nullptr;
    bool m_isReadyConnect = false;
};

#endif // SWCORE_HPP
