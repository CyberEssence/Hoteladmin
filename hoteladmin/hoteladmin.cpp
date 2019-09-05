#include "hoteladmin.h"
#include <string>
#include <QTimer>
#include "windows.h"
#include <QTextCodec>
#include "db.h"
#include <QMainWindow>

hoteladmin::hoteladmin(QWidget *parent)
	: QMainWindow(parent)
{
	ui.setupUi(this);
	current =0;
        count=0;
    ui.room->setRange(1,1000);
	ui.floor->setRange(1,24);
	hideShowRecord();
	
	ui.numberRoom->setRange(1,8);
	ui.cost->setRange(100,10000);
	
	connect(ui.btnCreate, SIGNAL(clicked()), this, SLOT(newItem()));
    connect(ui.btnSave, SIGNAL(clicked()), this, SLOT(save()));
    connect(ui.browser, SIGNAL(currentRowChanged(int)), this, SLOT(nextRecord(int)));
	connect(ui.btnDelete, SIGNAL(clicked()), this, SLOT(destroy()));
	connect(ui.numberRoom, SIGNAL(valueChanged(int)), this, SLOT(checkRangeFreeRoom()));
	connect(ui.numberFreeRoom, SIGNAL(valueChanged(int)), this, SLOT(whoTakeCheck()));
    connect(ui.numberRoom, SIGNAL(valueChanged(int)), this, SLOT(whoTakeCheck()));
    connect(ui.btnFill, SIGNAL(clicked()), this, SLOT(createTen()));

    //filename = config();
    //readData(filename);
    timer = new QTimer(this);
    //connect(timer, SIGNAL(timeout()), this, SLOT(saveTime()));
    timer->setInterval(15000);
    timer->start();

    dataBase.connect();


    loadData();
}

hoteladmin::~hoteladmin()
{

}

void hoteladmin::loadData(){

    records = dataBase.getRecords();
    for(int i = 0; i < records.size(); i++){
    QListWidgetItem *item = new QListWidgetItem(records[i].ALL_roomClass + "\t" + QString::number(records[i].ALL_number) + "\t" +  QString::number(records[i].ALL_floor) + "\t" + QString::number(records[i].ALL_numberOfFreePlaces) + "\t" + QString::number(records[i].ALL_Cost));

    item->setData(Qt::UserRole, records[i].id);
    ui.browser->addItem(item);
    }
}


//Сохраняем данные в файл при закрытии программы
void hoteladmin::closeEvent(QCloseEvent *event){
    dataBase.close();
}


ClassName hoteladmin::getDataFromUi(){
    ClassName tmp;

    //Исправить взятие данных из ui.classSelection
    tmp.ALL_roomClass = ui.classSelection->currentItem()->data(Qt::UserRole).toString();
    tmp.ALL_whoIsThere = ui.whoTake->currentItem()->data(Qt::UserRole).toString();
    tmp.ALL_number = ui.room->value();
    tmp.ALL_floor = ui.floor->value();
    tmp.ALL_numberOfPlaces = ui.numberRoom->value();
    tmp.ALL_numberOfFreePlaces = ui.numberFreeRoom->value();
    tmp.ALL_Cost = ui.cost->value();
    tmp.ALL_note = ui.note->toPlainText();
    tmp.ALL_seaview = ui.seaview->isChecked();
    tmp.id = ui.browser->currentItem()->data(Qt::UserRole).toUInt();

    return tmp;
}

