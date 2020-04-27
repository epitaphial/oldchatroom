#include "fileserver.h"

fileserver::fileserver(QObject *parent,int fileport):QTcpServer(parent)
{
    listen(QHostAddress::Any,fileport);
}

void fileserver::incomingConnection(int sockectDescriptor)
{
    filetcpsocket *filetcpClientSocket = new filetcpsocket();
    connect(filetcpClientSocket,SIGNAL(disconnected(int)),this,SLOT(slotDisconnected(int)));
    connect(filetcpClientSocket,SIGNAL(sigSendFileInfo(QString,QString,bool)),this,SLOT(slotUpdateFileInfo(QString,QString,bool)));
    filetcpClientSocket->setSocketDescriptor(sockectDescriptor);
    filetcpClientSocketList.append(filetcpClientSocket);
}


void fileserver::slotDisconnected(int descriptor)
{
    for(int i = 0;i < filetcpClientSocketList.count();i++)
    {
        filetcpsocket *item = filetcpClientSocketList.at(i);
        if(item->socketDescriptor() == descriptor)
        {
            filetcpClientSocketList.removeAt(i);
            return;
        }
    }
    return;
}

void fileserver::slotUpdateFileInfo(QString fileName,QString userName,bool ifpic)
{
    emit slotUploadInfo(fileName,userName,ifpic);
}
