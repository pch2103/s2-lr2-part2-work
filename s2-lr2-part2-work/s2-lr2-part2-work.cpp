// s2-lr2-part2.cpp 
//
// Создать новый класс fio, данные - члены класса(все pri) :
//	char	*familia, 	// указатели на строки с фамилией,
//	*imya, 	// именем и отчеством
//	*otchestvo;

//Тестовая программа №2 :
//-- Выполнить все действия Тестовой программы №1
//-- Создать объект класса clinic, используя конструктор с параметрами – это эталонный объект(ЭО)
//-- Найти ЭО в базе, если не найдено, то дополнить базу на основе копирования ЭО. Если найдено, то посчитать количество найденных ЭО.

#include <iostream>
#include <cstring>
#include <sstream>
#include <Windows.h>

#pragma warning (disable : 4996) //Без этого ругается на fopen и strcpy https://stackoverflow.com/questions/14386/fopen-deprecated-warning

#define FIO_WITH 60
#define CHARACTER_WITH 20
#define BASE_NAME "clinicBasePart2.txt" //Структура файла будет отличаться от базового варианта с одним классом

class Clinic;

class fio {
	char* familia;
	char* imya;
	char* otchestvo;

public:
	fio() //конструктор по-умолчанию
	{
		std::cout << "fio::Kонструктор по-умолчанию" << std::endl;
		familia = new char[CHARACTER_WITH];
		strcpy(familia, "");
		imya = new char[CHARACTER_WITH];
		strcpy(imya, "");
		otchestvo = new char[CHARACTER_WITH];
		strcpy(otchestvo, "");
	}

	fio(const char* _fio) //конструктор c параметрами в виде одной строки
	{
		std::cout << "fio::Kонструктор c параметрами в виде одной строки" << std::endl;

		char buff[3][CHARACTER_WITH] = { "", "", "" };
		sscanf(_fio, "%s %s %s", buff[0], buff[1], buff[2]);

		familia = new char[strlen(buff[0]) + 1];
		imya = new char[strlen(buff[1]) + 1];
		otchestvo = new char[strlen(buff[2]) + 1];

		strcpy(familia, buff[0]);
		strcpy(imya, buff[1]);
		strcpy(otchestvo, buff[2]);
	}

	fio(const fio& F)
		: familia(new char[strlen(F.familia) + 1]), imya(new char[strlen(F.imya) + 1]), otchestvo(new char[strlen(F.otchestvo) + 1])
		//Kонструктор Копирования
	{
		std::cout << "fio::Kонструктор Копирования" << std::endl;
		strcpy(familia, F.familia);
		strcpy(imya, F.imya);
		strcpy(otchestvo, F.otchestvo);
	}


	fio& operator= (const fio &F) // Перегрузка оператора "="
	{
		std::cout << "fio::Оператор =" << std::endl;

		if (this == &F) return *this; //самоприсваивание

		delete[] familia;
		delete[] imya;
		delete[] otchestvo;

		familia = new char[strlen(F.familia) + 1];
		imya = new char[strlen(F.imya) + 1];
		otchestvo = new char[strlen(F.otchestvo) + 1];

		strcpy(familia, F.familia);
		strcpy(imya, F.imya);
		strcpy(otchestvo, F.otchestvo);

		return *this;
	}

	~fio()
	{
		std::cout << "~fio::Деструктор" << std::endl;
		delete[] familia;
		delete[] imya;
		delete[] otchestvo;
	}

	char* getFio() {
		char* _fio = new char[FIO_WITH];

		sprintf(_fio, "%s %s %s\0", familia, imya, otchestvo);

		return _fio;
	}

}; //class fio

class Clinic {
	fio doctorFio;
	int specialty;
	int qualification;
	static int count;
public:

	Clinic() : doctorFio() //конструктор по-умолчанию
	{
		std::cout << "Clinic::Kонструктор по-умолчанию" << std::endl;
	}

	//конструктор c параметрами
	Clinic(fio _fio, int _specialty = 3, int _qualification = 2) :
		doctorFio(_fio), specialty(_specialty), qualification(_qualification)
	{
		std::cout << "Clinic::Kонструктор c параметрами" << std::endl;
	}


	Clinic(const Clinic& CL) :
		specialty(CL.specialty), qualification(CL.qualification), doctorFio(CL.doctorFio)
		//Kонструктор Копирования
	{
		std::cout << "Clinic::Kонструктор Копирования" << std::endl;
	}

	~Clinic()
	{
		std::cout << "~Clinic::Деструктор" << std::endl;
	}


