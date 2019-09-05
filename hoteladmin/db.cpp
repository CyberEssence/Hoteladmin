#include "db.h"
#include <structrecord.h>
#include <QStringList>
#include <windows.h>
#include <QMessageBox>
#include "classname.h"

DB::DB()
{

}

DB::~DB()
{

}

bool DB::connect(){
    bool connect;

    hPipe = CreateFile( PIPE_NAME, GENERIC_READ | GENERIC_WRITE, 0, NULL, OPEN_EXISTING, 0, NULL);

    if(hPipe == INVALID_HANDLE_VALUE){
        connect = WaitNamedPipe(PIPE_NAME,5000);
        while(connect){
            connect = WaitNamedPipe(PIPE_NAME,5000);
        }
        hPipe = CreateFile( PIPE_NAME, GENERIC_READ | GENERIC_WRITE, 0, NULL, OPEN_EXISTING, 0, NULL);
    }

    return true;
}

int DB::getCount(){

    return countRecord.size();
}

ClassName DB::getRecord(unsigned int id){

    int command = REQ_GET_RECORD;
        DWORD  cbWritten;
        DWORD  cbRead;
        ClassName tmp;

        WriteFile(hPipe,&command,1,&cbWritten,NULL);
        WriteFile(hPipe,&id,sizeof(id),&cbWritten,NULL);

        tmp = fromServer();

        return tmp;
}

std::vector<structRecord> DB::getRecords(){

    std::vector <structRecord> records;
    structRecord tmp;
    int command = REQ_GET_RECORDS;
    DWORD  cbWritten;
    DWORD  cbRead;
    int size;

    WriteFile(hPipe,&command,1,&cbWritten,NULL);

    ReadFile(hPipe, &size, sizeof(size),&cbRead, NULL);

    for (int i = 0; i < size; i++)
    {
        ReadFile(hPipe, &tmp, records.size(),&cbRead, NULL);
        records.push_back(tmp);
    }
    return records;
}

int DB::append(ClassName &record){
    int command = REQ_APPEND;
    DWORD  cbWritten;
    DWORD  cbRead;
    int pos ;

    WriteFile(hPipe,&command,1,&cbWritten,NULL);

    WriteFile(hPipe,&record,sizeof(record),&cbWritten,NULL);

    ReadFile(hPipe, &pos, sizeof(pos),&cbRead, NULL);

    return pos;
}

int DB::insert(ClassName &record){

    auto i = countRecord.begin();
    int pos = 0;

    for(;i!=countRecord.end() && *i<record;++i){
        pos++;
    }
    if(pos == 0)
        countRecord.push_back(record);
    else
        countRecord.insert(i,record);
    return pos;
}

void DB::remove(unsigned int id){

    int command = REQ_REMOVE;
    DWORD  cbWritten;

    WriteFile(hPipe,&command,sizeof(command),&cbWritten,NULL);

    WriteFile(hPipe,&id,sizeof(id),&cbWritten,NULL);

}


void DB::close(){

    int command = REQ_CLOSE_SERVER;
    DWORD  cbWritten;


    WriteFile(hPipe,&command,1,&cbWritten,NULL);

}

unsigned int DB::getId(){
    unsigned int id;
    DWORD  cbRead;

    ReadFile(hPipe, &id, sizeof(id),&cbRead, NULL);

    return id;
}

ClassName DB::fromServer(){

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

void DB::toServer(ClassName &record){

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
