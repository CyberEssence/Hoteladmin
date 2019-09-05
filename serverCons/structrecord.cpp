#include "structrecord.h"
#include "classname.h"

structRecord::structRecord()
{
    QTextCodec* c = QTextCodec::codecForLocale();
    id = 0;
    ALL_roomClass=c->toUnicode("Класс I");
    ALL_number=1;
    ALL_floor=1;
    //ALL_numberOfPlaces=1;
    ALL_numberOfFreePlaces=1;
    ALL_Cost=100;
    //ALL_seaview=false;
    ALL_note="";
}

structRecord::structRecord(ClassName & record){
    ALL_roomClass = record.ALL_roomClass;
    ALL_number = record.ALL_number;
    ALL_floor = record.ALL_floor;
    ALL_numberOfFreePlaces = record.ALL_numberOfFreePlaces;
    ALL_Cost = record.ALL_Cost;
    ALL_note = record.ALL_note;
    id = record.id;
}

void structRecord::toClient(HANDLE &hPipe){
    char tmp[256]="0";
    strcpy(tmp,ALL_roomClass.toLocal8Bit().data());
    int size=ALL_roomClass.toLocal8Bit().size();
    DWORD  cbWritten;
    int id = this->id;

    WriteFile(hPipe,&id,sizeof(id),&cbWritten,NULL);

    WriteFile(hPipe,&size,sizeof(size),&cbWritten,NULL);
    WriteFile(hPipe,tmp,size+1,&cbWritten,NULL);

    strcpy(tmp,ALL_whoIsThere.toLocal8Bit().data());
    size=ALL_whoIsThere.toLocal8Bit().size();
    WriteFile(hPipe,&size,sizeof(size),&cbWritten,NULL);
    WriteFile(hPipe,tmp,size+1,&cbWritten,NULL);

     strcpy(tmp,ALL_note.toLocal8Bit().data());
     size=ALL_note.toLocal8Bit().size();
    WriteFile(hPipe,&size,sizeof(size),&cbWritten,NULL);
    WriteFile(hPipe,tmp,size+1,&cbWritten,NULL);
}

void structRecord::fromClient(HANDLE &hPipe){

    DWORD  cbRead;
    char tmp[256] = "0";
    structRecord record;
    int size;
    unsigned int id;

    ReadFile(hPipe,&id,sizeof(id),&cbRead,NULL);
    this->id = id;


    ReadFile(hPipe,&size,sizeof(size),&cbRead,NULL);
    ReadFile(hPipe,&tmp,size+1,&cbRead,NULL);
    ALL_roomClass = QString::fromLocal8Bit(tmp);

    ReadFile(hPipe,&size,sizeof(size),&cbRead,NULL);
    ReadFile(hPipe,&tmp,size+1,&cbRead,NULL);
    ALL_whoIsThere = QString::fromLocal8Bit(tmp);

    ReadFile(hPipe,&size,sizeof(size),&cbRead,NULL);
    ReadFile(hPipe,&tmp,size+1,&cbRead,NULL);
    ALL_note = QString::fromLocal8Bit(tmp);


}
