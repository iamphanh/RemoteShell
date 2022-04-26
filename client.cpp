#include "client.h"
#include "ui_client.h"

static const int DEFAULT_PORT = 2912;

client::client(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::client)
{
    ui->setupUi(this);
    ui->user->setText("LocalHost");
    ui->portNumber->setNum(DEFAULT_PORT);

    socket = new QTcpSocket(this);
    buffer = new QBuffer(this);
    buffer->open(QIODevice::ReadWrite);

    connect(socket,SIGNAL(connected()),this,SLOT(setConnect()));
    connect(socket,SIGNAL(disconnected()),this,SLOT(setDisconnect()));
    connect(ui->input,SIGNAL(returnPressed()),this,SLOT(send()));
    connect(ui->send,SIGNAL(clicked()),this,SLOT(send()));
    setDisconnect();

}

client::~client()
{
    delete ui;
}

void client::on_disconnect_clicked()
{
    if(socket->state() == QAbstractSocket::ConnectedState){
        socket->disconnectFromHost();
        setDisconnect();
}
    else{
        QMessageBox msgBox;
        msgBox.setText("Not connect !!!");
        msgBox.exec();
    }

}

void client::send()
{

        if(socket->error() == QAbstractSocket::RemoteHostClosedError){
           QMessageBox msgBox;
           msgBox.setWindowTitle("Erorr");
           msgBox.setText("Not connect !!!");
           msgBox.exec();
       }
        else {
            QString shell = ui->input->text();
            socket->write(shell.toLatin1());
            ui->input->clear();
        }


}

void client::on_connect_clicked()
{

    if(socket->state() == QAbstractSocket::UnconnectedState){

        socket->connectToHost(ui->user->text(),ui->portNumber->text().toInt());
        if(socket->waitForConnected(3000)){
            setConnect();
        } else{
            QMessageBox msgBox;
            msgBox.setWindowTitle("Erorr!!!");
            msgBox.setText("Connection failed !!!");
            msgBox.exec();
        }
     }
}

void client::setDisconnect()
{

    ui->disconnect->setEnabled(false);
    ui->connect->setEnabled(true);
    ui->send->setEnabled(false);
    ui->input->clear();

}

void client::setConnect()
{
    ui->disconnect->setEnabled(true);
    ui->connect->setEnabled(false);
    ui->send->setEnabled(true);
}