void hoteladmin::checkRangeFreeRoom()
{
	ui.numberFreeRoom->setRange(0,ui.numberRoom->value());
}
void hoteladmin::createTen()
{
    if (count<89)
    {

		QTextCodec* c = QTextCodec::codecForLocale();

        bool sea[10]={true,true,false,true,false,true,true,true,true,true};
        int number[10]={1,42,3,213,2,267,6,44,991,2};
		int floor[10]={1,2,3,4,2,3,6,24,10,21};
		QString classNumber[10]={c->toUnicode("Класс I"),c->toUnicode("Класс II"),c->toUnicode("Класс III"),c->toUnicode("Класс I"),c->toUnicode("Класс I"),c->toUnicode("Класс I"),
			c->toUnicode("Класс II"),c->toUnicode("Класс III"),c->toUnicode("Класс III"),c->toUnicode("Класс II")};
		int numberOfPlaces[10]={6,2,3,4,2,3,6,8,8,7};
		int numberOfFreePlaces[10]={1,0,3,3,2,1,0,0,0,0};
		int cost[10]={1000,200,3333,4442,232,5433,6000,8133,522,633};
		QString state[10]={c->toUnicode("смешанный состав"),c->toUnicode("муж."),c->toUnicode("пусто"),c->toUnicode("жен."),c->toUnicode("пусто"),c->toUnicode("смешанный состав"),c->toUnicode("смешанный состав"),
		c->toUnicode("смешанный состав"),c->toUnicode("муж."),c->toUnicode("муж.")};
		QString note[10]={c->toUnicode("Запись 1"),c->toUnicode("Запись 2"),c->toUnicode("Запись 3"),
			c->toUnicode("Запись 4"),c->toUnicode("Запись 5"),c->toUnicode("Запись 6"),
			c->toUnicode("Запись 7"),
		c->toUnicode("Запись 8")};

		for(int i=0;i<10;i++)
		{


			arr[count+i].ALL_number=number[i];
			arr[count+i].ALL_floor=floor[i];
			arr[count+i].ALL_roomClass=classNumber[i];
			arr[count+i].ALL_numberOfPlaces=numberOfPlaces[i];
			arr[count+i].ALL_numberOfFreePlaces=numberOfFreePlaces[i];
			arr[count+i].ALL_seaview=sea[i];
			arr[count+i].ALL_Cost=cost[i];
			arr[count+i].ALL_whoIsThere=state[i];
			arr[count+i].ALL_note=note[i];

		}
		
        for (int i=count;i<count+10;i++)
        {
            ui.browser->insertItem(i,c->toUnicode("Номер ") +  spaceNumber(i) + c->toUnicode(" этаж ") +  spaceFloor(i) + "\t"+arr[i].ALL_roomClass+ 
				"\t"+ QString::number(arr[i].ALL_numberOfFreePlaces)+ "\t"+ QString::number(arr[i].ALL_Cost));
        }
        count+=10;

   
    ui.btnFill->setDisabled(true);
	sort();
	 ui.browser->setCurrentRow(count-1);
	 hideShowRecord();
    }
}

void hoteladmin::sort()
{
	if(current!=-1)
	forFind=arr[current];
	    for (int i = 1; i < count; i++) {
        for (int j = 0; j < count - i; j++) {
            if (arr[j] > arr[j + 1]) {
               
                tmp = arr[j+ 1];
                arr[j+1] = arr[j ];
                arr[j ] = tmp;
            }
        }
    }
		QTextCodec* c = QTextCodec::codecForLocale();
		for (int i=0;i<count;i++)
        {
            ui.browser->item(i)->setText(c->toUnicode("Номер ") +  spaceNumber(i) + c->toUnicode(" этаж ") + spaceFloor(i)+
				"\t"+arr[i].ALL_roomClass+ "\t"+ QString::number(arr[i].ALL_numberOfFreePlaces)+ "\t"+ QString::number(arr[i].ALL_Cost));
        }
		 for (int i = 0; i < count; i++) 
			 if(forFind==arr[i])
			 {
				 ui.browser->setCurrentRow(i);
			 }
}

void hoteladmin::hideShowRecord()
{
	if (count<=0)
	{
		ui.room->setDisabled(true);
		ui.floor->setDisabled(true);
		ui.numberRoom->setDisabled(true);
		ui.numberFreeRoom->setDisabled(true);
		ui.whoTake->setDisabled(true);
		ui.note->setDisabled(true);
		ui.cost->setDisabled(true);
		ui.classSelection->setDisabled(true);
		ui.seaview->setDisabled(true);
	}
	else
		if(count>0)
	{
		ui.room->setDisabled(false);
		ui.floor->setDisabled(false);
		ui.numberRoom->setDisabled(false);
		ui.numberFreeRoom->setDisabled(false);
		ui.whoTake->setDisabled(false);
		ui.note->setDisabled(false);
		ui.cost->setDisabled(false);
		ui.classSelection->setDisabled(false);
		ui.seaview->setDisabled(false);
	}
}

void  hoteladmin::nextRecord(int index)
{
	
    current=index;
	if(current!=-1)
    returnAll();

}
//нажата кнопка "Создать"
//функция добавления элемента в listWidget
void hoteladmin::newItem()
{
	QTextCodec* c = QTextCodec::codecForLocale();
	
        ui.browser->insertItem(count,c->toUnicode("Номер ") + spaceNumber(count) + c->toUnicode(" этаж ") + spaceFloor(count)+
			"\t"+ arr[count].ALL_roomClass+ "\t"+ QString::number(arr[count].ALL_numberOfFreePlaces)+ "\t"+ QString::number(arr[count].ALL_Cost));
		sort();
        ui.browser->setCurrentRow(count);
        count++;
		hideShowRecord();
		current=ui.browser->currentRow();

}

