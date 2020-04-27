#ifndef FILESERVER_H
#define FILESERVER_H
#include <QTcpServer>
#include <QObject>
#include "filetcpsocket.h"


class fileserver : public QTcpServer
{
    Q_OBJECT
public:
    fileserver(QObject *parent = 0,int fileport = 0);
    QList<filetcpsocket*> filetcpClientSocketList;
public slots:
    //void informClients(QByteArray);
    void slotDisconnected(int);
    void slotUpdateFileInfo(QString,QString,bool);
protected:
    void incomingConnection(int sockectDescriptor);
signals:
    void slotUploadInfo(QString,QString,bool);
};

#endif // FILESERVER_H
