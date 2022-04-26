#include "sever.h"
#include "ui_sever.h"
#include "client.h"
#include "ui_client.h"
#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    sever w;
    client g;
    w.setWindowTitle("Server");
    g.setWindowTitle("Client");
    g.show();
    w.show();
    return a.exec();
}
