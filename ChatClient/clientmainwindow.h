#ifndef CLIENTMAINWINDOW_H
#define CLIENTMAINWINDOW_H

#include <QDialog>
#include <QLabel>
#include <QLineEdit>
#include <QPushButton>
#include <QGridLayout>
#include <QHostAddress>
#include <QMessageBox>
#include <QHostInfo>
#include "mainchatroom.h"

class clientmainwindow : public QDialog
{
    Q_OBJECT

public:
    clientmainwindow(QWidget *parent = nullptr,Qt::WindowFlags f =0);
    ~clientmainwindow();
private:
    QLabel *titileLabel,*ipLabel,*portLabel,*fileportLabel,*usernameLabel;
    QLineEdit *ipEdit,*portEdit,*fileportEdit,*usernameEdit;
    QPushButton *enterBtn;
    QGridLayout *mainLayout;
    mainchatroom *mcroom;
    int port,fileport;
    QHostAddress *serverIp;
    QString userName;
public slots:
    void slotEnter();
};
#endif // CLIENTMAINWINDOW_H
