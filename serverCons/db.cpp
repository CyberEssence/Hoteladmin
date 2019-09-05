#include "db.h"
#include <structrecord.h>
#include <QStringList>
#include "windows.h"
#include <QtWidgets/QMessageBox.h>
#include "classname.h"

DB::DB()
{
    id = 0;
}

DB::~DB()
{
}

int DB::getCount(){

    return arr.size();
}

ClassName DB::getRecord(unsigned int id){

    for (auto i = arr.begin();i!=arr.end();++i)
        if(i->id == id)
            return *i;

}

std::vector <structRecord> DB::getRecords(){

    std::vector <structRecord> records;

    for(ClassName elem : arr)
        records.push_back(structRecord (elem));
    return records;
}

int DB::append(ClassName &record){

    record.id = id++;
    return insert(record);
}

int DB::insert(ClassName &record){
    if( getCount()==0){
        arr.push_back(record);
        return 0;
    }
    auto i = arr.begin();
    int pos = 0;


    for(;i!=arr.end() && *i<record;++i){
        pos++;
    }
    if(pos == 0)
        arr.push_back(record);
    else
        arr.insert(i,record);
    return pos;
}

void DB::remove(unsigned int id){

    for(auto i = arr.begin(); i!= arr.end();++i)
        if(i->id == id){
            arr.erase(i);
            break;
        }
}

int DB::update(ClassName &record){

    int newPos = 0;

    remove(record.id);
    newPos = insert(record);

    return newPos;
}

void DB::save(QString filename){

    QTextCodec* c = QTextCodec::codecForLocale();
    DWORD bytesRead;
    QByteArray bpath = filename.toLocal8Bit();
    char *path = bpath.data();
    HANDLE hFile = CreateFileA(path, GENERIC_WRITE , NULL, NULL, CREATE_ALWAYS , FILE_ATTRIBUTE_NORMAL, NULL);
    if (INVALID_HANDLE_VALUE == hFile) {
        return;
    }
    for(int i = 0; i< getCount();i++){
            QString data;
            data += arr[i].ALL_roomClass + ";" + arr[i].ALL_whoIsThere + ";" +  QString::number(arr[i].ALL_number) + ";"
                    + QString::number(arr[i].ALL_floor) + ";" + QString::number(arr[i].ALL_numberOfPlaces) + ";" + QString::number(arr[i].ALL_numberOfFreePlaces) + ";"
                    + QString::number(arr[i].ALL_Cost) + ";" + arr[i].ALL_note + QString::number(arr[i].ALL_seaview) +'\r \n';
         QByteArray ba = data.toLocal8Bit();

    char *strm2 = ba.data();
    WriteFile(hFile,  strm2, strlen(strm2), &bytesRead, NULL);
    WriteFile(hFile,  "\r\n", 2, NULL, NULL);
    }

    CloseHandle(hFile);

}

void DB::load(QString filename)
{

    char buff[100]="0";
    QByteArray bpath = filename.toLocal8Bit();
    char *path = bpath.data();
    QString result;
    int numBuf;

    HANDLE hFile;//это дискрептор открытого файла
    DWORD lpNumberOfBytesRead;//Кол-во прочитаных байтов, если 0 то уже конец файла
    hFile = CreateFileA(  //функция создания ANSI!!!последния буква А
        path, //это имя файла \\условно считаеться как \ но писать 2 иначе vs подумает что служ. команда
        GENERIC_READ,  //в режим чтения GENERIC_WRITE то в запись
        0, NULL,
        OPEN_ALWAYS, //говорим что открываем файл который существует или создаем его
        FILE_ATTRIBUTE_NORMAL, //Это обычный файл с атриьутом не архивный и т.п.
        NULL); //это точно не помню с защитой

    do //тут обьявляем цикл с пост условием потому что мы не знаем какой длины файл и будем читать до конца, когда lpNumberOfBytesRead будет равна 0 тогда файл прочитан
    {
        //ReadFile(hFile,&numBuf,sizeof(numBuf),&lpNumberOfBytesRead, NULL);
        ReadFile(hFile,&buff,1,&lpNumberOfBytesRead, NULL);
        if (lpNumberOfBytesRead == 0)  //Если ничего не рпочитано выходим
            break; //выход из цикла
        result += QString::fromLocal8Bit(buff);


    } while (lpNumberOfBytesRead != 0);
    CloseHandle(hFile); //закрыть дискрептор в данном случае для файла.

    QStringList list = result.split('\n');
    list.removeLast();




    for (int i = 0; i < list.size(); i++)
    {
        QString dataList = list[i];
        QStringList newData = dataList.split(';');
        //QString FIO = newData[0]+newDate[1]+newDate[2];

        ClassName tmpDB(newData[0], newData[1], newData[2].toInt(), newData[3].toInt(), newData[4].toInt(), newData[5].toInt(),  newData[6].toInt(), newData[7], newData[8].toInt());

        append(tmpDB);


    }


}
