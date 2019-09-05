#include "classname.h"
#include "structrecord.h"

class DB
{
public:
    DB();
    ~DB();

    int getCount();
    ClassName getRecord(unsigned int id);
    int append(ClassName &record);
    int insert(ClassName &record);
    void remove(unsigned int id);
    int update(ClassName &record);
    void save(QString filename);
    void load(QString filename);
    std::vector <structRecord> getRecords();

private:
    std::vector <ClassName> arr;

    int id;
};