	Clinic& operator= (const Clinic &CL) // Перегрузка оператора "="
	{
		if (this == &CL) return *this; //самоприсваивание

		std::cout << "Clinic::Оператор =" << std::endl;
		doctorFio = CL.doctorFio;
		specialty = CL.specialty;
		qualification = CL.qualification;

		return *this;
	}

	//ДФ - дружественные функции ===============================================
	void friend showOneRecord(Clinic, int); //
	int friend fioSearch(Clinic*, char* F); //

	//МК - методы класса ===============================================
	Clinic* loadRecords();
	void enterNewRecord(); //ввод новой записи в единичную структуру потом она будет записана в addNewRecord()
	Clinic* addNewRecord(Clinic*); //выделение памяти под новую структу и заполнение ее данными
	void exitAndSave(); // запись базы из памяти на диск при выходе
	void qualificationFilter();  //Фильтрует вывод по квалификации
	void specialtyFilter();  //Фильтрует вывод по квалификации
	void sortRecordsByAlpha(); //Сортировка в алфавитном порядке

	int get_count() { return count; } //получить количество записей в памяти
	void add_count() { ++count; } //увеличить количество записей в памяти на 1 (при добавлении записи)
}; //class Clinic

int Clinic::count = 0;

//протоипы функций
void showAllRecords(Clinic*); //показывает все записи в памяти
void showRecordHeader(const char*); //вывод заголовка таблицы для записей
Clinic* testProgrammNumber2(Clinic*); //Тестовая программа номер 2
int menu(); //Меню функций базы

int main()
{
	SetConsoleCP(1251);
	SetConsoleOutputCP(1251);

	int choice;

	Clinic* myClynic = nullptr;
	Clinic tmpClynic;


	myClynic = myClynic->loadRecords();
	
	myClynic = testProgrammNumber2(myClynic); //Тестовая программа номер 2

	do {
		choice = menu();

		switch (choice)
		{
		case 1: //1 - Добавить новую запись
			tmpClynic.enterNewRecord();
			myClynic = tmpClynic.addNewRecord(myClynic);
			break;
		case 2: //2 - Показать все записи
			showAllRecords(myClynic);
			break;
					case 3: //3 - Поиск по ФИО
						fioSearch(myClynic, nullptr);  //Поиск по ФИО (ДФ)
						break;
					case 4: //4 - Фильтр по квалификации
						myClynic->qualificationFilter();  //Фильтрует вывод по квалификации
						break;
					case 5: //5 - Фильтр по специальности
						myClynic->specialtyFilter();  //Фильтрует вывод по специальности
						break;
					case 6: //6 - Сортировка в алфавитном порядке
						myClynic->sortRecordsByAlpha();
						break;
		default:
			break;
		}
	} while (choice != 0);

	myClynic->exitAndSave(); // запись базы из памяти на диск при выходе

	return 0;
} //main

//FFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFF
// ТЕСТОВАЯ ПРОГРАММА НОМЕР 2
//FFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFF

Clinic* testProgrammNumber2(Clinic* CL) //Тестовая программа номер 2
{
	std::cout << "---ТЕСТОВАЯ ПРОГРАММА НОМЕР 2 :: НАЧАЛО---ТЕСТОВАЯ ПРОГРАММА НОМЕР 2 :: НАЧАЛО---ТЕСТОВАЯ ПРОГРАММА НОМЕР 2 :: НАЧАЛО---" << std::endl;
	char _fio[FIO_WITH] = "Аристархов Аристарх Аристархович";
	fio testFio(_fio); //fio для теста
	Clinic testClinic(testFio); //Эталонный объект (ЭО) - конструктор с параметрами

	int answer = fioSearch(CL, _fio);
	std::cout << "!!!! Найдено эталонных объектов (ЭО): " << answer << std::endl;
	if (answer == 0) {
		std::cout << "!!!! База будет дополнена эталонным объектом (ЭО): " << std::endl;
		CL = testClinic.addNewRecord(CL);
	}
	std::cout << "---ТЕСТОВАЯ ПРОГРАММА НОМЕР 2 :: КОНЕЦ---ТЕСТОВАЯ ПРОГРАММА НОМЕР 2 :: КОНЕЦ---ТЕСТОВАЯ ПРОГРАММА НОМЕР 2 :: КОНЕЦ" << std::endl;

	return CL;
}

//FFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFF
// РЕАЛИЗАЦИЯ МЕТОДОВ
//FFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFF

