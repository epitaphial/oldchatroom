#include "mainchatroom.h"

mainchatroom::mainchatroom(int port,int fileport,QHostAddress *serverIp,QString userName)
{
    setWindowTitle(tr("nmsl chat"));
    #if defined(Q_OS_LINUX)
    setWindowState(Qt::WindowMaximized);
    #endif
    mainLayout = new QGridLayout(this);
    messageList = new QTextBrowser;
    messageEdit = new QTextEdit;
    toolListLayout = new QHBoxLayout;
    imageSender = new QPushButton(tr("图片"));
    fileSender = new QPushButton(tr("文件"));
    sendmesBtn = new QPushButton(tr("发送"));
    mainLayout->addWidget(messageList,0,0,1,1);
    mainLayout->addLayout(toolListLayout,1,0);
    toolListLayout->addWidget(imageSender);
    toolListLayout->addWidget(fileSender);
    mainLayout->addWidget(messageEdit,2,0,1,1);
    mainLayout->addWidget(sendmesBtn,3,0);
    theport = port;
    thefileport = fileport;
    theip = serverIp;
    thename = userName;
    tcpSocket = new QTcpSocket(this);
    connect(tcpSocket,SIGNAL(connected()),this,SLOT(slotConnected()));
    connect(tcpSocket,SIGNAL(disconnected()),this,SLOT(slotDisconnected()));
    connect(tcpSocket,SIGNAL(readyRead()),this,SLOT(dataReceived()));
    connect(sendmesBtn,SIGNAL(clicked()),this,SLOT(slotSend()));
    connect(fileSender,SIGNAL(clicked()),this,SLOT(slotShowfilebox()));
    tcpSocket->connectToHost(*serverIp,port);
}

void mainchatroom::slotConnected()
{
    int length = 0;
    QString msg = "\x05\x02\x01" + thename;
    if((length=tcpSocket->write(msg.toLocal8Bit(),msg.toLocal8Bit().length()))!=msg.toLocal8Bit().length())
    {
        return;
    }
}

void mainchatroom::slotSend()
{
    if(messageEdit->toPlainText() == "")
    {
        QMessageBox::information(this,tr("error"),tr("不能发送空信息！"));
        return;
    }else if(messageEdit->toPlainText().size() >= 400)
    {
        QMessageBox::information(this,tr("error"),tr("输入字数不能超过400！"));
        return;
    }
    QDateTime current_date_time =QDateTime::currentDateTime();
    QString msg = current_date_time.toString("yyyy.MM.dd hh:mm:ss")+ "\n" + thename + ":" + messageEdit->toPlainText()+"\n";
    tcpSocket->write(msg.toLocal8Bit(),msg.toLocal8Bit().length());
    messageEdit->clear();
    return;
}

void mainchatroom::dataReceived()
{
    while (tcpSocket->bytesAvailable()>0) {
        QByteArray datagram;
        datagram.resize(tcpSocket->bytesAvailable());
        tcpSocket->read(datagram.data(),datagram.size());
        QString msg = QString::fromLocal8Bit(datagram.data());
        messageList->append(msg.left(datagram.size()));
    }
    return;
}

void mainchatroom::slotDisconnected()
{
    return;
}

void mainchatroom::slotShowfilebox()
{
    sfbox = new sendfilebox(thefileport,theip,thename);
    sfbox->show();
    return;
}
