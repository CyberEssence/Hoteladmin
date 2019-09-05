#include "servercons.h"
#include <iostream>
#include <QStringList>
#include <windows.h>
#include "serverconst.h"

using namespace std;

serverCons::serverCons(){
    cout<< "Server start working" << endl;
    cout<< "Read data" << endl;
    fileName = config();

    readData(fileName);



}
serverCons::~serverCons(){

    cout<< "Server stopping ..."<<endl;

    writeData(fileName);

    DisconnectNamedPipe(hPipe);

    FlushFileBuffers(hPipe);

    CloseHandle(hPipe);

    cout<< "Server stoped"<<endl;
}


void serverCons::run(){
    bool fConnected;

    bool work= true;
    // Буфер для передачи данных через канал
    int  command = 0;

    // Количество байт данных, принятых через канал
    DWORD  cbRead;

    // Количество байт данных, переданных через канал
    DWORD  cbWritten;

    hPipe = CreateNamedPipe(PIPE_NAME,
        PIPE_ACCESS_DUPLEX,
        PIPE_TYPE_MESSAGE|PIPE_READMODE_MESSAGE|PIPE_WAIT,
        254,
        KBYTE,
        KBYTE,
        5000,
        NULL);
    if(hPipe == INVALID_HANDLE_VALUE){
        cout<< " CreateNamedPipe: ERROR " <<GetLastError();
        return;
    }

    cout<< "Waiting for connect..." << endl;
    fConnected = ConnectNamedPipe(hPipe,NULL);
    if(!fConnected){
        cout<< "ConnectNamedPipe: ERROR " <<GetLastError();
        return;
    }


    while(work){
        cout<< "Connected. Waiting for command..." << endl;
        ReadFile(hPipe, &command, 1,&cbRead, NULL);

        switch (command)
        {
            case -1:
                cout<< "COMMAND: Close server" << endl;
                work = false;
                break;
            case 0:
                cout<< "COMMAND: Get record" << endl;
                getRecord(hPipe);
                break;
            case 1:
                cout<< "COMMAND: append" << endl;
                append(hPipe);
                break;
            case 2:
                cout<< "COMMAND: remove " << endl;
                remove(hPipe);
                break;
            case 3:
                cout<< "COMMAND: get count" << endl;
                getCount(hPipe);
                break;
            case 4:
                cout<< "COMMAND: get records" << endl;
                getRecords(hPipe);
                break;
            default:
                break;
        }
    }

}

QString serverCons::config(){

    char buff[1000] ="0";
    QString configData;


    DWORD lpNumberOfBytesRead;//Кол-во прочитаных байтов, если 0 то уже конец файла

    DWORD dwCreate=OPEN_ALWAYS; //Флаг

    HANDLE config = CreateFileA("config.ini", GENERIC_READ , NULL, NULL, dwCreate , FILE_ATTRIBUTE_NORMAL, NULL);

    if (config == INVALID_HANDLE_VALUE) {   //если ошибка то выходим INVALID_HANDLE_VALUE

        MessageBox(NULL, TEXT("Error of server - file can't opened!"), TEXT("Warning"), MB_OK);
        CloseHandle(config);
    }
    do { //тут обьявляем цикл с пост условием потомучто мы не знаем какой длины файл и будем читать до конца, когда lpNumberOfBytesRead будет равна 0 тогда файл прочитан

        ReadFile( //функция чтения данных, параметры: дискрептор, массив куда читать, сколько прочитать байт, сколько прочитано,
            config,
            buff, 1, &lpNumberOfBytesRead, NULL);
        configData= QString::fromLocal8Bit(buff);
        if (lpNumberOfBytesRead == 0)  //Если ничего не рпочитано выходим
            break; //выход из цикла

    } while (lpNumberOfBytesRead != 0);
    CloseHandle(config); //закрыть дискрептор в данном случае для файла.

    return configData;

}

void serverCons::writeData(QString filename){

    dataBase.save(filename);

}

void serverCons::readData(QString filename)
{

    dataBase.load(filename);


}

void serverCons::getCount(HANDLE &hPipe){
    int count;
    DWORD  cbWritten;


    count = dataBase.getCount();
    WriteFile(hPipe,&count,sizeof(count),&cbWritten,NULL);

}

void serverCons::append(HANDLE &hPipe){
    ClassName tmp;
    DWORD  cbRead;
    DWORD  cbWritten;
    int pos;

    unsigned int id;

    tmp = fromClient(hPipe);
    pos = dataBase.append(tmp);
    WriteFile(hPipe,&pos,sizeof(pos),&cbWritten,NULL);

    id=tmp.id;
    WriteFile(hPipe,&id,sizeof(id),&cbWritten,NULL);


}

