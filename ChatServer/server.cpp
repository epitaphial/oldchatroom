#include "server.h"

server::server(QObject *parent,int port):QTcpServer(parent)
{
    listen(QHostAddress::Any,port);
}

void server::incomingConnection(int sockectDescriptor)
{
    tcpsocket *tcpClientSocket = new tcpsocket();
    connect(tcpClientSocket,SIGNAL(updateClients(QByteArray,int)),this,SLOT(updateClients(QByteArray,int)));
    connect(tcpClientSocket,SIGNAL(disconnected(int)),this,SLOT(slotDisconnected(int)));
    connect(tcpClientSocket,SIGNAL(updateMember(QString)),this,SLOT(updateMember(QString)));
    tcpClientSocket->setSocketDescriptor(sockectDescriptor);
    tcpClientSocketList.append(tcpClientSocket);
}

void server::updateClients(QByteArray qb,int length)
{
    for(int i = 0;i<tcpClientSocketList.count();i++)
    {
        tcpsocket *item = tcpClientSocketList.at(i);
        if(item->write(qb,length)!=length)
        {
            continue;
        }
    }
}

void server::slotDisconnected(int descriptor)
{
    for(int i = 0;i < tcpClientSocketList.count();i++)
    {
        QTcpSocket *item = tcpClientSocketList.at(i);
        if(item->socketDescriptor() == descriptor)
        {
            QByteArray leavemsg = (clientNameList.at(i) + tr("离开了房间\n")).toLocal8Bit();
            updateClients(leavemsg,leavemsg.length());
            emit sigDeleteMember(clientNameList.at(i));
            tcpClientSocketList.removeAt(i);
            clientNameList.removeAt(i);
            return;
        }
    }
    return;
}

void server::updateMember(QString userName)
{
    clientNameList.append(userName);
    emit sigUpdateMember(userName);
}
