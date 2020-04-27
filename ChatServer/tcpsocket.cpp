#include "tcpsocket.h"

tcpsocket::tcpsocket()
{
    connect(this,SIGNAL(readyRead()),this,SLOT(dataReceived()));
    connect(this,SIGNAL(disconnected()),this,SLOT(slotDisconnected()));
}

void tcpsocket::dataReceived()
{
    while (bytesAvailable()>0) {
        int length = bytesAvailable();
        char buf[65536];
        QByteArray qb;
        read(buf,length);
        qb = QByteArray(buf);
        if(qb.mid(0,3) == "\x05\x02\x01")
        {
            emit updateClients(qb.mid(3,length-3)+QString("进入了房间\n").toLocal8Bit(),length-3+QString("进入了房间\n").toLocal8Bit().length());
            emit updateMember(QString::fromLocal8Bit(qb.mid(3,length-3)));
        }else{
            emit updateClients(qb,length);
        }
    }
}

void tcpsocket::slotDisconnected()
{
    emit disconnected(this->socketDescriptor());
}
