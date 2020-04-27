#include "sendfilebox.h"

sendfilebox::sendfilebox(int fileport,QHostAddress *serverIp,QString username)
{
    #if defined(Q_OS_LINUX)
    setWindowState(Qt::WindowMaximized);
    #endif
    fileserverIp = serverIp;
    fileserverPort = fileport;
    userName = username;
    fileSocket = new QTcpSocket(this);
    //ui
    mainLayout = new QVBoxLayout(this);
    fileProBar = new QProgressBar;
    choosefileBtn = new QPushButton(tr("选择"));
    filepathLabel = new QLabel(tr("请选择你的文件"));
    sendfileBtn = new QPushButton(tr("发送！"));
    cancelfileBtn = new QPushButton(tr("取消发送"));
    sendfileBtn->setEnabled(false);
    cancelfileBtn->setEnabled(false);
    mainLayout->addWidget(fileProBar);
    mainLayout->addWidget(choosefileBtn);
    mainLayout->addWidget(filepathLabel);
    mainLayout->addWidget(sendfileBtn);
    mainLayout->addWidget(cancelfileBtn);
    connect(choosefileBtn,SIGNAL(clicked()),this,SLOT(chooseBtnOnclicked()));
    connect(sendfileBtn,SIGNAL(clicked()),this,SLOT(sendfileBtnOnclicked()));
    connect(fileSocket,SIGNAL(connected()),this,SLOT(startSendFile()));
    connect(fileSocket,SIGNAL(bytesWritten(qint64)),this,SLOT(sendFileData(qint64)));
    fileBytes = sentBytes = restBytes = 0;
    loadBytes = 4*1024;
}

void sendfilebox::chooseBtnOnclicked()
{
    fileName = QFileDialog::getOpenFileName(this);
    if(!fileName.isEmpty())
    {
        filepathLabel->setText(fileName);
        sendfileBtn->setEnabled(true);
    }else{
        filepathLabel->setText(tr("ERROR"));
    }
}

void sendfilebox::sendfileBtnOnclicked()
{
    fileSocket->connectToHost(*fileserverIp,fileserverPort);
    sendfileBtn->setEnabled(false);
    choosefileBtn->setEnabled(false);
    cancelfileBtn->setEnabled(true);
}

void sendfilebox::startSendFile()
{
    file = new QFile(fileName);
    if(!file->open(QFile::ReadOnly))
    {
        filepathLabel->setText("FILE OPEN FAILED!");
        return;
    }
    fileBytes = file->size();
    fileProBar->setValue(0);
    filepathLabel->setText(tr("传输中……"));
    QByteArray buf;
    QDataStream out(&buf,QIODevice::WriteOnly);
    out.setVersion(QDataStream::Qt_5_9);

    QString pureName = fileName.right(fileName.size()-fileName.lastIndexOf('/')-1);
    out<<qint64(0)<<qint64(0)<<pureName<<userName;
    fileBytes += buf.size();
    fileProBar->setMaximum(fileBytes);
    out.device()->seek(0);
    out << fileBytes << (qint64(buf.size()) - 2*sizeof (qint64));
    restBytes = fileBytes - fileSocket->write(buf);
}

void sendfilebox::sendFileData(qint64 sentSize)
{
    sentBytes += sentSize;
    fileProBar->setValue(sentBytes);
    if(restBytes>0)
    {
        QByteArray buf = file->read(qMin(loadBytes,restBytes));
        restBytes -= fileSocket->write(buf);
    }else{
        file->close();
    }
    if(sentBytes == fileBytes)
    {
        fileSocket->close();
        fileName.clear();
        QMessageBox::information(this,tr("success"),tr("成功传输！"));
        this->close();
    }
}
