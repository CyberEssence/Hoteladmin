#include <QCoreApplication>
#include <servercons.h>



int main(int argc, char *argv[])
{
    //QCoreApplication a(argc, argv);

    serverCons server;
    server.run();
    //return a.exec();
}
