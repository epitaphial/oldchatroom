#ifndef SENDFILEBOX_H
#define SENDFILEBOX_H

#include <QDialog>
#include <QHostAddress>
#include <QTcpSocket>
#include <QLabel>
#include <QVBoxLayout>
#include <QPushButton>
#include <QFileDialog>
#include <QProgressBar>
#include <QMessageBox>

class sendfilebox : public QDialog
{
    Q_OBJECT
private:
    QHostAddress *fileserverIp;
    QString userName;
    int fileserverPort;
    QTcpSocket *fileSocket;
    QFile *file;
    QString fileName;
    qint64 fileBytes,sentBytes,restBytes,loadBytes;
    //UI
    QProgressBar *fileProBar;
    QPushButton *choosefileBtn;
    QLabel *filepathLabel;
    QPushButton *sendfileBtn;
    QPushButton *cancelfileBtn;
    QVBoxLayout *mainLayout;

private slots:
    void chooseBtnOnclicked();
    void sendfileBtnOnclicked();
    void startSendFile();
    void sendFileData(qint64);
public:
    sendfilebox(int fileport,QHostAddress *serverIp,QString username);
};

#endif // SENDFILEBOX_H
