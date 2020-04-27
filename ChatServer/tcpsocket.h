#ifndef TCPSOCKET_H
#define TCPSOCKET_H

#include <QTcpSocket>
#include <QObject>


class tcpsocket : public QTcpSocket
{
    Q_OBJECT
public:
    tcpsocket();
signals:
    void updateClients(QByteArray,int);
    void disconnected(int);
    void updateMember(QString);
protected slots:
    void dataReceived();
    void slotDisconnected();
};

#endif // TCPSOCKET_H
