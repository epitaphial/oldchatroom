#ifndef MAINCHATROOM_H
#define MAINCHATROOM_H

#include <QDialog>
#include <QHostAddress>
#include <QTcpSocket>
#include <QHostAddress>
#include <QListWidget>
#include <QGridLayout>
#include <QTextEdit>
#include <QPushButton>
#include <QHBoxLayout>
#include <QDateTime>
#include <QMessageBox>
#include <QTextBrowser>
#include "sendfilebox.h"

class mainchatroom : public QDialog
{
    Q_OBJECT
private:
    int theport,thefileport;
    QHostAddress *theip;
    QString thename;
    QTcpSocket *tcpSocket;
    //ui
    QGridLayout *mainLayout;
    QTextBrowser *messageList;
    QHBoxLayout *toolListLayout;
    QPushButton *imageSender;
    QPushButton *fileSender;
    QTextEdit *messageEdit;
    QPushButton *sendmesBtn;
    //filebox
    sendfilebox *sfbox;
public:
    mainchatroom(int port,int fileport,QHostAddress *serverIp,QString userName);
public slots:
    void slotConnected();
    void slotDisconnected();
    void dataReceived();
    void slotSend();
    void slotShowfilebox();
};

#endif // MAINCHATROOM_H
