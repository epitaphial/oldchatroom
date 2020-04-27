#ifndef CHATWINDOW_H
#define CHATWINDOW_H

#include <QDialog>
#include <QListWidget>
#include <QLabel>
#include <QLineEdit>
#include <QPushButton>
#include <QGridLayout>
#include <QDateTime>
#include <QBuffer>
#include <QImage>
#include <QDataStream>
#include <QByteArray>
#include "server.h"
#include "fileserver.h"

class ChatWindow : public QDialog
{
    Q_OBJECT

public:
    ChatWindow(QWidget *parent = nullptr,Qt::WindowFlags f = 0);
    ~ChatWindow();

private:
    QLabel *titleLabel;
    QLabel *memberNumLabel;
    QListWidget *memberList;
    QLabel *portLabel;
    QLineEdit *portLineEdit;
    QLabel *fileportLabel;
    QLineEdit *fileportLineEdit;
    QPushButton *createChatBtn;
    QGridLayout *mainLayout;
    int port;
    int fileport;
    server *chatserver;
    fileserver *chatfileserver;
public slots:
    void slotCreateServer();
    void updateMember(QString);
    void deleteMember(QString);
    void slotUploadInfo(QString,QString,bool);
};
#endif // CHATWINDOW_H
