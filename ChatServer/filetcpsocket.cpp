#include "filetcpsocket.h"

filetcpsocket::filetcpsocket()
{
    fileBytes = nameSize = gotBytes = 0;
    file = Q_NULLPTR;
    connect(this,SIGNAL(readyRead()),this,SLOT(fileReceived()));
    connect(this,SIGNAL(disconnected()),this,SLOT(slotDisconnected()));
}

void filetcpsocket::fileReceived()
{
    QDataStream in(this);
    in.setVersion(QDataStream::Qt_5_9);
    if(gotBytes <= 2*sizeof (qint64))
    {
        if(!nameSize)
        {
            if(this->bytesAvailable()>=2*sizeof(qint64)){
                in>>fileBytes>>nameSize;
                gotBytes += 2 * sizeof (qint64);
            }else{
                return;
            }
        }else if(this->bytesAvailable() >= nameSize)
        {
            in>>fileName>>userName;
            gotBytes += nameSize;
        }else{
            return;
        }
    }

    if(!fileName.isEmpty() && file == Q_NULLPTR)
    {
        file = new QFile(fileName);
        if(!file->open(QFile::WriteOnly))
        {
            delete file;
            file = Q_NULLPTR;
            return;
        }
    }
    if(file == Q_NULLPTR)
    {
        return;
    }

    if(gotBytes < fileBytes)
    {
        gotBytes += this->bytesAvailable();
        file->write(this->readAll());
    }
    qDebug()<<"gb"<<gotBytes<<"d"<<fileBytes;
    if(gotBytes == fileBytes)
    {
        if(Util::isPicture(fileName))
        {
            emit sigSendFileInfo(fileName,userName,true);
        }else{
            emit sigSendFileInfo(fileName,userName,false);
        }
        this->close();
        file->close();
        delete file;
    }
}

void filetcpsocket::slotDisconnected()
{

}


