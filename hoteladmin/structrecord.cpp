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