Clinic* Clinic::loadRecords() //загрузка данных из файла и вывод их на экран и загрузка в пямять для работы
{
	int specialty, qualification;

	char* _fio;
	_fio = new char[FIO_WITH];
	fio _F;

	Clinic T;
	Clinic* PT = nullptr;

	FILE* FP = fopen(BASE_NAME, "r");

	if (FP != NULL) {
		std::cout << std::endl << "+++++++++++++++++++++++++++++++++++++" << std::endl;
		std::cout << "Загрузка текущей базы:" << std::endl;
		while (fscanf(FP, "%99[^\n]%d\n%d\n", _fio, &specialty, &qualification) != EOF)
		{
			std::cout << _fio << " " << specialty << " " << qualification << std::endl;
			_F = fio(_fio);
			T = Clinic(_F, specialty, qualification);
			PT = T.addNewRecord(PT);
		}
		std::cout << std::endl << "+++++++++++++++++++++++++++++++++++++" << std::endl;
		fclose(FP);

		return PT;
	}

	return nullptr;
} //loadRecords()

void Clinic::exitAndSave() // запись базы из памяти на диск при выходе
{
	int amount = this->get_count();

	FILE* FP = fopen(BASE_NAME, "w");

	for (int i = 0; i < amount; ++i) {
		fprintf(FP, "%s\n%d\n%d\n", (this + i)->doctorFio.getFio(), (this + i)->specialty, (this + i)->qualification);
	}

	if (FP != NULL)
	{
		fclose(FP);
	}
} //exitAndSave()

void Clinic::enterNewRecord() //ввод новой записи в единичную структуру потом она будет записана в addNewRecord()
{
	char buffer[FIO_WITH];
	int spec, qual;

	std::cout << "==================================" << std::endl;
	std::cout << "Добавление новой записи:" << std::endl;
	std::cout << "==================================" << std::endl;

	std::cout << "ФИО врача: ";
	std::cin.getline(buffer, FIO_WITH);

	this->doctorFio = fio::fio(buffer);

	do
	{
		std::cout << "Cпециальность (1-терапевт, 2-кардиолог, 3-стоматолог): ";
		std::cin >> spec;
		std::cin.get();
	} while (spec < 1 || spec > 3);

	do
	{
		std::cout << "Квалификация (0-высшая, 1-первая, 2-вторая): ";
		std::cin >> qual;
		std::cin.get();
	} while (qual < 0 || qual > 2);

	this->specialty = spec;
	this->qualification = qual;

} // enterNewRecord()

Clinic* Clinic::addNewRecord(Clinic* P) //выделение памяти под новую структу и заполнение ее данными
{
	int amount = this->get_count();

	if (amount == 0)
	{
		P = new Clinic(this->doctorFio, this->specialty, this->qualification);
	}
	else
	{
		Clinic *T = new Clinic[amount + 1];

		for (int i = 0; i < amount; ++i)  // копируем во временный объект	
		{
			T[i] = P[i];
			P[i].~Clinic(); // Удаляем ранние записи из памяти (delete[] P; - не работает) вызываем деструктор явно для каждой записи
		}

		T[amount] = Clinic(this->doctorFio, this->specialty, this->qualification); //Добавляем новую запись в память
		//delete[] P; - не работает см. выше
		P = T; //временный объект становится "присланным"
	}

	this->add_count(); // Увеличиваем счетчик (static-переменная)

	return P;
} // addNewRecord()

void Clinic::sortRecordsByAlpha() //Сортировка базы (в памяти) в алфавитном порядке
{
	int i, j, amount = this->get_count();

	Clinic T;

	for (i = 0; i < amount; ++i) {
		for (j = i + 1; j < amount; ++j) {
			if (strcmp(this[i].doctorFio.getFio(), this[j].doctorFio.getFio()) > 0) {
				T = this[i];
				this[i] = this[j];
				this[j] = T;
			}
		} //for j
	} //for i

	showAllRecords(this);
} //sortRecordsByAlpha()

int fioSearch(Clinic* CL, char* _fio)  //Поиск по ФИО (ДФ)
{
	int count = 0;
	int amount = CL->get_count();
	char buffer[FIO_WITH];

	if (_fio == nullptr) { //не передана ФИО для поиска - нужно вводить с клавиатуры
		std::cout << "===============================================================================================" << std::endl;
		std::cout << "Ведите ФИО врача для поиска: ";
		std::cin.getline(buffer, FIO_WITH);
		_fio = new char[strlen(buffer) + 1];
		strcpy(_fio, buffer);
	}

	std::cout << "===============================================================================================" << std::endl;

	showRecordHeader("Поиск по ФИО врача:");

	for (int i = 0; i < amount; ++i) {
		if (strcmp(_fio, CL[i].doctorFio.getFio()) == 0) {
			++count;
			showOneRecord(CL[i], count);
		}
	} //for
	if (count == 0) std::cout << "Записей для введенных ФИО нет" << std::endl;
	std::cout << "===============================================================================================" << std::endl;

	return count; //возвращаем количество найденных записей
} //fioSearch()

