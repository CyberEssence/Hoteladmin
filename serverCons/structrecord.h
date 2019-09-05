#ifndef STRUCTRECORD_H
#define STRUCTRECORD_H
#include "classname.h"
#include "windows.h"

struct structRecord
{
    structRecord();

    structRecord(ClassName & record);

    void fromClient(HANDLE &hPipe);
    void toClient(HANDLE &hPipe);

    unsigned int id;
    QString ALL_roomClass;
    QString ALL_whoIsThere;
    int ALL_number;
    int ALL_floor;
    //int ALL_numberOfPlaces;
    int ALL_numberOfFreePlaces;
    int ALL_Cost;
    QString ALL_note;
    //bool ALL_seaview;
};

#endif // STRUCTRECORD_H
