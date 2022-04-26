#ifndef SEVER_H
#define SEVER_H

#include <QMainWindow>
#include<QTcpServer>
#include<QTcpSocket>
#include<QMap>
#include<QBuffer>
#include<QList>
#include<QMessageBox>
#include<QProcess>
#include<QString>
namespace Ui {
class sever;
}

class sever : public QMainWindow
{
    Q_OBJECT

public:
    explicit sever(QWidget *parent = nullptr);
    ~sever();

private slots:
    void on_start_clicked();
    void addConnect();
    void removeConnect();
    void outputData();
    void rightMessage();
    void wrongMessage();
    void disConnect();

    void on_stop_clicked();

private:
    QTcpServer *server;
    QList<QTcpSocket*> connections;
    QHash<QTcpSocket*,QBuffer*> data;
    QProcess *myProcess;
    Ui::sever *ui;
};

#endif // SEVER_H
