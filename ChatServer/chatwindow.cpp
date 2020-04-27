#include "chatwindow.h"

ChatWindow::ChatWindow(QWidget *parent,Qt::WindowFlags f)
    : QDialog(parent,f)
{
    setWindowTitle(tr("nmsl chat"));
    titleLabel = new QLabel(tr("欢迎使用儒雅随和聊天软件！"));
    memberNumLabel = new QLabel(tr("当前在线成员："));
    memberList = new QListWidget;
    portLabel = new QLabel(tr("端口"));
    portLineEdit = new QLineEdit;
    fileportLabel = new QLabel(tr("文件端口"));
    fileportLineEdit = new QLineEdit;
    createChatBtn = new QPushButton(tr("创建聊天室"));
    mainLayout = new QGridLayout(this);
    mainLayout->addWidget(titleLabel,0,0,1,2);
    mainLayout->addWidget(memberNumLabel,1,0,1,2);
    mainLayout->addWidget(memberList,2,0,1,2);
    mainLayout->addWidget(portLabel,3,0);
    mainLayout->addWidget(portLineEdit,3,1);
    mainLayout->addWidget(fileportLabel,4,0);
    mainLayout->addWidget(fileportLineEdit,4,1);
    mainLayout->addWidget(createChatBtn,5,0,1,2);
    port = 55555;
    portLineEdit->setText(QString::number(port));
    fileport = 55556;
    fileportLineEdit->setText(QString::number(fileport));
    QRegExp regx("[0-9]+$");
    QValidator *validator = new QRegExpValidator(regx,this);
    portLineEdit->setValidator(validator);
    fileportLineEdit->setValidator(validator);
    connect(createChatBtn,SIGNAL(clicked()),this,SLOT(slotCreateServer()));
}

ChatWindow::~ChatWindow()
{
}

void ChatWindow::slotCreateServer()
{
    port = portLineEdit->text().toInt();
    chatserver = new server(this,port);
    fileport = fileportLineEdit->text().toInt();
    chatfileserver = new fileserver(this,fileport);
    connect(chatserver,SIGNAL(sigUpdateMember(QString)),this,SLOT(updateMember(QString)));
    connect(chatserver,SIGNAL(sigDeleteMember(QString)),this,SLOT(deleteMember(QString)));
    connect(chatfileserver,SIGNAL(slotUploadInfo(QString,QString,bool)),this,SLOT(slotUploadInfo(QString,QString,bool)));
    createChatBtn->setEnabled(false);
}

void ChatWindow::updateMember(QString userName)
{
    memberList->addItem(userName);
}

void ChatWindow::deleteMember(QString userName)
{
    for (int i = 0;i<memberList->count();i++) {
        if(userName == memberList->item(i)->text())
        {
            memberList->takeItem(i);
        }
    }
}

void ChatWindow::slotUploadInfo(QString fileName,QString userName,bool ifpic)
{
    if(ifpic)
    {
        QDateTime current_date_time =QDateTime::currentDateTime();
        QString info = current_date_time.toString("yyyy.MM.dd hh:mm:ss")+ "\n" +  userName + " 发送了图片:" + fileName + "\n";
        this->chatserver->updateClients(info.toLocal8Bit(),info.toLocal8Bit().length());
    }else{
        QDateTime current_date_time =QDateTime::currentDateTime();
        QString info = current_date_time.toString("yyyy.MM.dd hh:mm:ss")+ "\n" +  userName + " 发送了文件:" + fileName + "\n";
        this->chatserver->updateClients(info.toLocal8Bit(),info.toLocal8Bit().length());
    }

}
