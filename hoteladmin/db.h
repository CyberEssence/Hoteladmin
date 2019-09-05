#ifndef DB_H
#define DB_H
#include <classname.h>
#include <QStringList>
#include <windows.h>
#include <serverconst.h>
#include <QMessageBox>
#include <structRecord.h>
#include "classname.h"

class DB
{
public:
    DB();
    ~DB();

    bool connect();
    int getCount();
    ClassName getRecord(unsigned int id);
    int append(ClassName &record);
    int insert(ClassName &record);
    bool uniqFIO(QString FIO);
    QStringList parentsForCombobox(unsigned int id);
    void remove(unsigned int id);
    int update(ClassName &record);
    void close();
    std::vector <structRecord> getRecords();
    void toServer(ClassName &record);
    ClassName fromServer();
    unsigned int getId();

private:
    std::vector <ClassName> countRecord;
    HANDLE hPipe;
};
#endif // HOTELADMIN_H
