#ifndef SERVERCONS_H
#define SERVERCONS_H
#include <QString>
#include "windows.h"
#include "db.h"

class serverCons
{
public:
    serverCons();
    ~serverCons();
    void run();

private:
        QString fileName;
        void writeData(QString filename);
        void readData(QString filename);
        QString config();
        DB dataBase;
        HANDLE hPipe;
        void getCount(HANDLE &hPipe);
        void getRecord(HANDLE &hPipe);
        void append(HANDLE &hPipe);
        void remove(HANDLE &hPipe);
        void update(HANDLE &hPipe);
        void save(HANDLE &hPipe);
        void getRecords(HANDLE &hPipe);
        ClassName fromClient(HANDLE &hPipe);
        void  toClient(HANDLE &hPipe,ClassName &record);
};

#endif // SERVERCONS_H
