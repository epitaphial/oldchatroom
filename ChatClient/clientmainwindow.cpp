#include "clientmainwindow.h"



clientmainwindow::clientmainwindow(QWidget *parent,Qt::WindowFlags f)
    : QDialog(parent,f)
{
    setWindowTitle(tr("nmsl chat"));
    #if defined(Q_OS_LINUX)
    setWindowState(Qt::WindowMaximized);
    #endif
    titileLabel = new QLabel(tr("祖安聊天室V1.0"));
    ipLabel = new QLabel(tr("ip地址："));
    portLabel = new QLabel(tr("端口："));
    fileportLabel = new QLabel(tr("文件端口："));
    usernameLabel = new QLabel(tr("昵称："));
    ipEdit = new QLineEdit;
    portEdit = new QLineEdit;
    fileportEdit = new QLineEdit;
    usernameEdit = new QLineEdit;
    enterBtn = new QPushButton(tr("开始网上冲浪！"));
    mainLayout = new QGridLayout(this);
    mainLayout->addWidget(titileLabel,0,0);
    mainLayout->addWidget(ipLabel,1,0);
    mainLayout->addWidget(ipEdit,1,1);
    mainLayout->addWidget(portLabel,2,0);
    mainLayout->addWidget(portEdit,2,1);
    mainLayout->addWidget(fileportLabel,3,0);
    mainLayout->addWidget(fileportEdit,3,1);
    mainLayout->addWidget(usernameLabel,4,0);
    mainLayout->addWidget(usernameEdit,4,1);
    mainLayout->addWidget(enterBtn,5,0,1,2);
    port = 55555;
    portEdit->setText(QString::number(port));
    QRegExp regx("[0-9]+$");
    QValidator *validator = new QRegExpValidator(regx,this);
    portEdit->setValidator(validator);
    fileport = 55556;
    fileportEdit->setText(QString::number(fileport));
    fileportEdit->setValidator(validator);
    ipEdit->setText("127.0.0.1");
    serverIp = new QHostAddress();
    connect(enterBtn,SIGNAL(clicked()),this,SLOT(slotEnter()));
}

clientmainwindow::~clientmainwindow()
{
}

void clientmainwindow::slotEnter()
{
    QString ip = ipEdit->text();
    if(!serverIp->setAddress(ip))
    {
        QMessageBox::information(this,tr("error"),tr("服务器ip地址错误！"));
        return;
    }
    if(usernameEdit->text() == "")
    {
        QMessageBox::information(this,tr("error"),tr("用户名不能为空！"));
        return;
    }
    userName = usernameEdit->text();
    port = portEdit->text().toInt();
    fileport = fileportEdit->text().toInt();
    mcroom = new mainchatroom(port,fileport,serverIp,userName);
    mcroom->show();
    this->hide();
}

