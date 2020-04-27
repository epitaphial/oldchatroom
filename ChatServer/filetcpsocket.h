#ifndef FILETCPSOCKET_H
#define FILETCPSOCKET_H

#include <QTcpSocket>
#include <QObject>
#include <QDataStream>
#include <QFile>
#include "util.h"

class filetcpsocket : public QTcpSocket
{
    Q_OBJECT
public:
    filetcpsocket();
signals:
    void informClients(QByteArray);
    void disconnected(int);
    void sigSendFileInfo(QString,QString,bool);
protected slots:
    void fileReceived();
    void slotDisconnected();
private:
    qint64 gotBytes,fileBytes,nameSize;
    QString fileName,userName;
    QFile *file;
};

#endif // FILETCPSOCKET_H
