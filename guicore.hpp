#ifndef GUICORE_HPP
#define GUICORE_HPP

#include <QMainWindow>
#include <QRegExpValidator>

#include "swcore.hpp"
#include "qcustomplot.hpp"

QT_BEGIN_NAMESPACE
namespace Ui { class guicore; }
QT_END_NAMESPACE

class guicore : public QMainWindow
{
    Q_OBJECT
    Q_PROPERTY(bool connectedToServ READ connectedToServ WRITE setConnectedToServ NOTIFY connectedToServChanged)
    Q_PROPERTY(bool broadCast       READ broadCast       WRITE setBroadCast       NOTIFY broadCastChanged)
    Q_PROPERTY(bool isExistGraf     READ isExistGraf     WRITE setIsExistGraf     NOTIFY isExistGrafChanged)
public:
    guicore(QWidget *parent = nullptr);
    ~guicore();
    QString timeStamp();
    bool connectedToServ() const;
    bool broadCast() const;
    bool isExistGraf() const;
public slots:
    void slotAddMsgToStatusBar (QString msg);
    void slotConnected();
    void slotDisconnected();
    void slotAddMsgToPlat(QString msg);
    void setConnectedToServ(bool connectedToServ);
    void addPointToPlat(int x,int y );
    void setBroadCast(bool broadCast);
    void serverSendAnswer (QString ans,QString req);
    void slotReadyToConnect(bool status);
    void setIsExistGraf(bool isExistGraf);
signals:
    void connectedToServChanged(bool connectedToServ);
    void setParamForConnect(QString addr,quint16 port,QString name);
    void connected();
    void disconnect();
    void broadCastChanged(bool broadCast);
    void isExistGrafChanged(bool isExistGraf);
//    void sendReqToServ(QString);
private slots:
    void on_pushButtonConnetedTo_clicked();
    void on_pushButtonStartBroadCast_clicked();
private:
    Ui::guicore *ui;
    swcore *swc;
    QThread *sysThread;
    bool m_connectedToServ;
    bool m_broadCast;
    bool m_isExistGraf;
};
#endif // GUICORE_HPP
