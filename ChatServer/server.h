#ifndef SERVER_H
#define SERVER_H

#include <QTcpServer>
#include <QObject>
#include "tcpsocket.h"


class server : public QTcpServer
{
    Q_OBJECT
public:
    server(QObject *parent = 0,int port = 0);
    QList<tcpsocket*> tcpClientSocketList;
    QList<QString> clientNameList;
public slots:
    void updateClients(QByteArray,int);
    void slotDisconnected(int);
    void updateMember(QString);
signals:
    void sigUpdateMember(QString);
    void sigDeleteMember(QString);
protected:
    void incomingConnection(int sockectDescriptor);
};

#endif // SERVER_H