//нажата кнопка "Сохранить"
void hoteladmin::save()
{
   saveAll();
   QTextCodec* c = QTextCodec::codecForLocale();
     if (count>0)
   ui.browser->item(current)->setText(c->toUnicode("Номер ") + spaceNumber(count) + c->toUnicode(" этаж ") +  spaceFloor(count)+
   "\t"+ arr[count].ALL_roomClass+ "\t"+ QString::number(arr[count].ALL_numberOfFreePlaces)+ "\t"+ QString::number(arr[count].ALL_Cost));
	 sort();
}

//нажата кнопка "Удалить"
void hoteladmin::destroy()
{
    if (count>0)
        {
            arr[ui.browser->currentRow()]=clear;
            QListWidgetItem *item = ui.browser->takeItem(ui.browser->currentRow());
            delete item;
            count--;
			hideShowRecord();
            for(int i=ui.browser->currentRow();i>0&&i<count;i++)
            {
                arr[i]=arr[i+1];
                arr[i+1]=clear;
            }

        }
	

}

void hoteladmin::whoTakeCheck()
{
	QTextCodec* c = QTextCodec::codecForLocale();
	QString state[4]={c->toUnicode("пусто"),c->toUnicode("муж."),c->toUnicode("жен."),c->toUnicode("смешанный состав")};
    int n,m;
	QString tmp;
	if(ui.whoTake->currentItem())
	tmp =ui.whoTake->currentItem()->text();
    n = ui.numberFreeRoom->value();
    m = ui.numberRoom->value();
    if (n<m && n>=0 && m-n>=1 && m-n<2)
    {
		ui.whoTake->clear();
        ui.whoTake->addItem(state[1]);
		ui.whoTake->addItem(state[2]);

    }
	else if (n<m && n>=0 && m-n>=2)
	{
		ui.whoTake->clear();
        ui.whoTake->addItem(state[1]);
		ui.whoTake->addItem(state[2]);
		ui.whoTake->addItem(state[3]);
	}
	else 
	{
		ui.whoTake->clear();
        ui.whoTake->addItem(state[0]);
	}
	if(arr[current].ALL_whoIsThere!="" && !ui.whoTake->findItems(arr[current].ALL_whoIsThere,0).empty())
	{
         ui.whoTake->setCurrentItem(ui.whoTake->findItems(arr[current].ALL_whoIsThere,0).takeFirst());
	}
	if(count>0 && !ui.whoTake->currentItem() && tmp!="" && !ui.whoTake->findItems(tmp,0).empty())
	ui.whoTake->setCurrentItem(ui.whoTake->findItems(tmp,0).takeFirst());

}
    //функция сохранения данных
    void hoteladmin::saveAll()
    {

        arr[current].ALL_number = ui.room->value();
        arr[current].ALL_floor = ui.floor->value();
		 if(ui.classSelection->currentItem())
        arr[current]. ALL_roomClass =ui.classSelection->currentItem()->text();
        arr[current]. ALL_seaview = ui.seaview->isChecked();
         arr[current].ALL_numberOfPlaces = ui.numberRoom->value();
         arr[current].ALL_numberOfFreePlaces = ui.numberFreeRoom->value();
		 if(ui.whoTake->currentItem())
        arr[current]. ALL_whoIsThere = ui.whoTake->currentItem()->text();
         arr[current].ALL_Cost = ui.cost->value();
        arr[current]. ALL_note = ui.note->toPlainText();
    }
    void  hoteladmin::returnAll()
        {
         ui.room->setValue(arr[current].ALL_number);
         ui.floor->setValue(arr[current].ALL_floor);
		 if(arr[current].ALL_roomClass!="")
         ui.classSelection->setCurrentItem(ui.classSelection->findItems(arr[current].ALL_roomClass,0).takeFirst());
		 else ui.classSelection->setCurrentRow(-1);
         ui.seaview->setChecked(arr[current].ALL_seaview);
         ui.numberRoom->setValue(arr[current].ALL_numberOfPlaces);
         ui.numberFreeRoom->setValue(arr[current].ALL_numberOfFreePlaces);
		 if(arr[current].ALL_whoIsThere!="")
         ui.whoTake->setCurrentItem(ui.whoTake->findItems(arr[current].ALL_whoIsThere,0).takeFirst());
		 else  ui.whoTake->setCurrentRow(-1);
         ui.cost->setValue(arr[current].ALL_Cost);
         ui.note->setText(arr[current].ALL_note);
        }


	QString hoteladmin::spaceNumber(int i)
	{
	if(arr[i].ALL_number<10)
        return  (QString("   ") +QString::number(arr[i].ALL_number));
	else
		if(arr[i].ALL_number<100)
        return  (QString("  ") +QString::number(arr[i].ALL_number));
		else
			if(arr[i].ALL_number<1000)
        return  (QString(" ") +QString::number(arr[i].ALL_number));
			else
	if(arr[i].ALL_number==1000)
		return  (QString::number(arr[i].ALL_number));

	}

	QString hoteladmin::spaceFloor(int i)
	{
	if(arr[i].ALL_floor<10)
        return  (QString("   ") +QString::number(arr[i].ALL_floor));
	else
		if(arr[i].ALL_floor<100)
        return  (QString("  ") +QString::number(arr[i].ALL_floor));
		else
			if(arr[i].ALL_floor<1000)
        return  (QString(" ") +QString::number(arr[i].ALL_floor));
			else
	if(arr[i].ALL_floor==1000)
		return  (QString::number(arr[i].ALL_floor));
	}

    /*void hoteladmin::saveTime(){
        writeData(filename);
    }

    void hoteladmin::closeEvent(QCloseEvent *event){
        writeData(filename);
    }*/

    /*QString config(){

        char buff[1000] ="0";
        QString configData;


        DWORD lpNumberOfBytesRead;//Кол-во прочитаных байтов, если 0 то уже конец файла

        DWORD dwCreate=OPEN_ALWAYS;

        HANDLE config = CreateFileA("config.ini", GENERIC_READ , NULL, NULL, OPEN_ALWAYS , FILE_ATTRIBUTE_NORMAL, NULL);
        if (config == INVALID_HANDLE_VALUE)   //если ошибка то выходим INVALID_HANDLE_VALUE
        {
            MessageBox(NULL, TEXT("Client's error! File didn't opened!"), TEXT("Warning"), MB_OK);
            CloseHandle(config);
        }
        do //тут обьявляем цикл с пост условием потомучто мы не знаем какой длины файл и будем читать до конца, когда lpNumberOfBytesRead будет равна 0 тогда файл прочитан
        {
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


    void hoteladmin::writeData(QString filename){
        QTextCodec* c = QTextCodec::codecForLocale();
        DWORD bytesRead;
        QByteArray bpath = filename.toLocal8Bit();
        char *path = bpath.data();
        HANDLE hFile = CreateFileA(path, GENERIC_WRITE , NULL, NULL, CREATE_ALWAYS , FILE_ATTRIBUTE_NORMAL, NULL);
        if (INVALID_HANDLE_VALUE == hFile) {
            return;
        }
        for(int i = 0; i< count;i++){
                QString data;
               //QString dateBirth =countRecord[i].dateBirth.toString("dd.MM.yyyy");
                //QString dateDeath = countRecord[i].dateDeath.toString("dd.MM.yyyy");
             data += arr[i].ALL_roomClass + ";" + arr[i].ALL_whoIsThere + ";" +  QString::number(arr[i].ALL_number) + ";"
                     + QString::number(arr[i].ALL_floor) + ";" + QString::number(arr[i].ALL_numberOfPlaces) + ";" + QString::number(arr[i].ALL_numberOfFreePlaces) + ";"
                     + QString::number(arr[i].ALL_Cost) + ";" + arr[i].ALL_note + QString::number(arr[i].ALL_seaview) +'\r \n';
             QByteArray ba = data.toLocal8Bit();

        char *strm2 = ba.data();
        WriteFile(hFile,  strm2, strlen(strm2), &bytesRead, NULL);
        WriteFile(hFile,  "\r\n", 2, NULL, NULL);
        }



        QByteArray ba = data.toLocal8Bit();
        char *strm2 = ba.data();

        //DWORD bytesRead;


        //WriteFile(hFile,  strm2, strlen(strm2), &bytesRead, NULL);
        CloseHandle(hFile);

    }

    void hoteladmin::readData(QString filename)
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
                OPEN_EXISTING, //говорим что открываем файл который существует или создаем его
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

            arr[i] = tmpDB;

            ui.browser->addItem(arr[i].ALL_roomClass + "\t" + QString::number(arr[i].ALL_number) + "\t" +  QString::number(arr[i].ALL_floor) + "\t" + QString::number(arr[i].ALL_numberOfFreePlaces) + "\t" + QString::number(arr[i].ALL_Cost));
            count++;
        }
        ui.browser->setCurrentRow(0);

    }*/
