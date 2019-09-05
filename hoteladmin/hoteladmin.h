#ifndef HOTELADMIN_H
#define HOTELADMIN_H

#include <QtWidgets/QMainWindow>
#include "ui_hoteladmin.h"
#include "classname.h"
#include "db.h"

class hoteladmin : public QMainWindow
{
	Q_OBJECT

public:
	hoteladmin(QWidget *parent = 0);
	~hoteladmin();
    int current;
    int count;
private:
	Ui::hoteladminClass ui;
	bool numberError;
    bool costError;
    ClassName arr[100];
    std::vector<structRecord> records;
    ClassName clear;
	 ClassName tmp;
	 ClassName forFind;
     QTimer *timer;
     QString filename;
     DB dataBase;
     void closeEvent(QCloseEvent *event);
private slots:
    

    

    void save();

    void destroy();

    void whoTakeCheck();

  
    void createTen();

   void hideShowRecord();
    //функция сохранения данных
    void saveAll();
    void newItem();
    //функция возвращения данных
    void returnAll();
     void   nextRecord(int index);
	 void checkRangeFreeRoom();
	 void sort();
	 QString spaceNumber(int i);
	 QString spaceFloor(int i);

     void loadData();
     ClassName getDataFromUi();

     /*void writeData(QString filename);
     void readData(QString filename);
     void saveTime();*/
	 
};

#endif // HOTELADMIN_H
