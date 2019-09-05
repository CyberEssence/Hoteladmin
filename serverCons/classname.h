#ifndef CLASSNAME_H
#define CLASSNAME_H
#include <QString>
#include <QTextCodec>

class ClassName
{

public:
    QString ALL_roomClass;
    QString ALL_whoIsThere;
    int ALL_number;
    int ALL_floor;
    int ALL_numberOfPlaces;
    int ALL_numberOfFreePlaces;
    int ALL_Cost;
    QString ALL_note;
    bool ALL_seaview;
    unsigned int id;

    ClassName()
    {
        ALL_whoIsThere="";
        QTextCodec* c = QTextCodec::codecForLocale();
        ALL_roomClass=c->toUnicode("Класс I");
        ALL_number=1;
        ALL_floor=1;
        ALL_numberOfPlaces=1;
       ALL_numberOfFreePlaces=1;
        ALL_Cost=100;
        ALL_seaview=false;
        ALL_note="";
    }

    ClassName(QString ALL_roomClass, QString ALL_whoIsThere, int ALL_number, int ALL_floor, int ALL_numberOfPlaces, int ALL_numberOfFreePlaces, int ALL_Cost, QString ALL_note, bool ALL_seaview)
    {
        ClassName::ALL_roomClass=ALL_roomClass;
        ClassName::ALL_whoIsThere= ALL_whoIsThere;
        ClassName::ALL_number=ALL_number;
        ClassName::ALL_floor= ALL_floor;
        ClassName::ALL_numberOfPlaces=ALL_numberOfPlaces;
        ClassName::ALL_numberOfFreePlaces=ALL_numberOfFreePlaces;
        ClassName::ALL_Cost=ALL_Cost;
        ClassName::ALL_note=ALL_note;
        ClassName::ALL_seaview=ALL_seaview;

    }

   bool operator>(const ClassName& Right) const
   {
       if(ALL_roomClass.length()>Right.ALL_roomClass.length())
           return true;
       else
           if (ALL_roomClass.length()==Right.ALL_roomClass.length() && ALL_floor>Right.ALL_floor)

               return true;
           else
               if (ALL_roomClass.length()==Right.ALL_roomClass.length() && ALL_floor==Right.ALL_floor &&ALL_number>Right.ALL_number)
                   return true;
               else
                   return false;
   }

   bool operator<(const ClassName & Right)const
       {
       if(ALL_roomClass.length()<Right.ALL_roomClass.length())
           return true;
       else
           if (ALL_roomClass.length()==Right.ALL_roomClass.length() && ALL_floor<Right.ALL_floor)

               return true;
           else
               if (ALL_roomClass.length()==Right.ALL_roomClass.length() && ALL_floor==Right.ALL_floor &&ALL_number<Right.ALL_number)
                   return true;
               else
                   return false;
       }

   bool operator==(const ClassName& Right) const
   {
       if(ALL_whoIsThere==Right.ALL_whoIsThere &&
       ALL_roomClass==Right.ALL_roomClass &&
       ALL_number==Right.ALL_number &&
     ALL_floor==Right.ALL_floor &&
       ALL_numberOfPlaces==Right.ALL_numberOfPlaces &&
       ALL_numberOfFreePlaces==Right.ALL_numberOfFreePlaces &&
       ALL_Cost==Right.ALL_Cost &&
       ALL_seaview==Right.ALL_seaview &&
       ALL_note==Right.ALL_note)
           return true;
       else
            return false;
   }

   ClassName& operator=(const ClassName& Right)
   {
       this->ALL_whoIsThere=Right.ALL_whoIsThere;
       this->ALL_roomClass=Right.ALL_roomClass;
       this->ALL_number=Right.ALL_number;
       this->ALL_floor=Right.ALL_floor;
       this->ALL_numberOfPlaces=Right.ALL_numberOfPlaces;
       this->ALL_numberOfFreePlaces=Right.ALL_numberOfFreePlaces;
       this->ALL_Cost=Right.ALL_Cost;
       this->ALL_seaview=Right.ALL_seaview;
       this->ALL_note=Right.ALL_note;
       return *this;
   }

};
#endif // CLASSNAME_H
