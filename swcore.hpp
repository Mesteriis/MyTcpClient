#ifndef SWCORE_HPP
#define SWCORE_HPP

#include <QObject>
#include <QTcpSocket>
#include <QDataStream>
#include <QTime>
#include <QDate>
#include <QThread>


class swcore : public QObject{
    Q_OBJECT
    Q_PROPERTY(quint16 portServ READ port WRITE setPortServ NOTIFY portServChanged)
    Q_PROPERTY(QString nameClient READ nameClient WRITE setNameClient NOTIFY nameClientChanged)
    Q_PROPERTY(quint16 nextBlockSize READ nextBlockSize WRITE slotSetNextBlockSize NOTIFY nextBlockSizeChanged)


public:
    swcore(const QString& strHost, int nPort, QString nameClient, QObject *parent = 0);
    ~swcore();
    quint16 port() const;
    QString nameClient() const;
    quint16 nextBlockSize() const;
    QString timeStamp();

public slots:
    void setPortServ(quint16 portServ);
    void setNameClient(QString nameClient);

    void slotReadyRead ();
    void slotError (QAbstractSocket::SocketError err);
    void slotSendToServer (QString str);
    void slotConnected ();
    void slotConnectToServ ();
    void slotSetNextBlockSize (quint16 nextBlockSize);

signals:
    void portServChanged (quint16 portServ);
    void nameClientChanged (QString nameClient);
    void nextBlockSizeChanged (quint16 nextBlockSize);
    void sentFromServer (QString);
    void systemMsg (QString);

private:
    QTcpSocket *m_socket;
    quint16 m_portServ;
    QHostAddress *m_addresServ;
    QString m_nameClient;
    quint16 m_nextBlockSize;

};

#endif // SWCORE_HPP