void serverCons::remove(HANDLE &hPipe){

    unsigned int id;
    DWORD  cbRead;
    DWORD  cbWritten;
    QStringList uniq;

    ReadFile(hPipe, &id, sizeof(id),&cbRead, NULL);
    dataBase.remove(id);
    //WriteFile(hPipe,&uniq,sizeof(uniq),&cbWritten,NULL);


}

void serverCons::getRecord(HANDLE &hPipe){

    unsigned int id;
    DWORD  cbRead;
    DWORD  cbWritten;
    ClassName tmp;

    ReadFile(hPipe, &id, sizeof(id),&cbRead, NULL);
    tmp = dataBase.getRecord(id);
    toClient(hPipe,tmp);
    //WriteFile(hPipe,&tmp,sizeof(tmp),&cbWritten,NULL);


}

void serverCons::getRecords(HANDLE &hPipe){

    DWORD  cbWritten;
    vector<structRecord> tmp = dataBase.getRecords();
    int size = dataBase.getCount();

    WriteFile(hPipe,&size,sizeof(size),&cbWritten,NULL);
    for(int i =0;i<size;i++){
        tmp[i].toClient(hPipe);
    }
    //WriteFile(hPipe,&tmp[i],tmp.size(),&cbWritten,NULL);


}

ClassName serverCons::fromClient(HANDLE &hPipe){
    DWORD  cbRead;
    char tmp[256] = "0";
    ClassName record;
    int size;
    bool boolArray [1];
    int intArray [5];
    //QDate dateArray [2];
    unsigned int id;


    ReadFile(hPipe,&id,sizeof(id),&cbRead,NULL);
    record.id = id;

    ReadFile(hPipe,&boolArray,sizeof(boolArray),&cbRead,NULL);
    record.ALL_seaview = boolArray[0];

    ReadFile(hPipe,&intArray,sizeof(intArray),&cbRead,NULL);
    record.ALL_number =  intArray[0];
    record.ALL_floor =  intArray[1];
    record.ALL_numberOfPlaces =  intArray[2];
    record.ALL_numberOfFreePlaces =  intArray[3];
    record.ALL_Cost =  intArray[4];

    /*ReadFile(hPipe,&dateArray,sizeof(dateArray),&cbRead,NULL);
    record.dateBirth =  dateArray[0];
    record.dateDeath =  dateArray[1];*/

    ReadFile(hPipe,&size,sizeof(size),&cbRead,NULL);
    ReadFile(hPipe,&tmp,size+1,&cbRead,NULL);
    record.ALL_whoIsThere = QString::fromLocal8Bit(tmp);

    ReadFile(hPipe,&size,sizeof(size),&cbRead,NULL);
    ReadFile(hPipe,&tmp,size+1,&cbRead,NULL);
    record.ALL_roomClass = QString::fromLocal8Bit(tmp);

    ReadFile(hPipe,&size,sizeof(size),&cbRead,NULL);
    ReadFile(hPipe,&tmp,size+1,&cbRead,NULL);
    record.ALL_note = QString::fromLocal8Bit(tmp);

    return record;
}

void serverCons::toClient(HANDLE &hPipe,ClassName &record){

    char tmp[256]="0";
    strcpy(tmp,record.ALL_roomClass.toLocal8Bit().data());
    int size=record.ALL_roomClass.toLocal8Bit().size();
    DWORD  cbWritten;
    bool boolArray [1] = {record.ALL_seaview};
    int intArray [5] = {record.ALL_number,record.ALL_floor,record.ALL_numberOfPlaces,record.ALL_numberOfFreePlaces, record.ALL_Cost};
    //QDate dateArray [2] = {record.dateBirth,record.dateDeath};
    unsigned int id = record.id;

    WriteFile(hPipe,&id,sizeof(id),&cbWritten,NULL);

    WriteFile(hPipe,&boolArray,sizeof(boolArray),&cbWritten,NULL);
    WriteFile(hPipe,&intArray,sizeof(intArray),&cbWritten,NULL);
    //WriteFile(hPipe,&dateArray,sizeof(dateArray),&cbWritten,NULL);

    WriteFile(hPipe,&size,sizeof(size),&cbWritten,NULL);
    WriteFile(hPipe,tmp,size+1,&cbWritten,NULL);

    strcpy(tmp,record.ALL_whoIsThere.toLocal8Bit().data());
    size=record.ALL_whoIsThere.toLocal8Bit().size();
    WriteFile(hPipe,&size,sizeof(size),&cbWritten,NULL);
    WriteFile(hPipe,tmp,size+1,&cbWritten,NULL);

     strcpy(tmp,record.ALL_whoIsThere.toLocal8Bit().data());
     size=record.ALL_whoIsThere.toLocal8Bit().size();
    WriteFile(hPipe,&size,sizeof(size),&cbWritten,NULL);
    WriteFile(hPipe,tmp,size+1,&cbWritten,NULL);
}