void Clinic::qualificationFilter()  //Фильтрует вывод по квалификации
{
	int amount = this->get_count();
	int qual;

	do
	{
		std::cout << "===============================================================================================" << std::endl;
		std::cout << "Код квалификации (0-высшая, 1-первая, 2-вторая): ";
		std::cin >> qual;
		std::cin.get();
	} while (qual < 0 || qual > 2);
	std::cout << "===============================================================================================" << std::endl;

	showRecordHeader("Фильтр по квалификации:");

	for (int i = 0; i < amount; ++i) {
		if (qual == this[i].qualification) {
			showOneRecord(this[i], i + 1);
		}
	} //for
	std::cout << "===============================================================================================" << std::endl;
} //qualificationFilter()

void Clinic::specialtyFilter()  //Фильтрует вывод по специальности
{
	int amount = this->get_count();
	int spec;

	do
	{
		std::cout << "===============================================================================================" << std::endl;
		std::cout << "Код специальности (1-терапевт, 2-кардиолог, 3-стоматолог): ";
		std::cin >> spec;
		std::cin.get();
	} while (spec < 0 || spec > 2);
	std::cout << "===============================================================================================" << std::endl;

	showRecordHeader("Фильтр по специальности:");

	for (int i = 0; i < amount; ++i) {
		if (spec == this[i].specialty) {
			showOneRecord(this[i], i + 1);
		}
	} //for
	std::cout << "===============================================================================================" << std::endl;
} //specialtyFilter()

void showRecordHeader(const char* title) //вспомогательная функция - печать заголовка таблицы
{
	std::cout << std::endl << "===============================================================================================" << std::endl;
	std::cout << title;
	std::cout << std::endl << "===============================================================================================" << std::endl;
	std::cout.setf(std::ios::left);
	std::cout.width(6);
	std::cout << " №";
	std::cout.setf(std::ios::left);
	std::cout.width(FIO_WITH);
	std::cout << "ФИО врача";
	std::cout.setf(std::ios::left);
	std::cout.width(16);
	std::cout << "Cпециальность";
	std::cout.setf(std::ios::left);
	std::cout.width(16);
	std::cout << "Квалификация";
	std::cout << std::endl << "===============================================================================================" << std::endl;
}

void showOneRecord(Clinic Record, int number = 1) //вспомогательная функция - печать одной записи
{
	std::cout.setf(std::ios::left);
	std::cout << " ";
	std::cout.width(5);
	std::cout << number;
	std::cout.setf(std::ios::left);
	std::cout.width(FIO_WITH);
	std::cout << Record.doctorFio.getFio();
	std::cout.setf(std::ios::left);
	std::cout.width(16);
	std::cout << Record.specialty;
	std::cout.setf(std::ios::left);
	std::cout.width(16);
	std::cout << Record.qualification;
	std::cout << std::endl;
}

void showAllRecords(Clinic* P) //показывает все записи в памяти
{
	showRecordHeader("Все записи в базе поликлиники:");

	for (int i = 0; i < P->get_count(); ++i) {
		showOneRecord(P[i], i + 1);
	}
	std::cout << "===============================================================================================" << std::endl;
}// showAllRecords()

int menu() //Меню функций базы
{
	int select;
	std::cout << "==================================" << std::endl;
	std::cout << "Меню:" << std::endl;
	std::cout << "==================================" << std::endl;
	std::cout << "1 - Добавить новую запись" << std::endl;
	std::cout << "2 - Показать все записи" << std::endl;
	std::cout << "3 - Поиск врача по ФИО" << std::endl;
	std::cout << "4 - Фильтр по квалификации" << std::endl;
	std::cout << "5 - Фильтр по специальности" << std::endl;
	std::cout << "6 - Сортировка в алфавитном порядке" << std::endl;
	std::cout << "0 - Выход из программы и запись" << std::endl;
	std::cout << "----------------------------------" << std::endl;

	do
	{
		std::cout << "Введите номер пункта меню: ";
		std::cin >> select;
		std::cin.get();
	} while (select < 0 || select > 6);

	return select;
} // menu()