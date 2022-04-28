#ifndef CLIENT_H
#define CLIENT_H
#include <QDebug>
#include <QMainWindow>
#include<QBuffer>
#include<QTcpServer>
#include<QTcpSocket>
#include<QMessageBox>
#include<QString>

namespace Ui {
class client;
}

class client : public QMainWindow
{
    Q_OBJECT

public:
    explicit client(QWidget *parent = nullptr);
    ~client();

private slots:
    void on_disconnect_clicked();
    void send();
    void on_connect_clicked();
    void setDisconnect();
    void setConnect();

private:
    Ui::client *ui;
    QBuffer *buffer;
    QTcpSocket *socket;

};

#endif // CLIENT_H
