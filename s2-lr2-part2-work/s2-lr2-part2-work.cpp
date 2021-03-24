// s2-lr2-part2.cpp 
//
// ������� ����� ����� fio, ������ - ����� ������(��� pri) :
//	char	*familia, 	// ��������� �� ������ � ��������,
//	*imya, 	// ������ � ���������
//	*otchestvo;

//�������� ��������� �2 :
//-- ��������� ��� �������� �������� ��������� �1
//-- ������� ������ ������ clinic, ��������� ����������� � ����������� � ��� ��������� ������(��)
//-- ����� �� � ����, ���� �� �������, �� ��������� ���� �� ������ ����������� ��. ���� �������, �� ��������� ���������� ��������� ��.

#include <iostream>
#include <cstring>
#include <sstream>
#include <Windows.h>

#pragma warning (disable : 4996) //��� ����� �������� �� fopen � strcpy https://stackoverflow.com/questions/14386/fopen-deprecated-warning

#define FIO_WITH 60
#define CHARACTER_WITH 20
#define BASE_NAME "clinicBasePart2.txt" //��������� ����� ����� ���������� �� �������� �������� � ����� �������

class Clinic;

class fio {
	char* familia;
	char* imya;
	char* otchestvo;

public:
	fio() //����������� ��-���������
	{
		std::cout << "fio::K���������� ��-���������" << std::endl;
		familia = new char[CHARACTER_WITH];
		strcpy(familia, "");
		imya = new char[CHARACTER_WITH];
		strcpy(imya, "");
		otchestvo = new char[CHARACTER_WITH];
		strcpy(otchestvo, "");
	}

