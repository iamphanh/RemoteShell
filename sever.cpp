#include "sever.h"
#include "ui_sever.h"

static const int DEFAULT_PORT = 2912;
QHostAddress address;
int port;
sever::sever(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::sever)
{
    ui->setupUi(this);
    ui->output->setStyleSheet("background-color: rgba(0, 0, 0, 1)");
    server = new QTcpServer();
    connect(server,SIGNAL(newConnection()),this,SLOT(addConnect()));
    //connect(server,SIGNAL(disconnect()),this,SLOT(disConnect()));
    port = DEFAULT_PORT;
    address = QHostAddress::LocalHost;
    ui->portNumber->setText(QString::number(port));
    ui->ipAddress->setText(address.toString());
    ui->ipAddress->isEnabled();

}

sever::~sever()
{
    delete ui;
}

void sever::disConnect()
{
    if(server->isListening()){
        server->close();
        ui->start->setEnabled(true);
        ui->stop->setEnabled(false);
    }
    else {
           qDebug()<< "Disconnect faile!!";
        }
}

void sever::on_start_clicked()
{
  port = ui->portNumber->text().toInt();
  bool bit = server->listen(address,port);
  if(bit){
      ui->start->setEnabled(false);
      ui->stop->setEnabled(true);
  }
  else {
      qDebug()<<"Sever can't srart. Check IP or port !!!";
  }
}

void sever::addConnect()
{
    QTcpSocket* connection = server->nextPendingConnection();
    QBuffer *buffer = new QBuffer(this);
    buffer->open(QIODevice::ReadWrite);
    data.insert(connection,buffer);
    connections.append(connection);
    connect(connection,SIGNAL(disconnected()),this,SLOT(removeConnect()));
    connect(connection,SIGNAL(readyRead()),this,SLOT(outputData()));

}

void sever::removeConnect()
{
   QTcpSocket* socket = static_cast<QTcpSocket*>(sender());
   QBuffer *buffer = data.take(socket);
   buffer->close();
   buffer->deleteLater();
   connections.removeAll(socket);
   socket->deleteLater();
}

void sever::outputData()
{
    QTcpSocket* socket = static_cast<QTcpSocket*>(sender());
    QBuffer *buffer = data.value(socket);
    qint64 bytes = buffer->write(socket->readAll());
    buffer->seek(buffer->pos() - bytes);

    QString shell = buffer->readLine();
    QStringList commandLine = QStringList() << "/c" << shell;
    myProcess = new QProcess();
    myProcess->start("C:\\Windows\\System32\\cmd.exe",commandLine);
    myProcess->waitForStarted();

    connect(myProcess,SIGNAL(readyReadStandardOutput()),this,SLOT(rightMessage()));
    connect(myProcess,SIGNAL(readyReadStandardError()),this,SLOT(wrongMessage()));

}

void sever::rightMessage()
{
    QString message = myProcess->readAllStandardOutput();
    ui->output->setStyleSheet("background-color: rgba(0, 0, 0, 1); color:White");
    ui->output->setText(message);
}


void sever::wrongMessage()
{
    QString message = myProcess->readAllStandardError();
    ui->output->setStyleSheet("background-color: rgba(0, 0, 0, 1); color:red");
    ui->output->setText(message);
}

void sever::on_stop_clicked()
{
    if(server->isListening()){
        server->close();
        for(QTcpSocket *socket : connections){
            socket->disconnectFromHost();
            socket->deleteLater();
        }


        ui->start->setEnabled(true);
        ui->stop->setEnabled(false);

    } else {
        qDebug() << "No connected!!!";
}
}