	fio(const char* _fio) //����������� c ����������� � ���� ����� ������
	{
		std::cout << "fio::K���������� c ����������� � ���� ����� ������" << std::endl;

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
		//K���������� �����������
	{
		std::cout << "fio::K���������� �����������" << std::endl;
		strcpy(familia, F.familia);
		strcpy(imya, F.imya);
		strcpy(otchestvo, F.otchestvo);
	}


	fio& operator= (const fio &F) // ���������� ��������� "="
	{
		std::cout << "fio::�������� =" << std::endl;

		if (this == &F) return *this; //����������������

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
		std::cout << "~fio::����������" << std::endl;
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

	Clinic() : doctorFio() //����������� ��-���������
	{
		std::cout << "Clinic::K���������� ��-���������" << std::endl;
	}

	//����������� c �����������
	Clinic(fio _fio, int _specialty = 3, int _qualification = 2) :
		doctorFio(_fio), specialty(_specialty), qualification(_qualification)
	{
		std::cout << "Clinic::K���������� c �����������" << std::endl;
	}


	Clinic(const Clinic& CL) :
		specialty(CL.specialty), qualification(CL.qualification), doctorFio(CL.doctorFio)
		//K���������� �����������
	{
		std::cout << "Clinic::K���������� �����������" << std::endl;
	}

	~Clinic()
	{
		std::cout << "~Clinic::����������" << std::endl;
	}


	Clinic& operator= (const Clinic &CL) // ���������� ��������� "="
	{
		if (this == &CL) return *this; //����������������

		std::cout << "Clinic::�������� =" << std::endl;
		doctorFio = CL.doctorFio;
		specialty = CL.specialty;
		qualification = CL.qualification;

		return *this;
	}

	//�� - ������������� ������� ===============================================
	void friend showOneRecord(Clinic, int); //
	int friend fioSearch(Clinic*, char* F); //

	//�� - ������ ������ ===============================================
	Clinic* loadRecords();
	void enterNewRecord(); //���� ����� ������ � ��������� ��������� ����� ��� ����� �������� � addNewRecord()
	Clinic* addNewRecord(Clinic*); //��������� ������ ��� ����� ������� � ���������� �� �������
	void exitAndSave(); // ������ ���� �� ������ �� ���� ��� ������
	void qualificationFilter();  //��������� ����� �� ������������
	void specialtyFilter();  //��������� ����� �� ������������
	void sortRecordsByAlpha(); //���������� � ���������� �������

	int get_count() { return count; } //�������� ���������� ������� � ������
	void add_count() { ++count; } //��������� ���������� ������� � ������ �� 1 (��� ���������� ������)
}; //class Clinic

int Clinic::count = 0;

//�������� �������
void showAllRecords(Clinic*); //���������� ��� ������ � ������
void showRecordHeader(const char*); //����� ��������� ������� ��� �������
Clinic* testProgrammNumber2(Clinic*); //�������� ��������� ����� 2
int menu(); //���� ������� ����

int main()
{
	SetConsoleCP(1251);
	SetConsoleOutputCP(1251);

	int choice;

	Clinic* myClynic = nullptr;
	Clinic tmpClynic;


	myClynic = myClynic->loadRecords();
	
	myClynic = testProgrammNumber2(myClynic); //�������� ��������� ����� 2

	do {
		choice = menu();

		switch (choice)
		{
		case 1: //1 - �������� ����� ������
			tmpClynic.enterNewRecord();
			myClynic = tmpClynic.addNewRecord(myClynic);
			break;
		case 2: //2 - �������� ��� ������
			showAllRecords(myClynic);
			break;
					case 3: //3 - ����� �� ���
						fioSearch(myClynic, nullptr);  //����� �� ��� (��)
						break;
					case 4: //4 - ������ �� ������������
						myClynic->qualificationFilter();  //��������� ����� �� ������������
						break;
					case 5: //5 - ������ �� �������������
						myClynic->specialtyFilter();  //��������� ����� �� �������������
						break;
					case 6: //6 - ���������� � ���������� �������
						myClynic->sortRecordsByAlpha();
						break;
		default:
			break;
		}
	} while (choice != 0);

	myClynic->exitAndSave(); // ������ ���� �� ������ �� ���� ��� ������

	return 0;
} //main

//FFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFF
// �������� ��������� ����� 2
//FFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFF

Clinic* testProgrammNumber2(Clinic* CL) //�������� ��������� ����� 2
{
	std::cout << "---�������� ��������� ����� 2 :: ������---�������� ��������� ����� 2 :: ������---�������� ��������� ����� 2 :: ������---" << std::endl;
	char _fio[FIO_WITH] = "���������� �������� ������������";
	fio testFio(_fio); //fio ��� �����
	Clinic testClinic(testFio); //��������� ������ (��) - ����������� � �����������

	int answer = fioSearch(CL, _fio);
	std::cout << "!!!! ������� ��������� �������� (��): " << answer << std::endl;
	if (answer == 0) {
		std::cout << "!!!! ���� ����� ��������� ��������� �������� (��): " << std::endl;
		CL = testClinic.addNewRecord(CL);
	}
	std::cout << "---�������� ��������� ����� 2 :: �����---�������� ��������� ����� 2 :: �����---�������� ��������� ����� 2 :: �����" << std::endl;

	return CL;
}

//FFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFF
// ���������� �������
//FFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFF

Clinic* Clinic::loadRecords() //�������� ������ �� ����� � ����� �� �� ����� � �������� � ������ ��� ������
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
		std::cout << "�������� ������� ����:" << std::endl;
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

void Clinic::exitAndSave() // ������ ���� �� ������ �� ���� ��� ������
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

void Clinic::enterNewRecord() //���� ����� ������ � ��������� ��������� ����� ��� ����� �������� � addNewRecord()
{
	char buffer[FIO_WITH];
	int spec, qual;

	std::cout << "==================================" << std::endl;
	std::cout << "���������� ����� ������:" << std::endl;
	std::cout << "==================================" << std::endl;

	std::cout << "��� �����: ";
	std::cin.getline(buffer, FIO_WITH);

	this->doctorFio = fio::fio(buffer);

	do
	{
		std::cout << "C������������ (1-��������, 2-���������, 3-����������): ";
		std::cin >> spec;
		std::cin.get();
	} while (spec < 1 || spec > 3);

	do
	{
		std::cout << "������������ (0-������, 1-������, 2-������): ";
		std::cin >> qual;
		std::cin.get();
	} while (qual < 0 || qual > 2);

	this->specialty = spec;
	this->qualification = qual;

} // enterNewRecord()

Clinic* Clinic::addNewRecord(Clinic* P) //��������� ������ ��� ����� ������� � ���������� �� �������
{
	int amount = this->get_count();

	if (amount == 0)
	{
		P = new Clinic(this->doctorFio, this->specialty, this->qualification);
	}
	else
	{
		Clinic *T = new Clinic[amount + 1];

		for (int i = 0; i < amount; ++i)  // �������� �� ��������� ������	
		{
			T[i] = P[i];
			P[i].~Clinic(); // ������� ������ ������ �� ������ (delete[] P; - �� ��������) �������� ���������� ���� ��� ������ ������
		}

		T[amount] = Clinic(this->doctorFio, this->specialty, this->qualification); //��������� ����� ������ � ������
		//delete[] P; - �� �������� ��. ����
		P = T; //��������� ������ ���������� "����������"
	}

	this->add_count(); // ����������� ������� (static-����������)

	return P;
} // addNewRecord()

void Clinic::sortRecordsByAlpha() //���������� ���� (� ������) � ���������� �������
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

int fioSearch(Clinic* CL, char* _fio)  //����� �� ��� (��)
{
	int count = 0;
	int amount = CL->get_count();
	char buffer[FIO_WITH];

	if (_fio == nullptr) { //�� �������� ��� ��� ������ - ����� ������� � ����������
		std::cout << "===============================================================================================" << std::endl;
		std::cout << "������ ��� ����� ��� ������: ";
		std::cin.getline(buffer, FIO_WITH);
		_fio = new char[strlen(buffer) + 1];
		strcpy(_fio, buffer);
	}

	std::cout << "===============================================================================================" << std::endl;

	showRecordHeader("����� �� ��� �����:");

	for (int i = 0; i < amount; ++i) {
		if (strcmp(_fio, CL[i].doctorFio.getFio()) == 0) {
			++count;
			showOneRecord(CL[i], count);
		}
	} //for
	if (count == 0) std::cout << "������� ��� ��������� ��� ���" << std::endl;
	std::cout << "===============================================================================================" << std::endl;

	return count; //���������� ���������� ��������� �������
} //fioSearch()

void Clinic::qualificationFilter()  //��������� ����� �� ������������
{
	int amount = this->get_count();
	int qual;

	do
	{
		std::cout << "===============================================================================================" << std::endl;
		std::cout << "��� ������������ (0-������, 1-������, 2-������): ";
		std::cin >> qual;
		std::cin.get();
	} while (qual < 0 || qual > 2);
	std::cout << "===============================================================================================" << std::endl;

	showRecordHeader("������ �� ������������:");

	for (int i = 0; i < amount; ++i) {
		if (qual == this[i].qualification) {
			showOneRecord(this[i], i + 1);
		}
	} //for
	std::cout << "===============================================================================================" << std::endl;
} //qualificationFilter()

void Clinic::specialtyFilter()  //��������� ����� �� �������������
{
	int amount = this->get_count();
	int spec;

	do
	{
		std::cout << "===============================================================================================" << std::endl;
		std::cout << "��� ������������� (1-��������, 2-���������, 3-����������): ";
		std::cin >> spec;
		std::cin.get();
	} while (spec < 0 || spec > 2);
	std::cout << "===============================================================================================" << std::endl;

	showRecordHeader("������ �� �������������:");

	for (int i = 0; i < amount; ++i) {
		if (spec == this[i].specialty) {
			showOneRecord(this[i], i + 1);
		}
	} //for
	std::cout << "===============================================================================================" << std::endl;
} //specialtyFilter()

void showRecordHeader(const char* title) //��������������� ������� - ������ ��������� �������
{
	std::cout << std::endl << "===============================================================================================" << std::endl;
	std::cout << title;
	std::cout << std::endl << "===============================================================================================" << std::endl;
	std::cout.setf(std::ios::left);
	std::cout.width(6);
	std::cout << " �";
	std::cout.setf(std::ios::left);
	std::cout.width(FIO_WITH);
	std::cout << "��� �����";
	std::cout.setf(std::ios::left);
	std::cout.width(16);
	std::cout << "C������������";
	std::cout.setf(std::ios::left);
	std::cout.width(16);
	std::cout << "������������";
	std::cout << std::endl << "===============================================================================================" << std::endl;
}

void showOneRecord(Clinic Record, int number = 1) //��������������� ������� - ������ ����� ������
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

void showAllRecords(Clinic* P) //���������� ��� ������ � ������
{
	showRecordHeader("��� ������ � ���� �����������:");

	for (int i = 0; i < P->get_count(); ++i) {
		showOneRecord(P[i], i + 1);
	}
	std::cout << "===============================================================================================" << std::endl;
}// showAllRecords()

int menu() //���� ������� ����
{
	int select;
	std::cout << "==================================" << std::endl;
	std::cout << "����:" << std::endl;
	std::cout << "==================================" << std::endl;
	std::cout << "1 - �������� ����� ������" << std::endl;
	std::cout << "2 - �������� ��� ������" << std::endl;
	std::cout << "3 - ����� ����� �� ���" << std::endl;
	std::cout << "4 - ������ �� ������������" << std::endl;
	std::cout << "5 - ������ �� �������������" << std::endl;
	std::cout << "6 - ���������� � ���������� �������" << std::endl;
	std::cout << "0 - ����� �� ��������� � ������" << std::endl;
	std::cout << "----------------------------------" << std::endl;

	do
	{
		std::cout << "������� ����� ������ ����: ";
		std::cin >> select;
		std::cin.get();
	} while (select < 0 || select > 6);

	return select;
} // menu()