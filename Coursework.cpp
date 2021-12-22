/***---------------------------------------****
****---------���� ����������������---------****
****---------------------------------------****
* Project Type : Win32 Console Application    *
* Project Name : Coursework.CPP               *
* Language     : CPP, MSVS ver 2015 and above *
* Programmers  : Zhbanov I.A.			      *
* Modified By  : Zhbanov I.A.                 *
* Created      : 20/10/2021                   *
* Last Revision: 30/11/2021                   *
* --------------------------------------------*
* Comment      : �������� ������ �� �������   *
****------------------------------------------*/
#include <iostream>
#include <fstream>
#include <string>
#include <iomanip>
#include <Windows.h>
#include <regex>
using namespace std;
const regex BIRTHDAYPATTERN(R"([12][0-9][0-9][0-9]\.((0[1-9])|(1[0-2]))\.((0[1-9])|([12][0-9])|(3[01])))");	//������ ���� ��������
const regex CYRILLICWORDPATTERN(R"([\\w�-��-�]+)");		//������ ��� �������� ���� �� ���������
const regex GROUPPATTERN(R"(�([1-9]|(1[0-2]))[���]-[1-6][0-9][0-9](�|(��)|�|�|(��)|�|(��)|(��))-([0-9][0-9]))");	//������ ����� ������
ifstream fin("students.txt");		//���� ��� ������
ofstream fout("studentsOUT.txt");	//���� ��� ������

struct student
{
	string lastname; //�������
	string name;	 //���
	string midname;	 //��������
	string group;	 //����� ������
	string gender;	 //���
	string birthday; //���� ��������
	int mail;		 //�������� ������
	string city;	 //�����
	string street;	 //�����
	int house;		 //����� ����
	int flat;		 //����� ��������
	student* next;	 //��������� �� ��������� �������
	student* prev;	 //��������� �� ���������� �������
};
student* beginPtr = NULL,		//��������� �� ������ ������
	   * lastPtr = NULL,		//��������� �� ����� ������
	   * list = new student;	//��������� �� ������� ������� ������
int sizeList = 0;				//������ ������

void PrintMainMenu();		//������� ������ �������� ����
void PrintInputMenu();		//������� ������ ���� ������
void PrintOutputMenu();		//������� ������ ���� ������
void PrintSelectingMenu();	//������� ������ ���� ��� ������� ���������
void PrintSortingMenu();	//������� ������ ���� ��� ����������
void PrintDeleteingMenu();	//������� ������ ���� ��� ��������
void InputList(istream& streamIn);		//������� ����� ������
bool IsInputIncorrect();				//�������� ������������ ����� ������
void OutputList(ostream& streamOut);	//������� ������ ������
template<typename T> T GetValue(student* value, int fieldNumber);	//������� ��������� �������� ������������� ���� ���������
void DeleteFirstStudent();		//������� �������� ������� �������� � ������
void DeleteLastStudent();		//������� �������� ���������� �������� � ������
void DeleteStudentWithIndex();	//������� ������� �������� �� �������
void DeleteAllStudents();		//������� �������� ����� ������ �������
void PrintRow();				//������� ������ ������ (���������� �� ������ ��������)
void SwapValues(student* firstValue, student* secondValue);		//������� ������������ �������� ����� ����� ��������� ����������
template<typename T> void Selecting(T value, int fieldNumber);	//������� ������� ��������� �� ��������
bool ComparisonOfValues(student* firstValue, int fieldNumber);	//������� ��������� �������� ��������� ������
void BubbleSort(int fieldNumber);	//����������� ����������

int main() 
{
	SetConsoleCP(1251);
	SetConsoleOutputCP(1251);
	int menuKey = 0;		//���� ��� ����
	while (menuKey != 6)
	{
		cout << "----------------------------------------------------------------------------------------------------\n";
		PrintMainMenu();	//�������� ������� ����
		cout << "\n�������� ����� ����: ";
		cin >> menuKey;		//������ ���� ��� ����
		switch (menuKey)
		{
		case 1:
			PrintInputMenu();		//������� ���� ��� ����� ������
			break;
		case 2:
			PrintSelectingMenu();	//������� ���� ��� ������� ���������
			break;
		case 3:
			PrintOutputMenu();		//������� ���� ��� ������ ������
			break;
		case 4:
			PrintSortingMenu();		//������� ���� ��� ���������� ������
			break;
		case 5:
			PrintDeleteingMenu();	//������� ���� ��� �������� ������
			break;
		case 6:
			cout << "�����...\n";
			break;
		default:
			cout << "������ ������ ������ ���� ���!\n";
			break;
		}//switch
	}//while
	return 0;
}//main

void PrintMainMenu() 
{
	cout << "1.���� ������\n"
		<< "2.������� �� ��������� �������\n"
		<< "3.����� ������\n"
		<< "4.���������� �� ��������� �������\n"
		<< "5.��������\n"
		<< "6.����� �� ���������\n";
}//PrintMenu

void PrintInputMenu() 
{
	cout << "�������� �������� �����:\n"
		<< "1.���� �� �����\n"
		<< "2.���� � �������\n";
	cout << "\n�������� ����� ����: ";
	int menuKey = 0;	//���� ��� ����
	cin >> menuKey;		//������ ���� ��� ����
	switch (menuKey)
	{
	case 1:
		InputList(fin);		//��������� ������ �� �����
		break;
	case 2:
		InputList(cin);		//��������� ������ � �������
		break;
	default:
		cout << "������ ������ ������ ���� ���!\n";
		break;
	}//switch
}//PrintMenu

void PrintOutputMenu() 
{
	cout << "�������� ������ ������:\n"
		<< "1.����� � ����\n"
		<< "2.����� � �������\n";
	cout << "\n�������� ����� ����: ";
	int menuKey = 0;	//���� ��� ����
	cin >> menuKey;		//������ ���� ��� ����
	switch (menuKey)
	{
	case 1:
		OutputList(fout);	//������� ������ � ����
		break;
	case 2:
		OutputList(cout);	//������� ������ � �������
		break;
	default:
		cout << "������ ������ ������ ���� ���!\n";
		break;
	}//switch
}//PrintOutputMenu

void PrintSelectingMenu() 
{
	cout << "�������� �����, � ������� ���������� ���������� �������:\n"
		<< "1.�������\n"
		<< "2.���\n"
		<< "3.��������\n"
		<< "4.����� ������\n"
		<< "5.���\n"
		<< "6.���� ��������\n"
		<< "7.�������� ������\n"
		<< "8.�����\n"
		<< "9.�����\n"
		<< "10.����� ����\n"
		<< "11.����� ��������\n";
	int menuKey = 0;		//���� ��� ����
	string valueString;		//��������� �������� ��� �������
	int valueInt;			//������������� �������� ��� �������
	cout << "\n�������� ����� ����: ";
	cin >> menuKey;			//������ ���� ��� ����
	cout << "������� ������� ��������: ";
	//� ����������� �� ���� ������ ���� ��������� ������ ������� �������� � �������� ������� �������
	switch (menuKey)
	{
	case 1:
		cin >> valueString;
		Selecting(valueString, menuKey);
		break;
	case 2:
		cin >> valueString;
		Selecting(valueString, menuKey);
		break;
	case 3:
		cin >> valueString;
		Selecting(valueString, menuKey);
		break;
	case 4:
		cin >> valueString;
		Selecting(valueString, menuKey);
		break;
	case 5:
		cin >> valueString;
		Selecting(valueString, menuKey);
		break;
	case 6:
		cin >> valueString;
		Selecting(valueString, menuKey);
		break;
	case 7:
		cin >> valueInt;
		Selecting(valueInt, menuKey);
		break;
	case 8:
		cin >> valueString;
		Selecting(valueString, menuKey);
		break;
	case 9:
		cin >> valueString;
		Selecting(valueString, menuKey);
		break;
	case 10:
		cin >> valueInt;
		Selecting(valueInt, menuKey);
		cin >> valueInt;
		break;
	case 11:
		cin >> valueInt;
		Selecting(valueInt, menuKey);
		break;
	default:
		cout << "������ ������ ������ ���� ���!\n";
		break;
	}//switch
}//PrintSelectingMenu

void PrintSortingMenu() 
{
	cout << "�������� �����, �� ������� ���������� ���������� ����������:\n"
		<< "1.�������\n"
		<< "2.���\n"
		<< "3.��������\n"
		<< "4.����� ������\n"
		<< "5.���\n"
		<< "6.���� ��������\n"
		<< "7.�������� ������\n"
		<< "8.�����\n"
		<< "9.�����\n"
		<< "10.����� ����\n"
		<< "11.����� ��������\n";
	cout << "\n�������� ����� ����: ";
	int menuKey = 0;		//���� ��� ����
	cin >> menuKey;			//������ ���� ��� ����
	BubbleSort(menuKey);	//��������� ������ ���������
}//PrintSortingMenu

void PrintDeleteingMenu()
{
	cout << "�������� ��� ��������:\n"
		<< "1.������� ������� ��������\n"
		<< "2.������� ���������� ��������\n"
		<< "3.������� �������� � ������ ��������\n"
		<< "4.������� ���� ������ �������\n";
	cout << "\n�������� ����� ����: ";
	int menuKey = 0;	//���� ��� ����
	cin >> menuKey;		//������ ���� ��� ����
	switch (menuKey)
	{
	case 1:
		DeleteFirstStudent();	//������� ������� ��������
		break;
	case 2:
		DeleteLastStudent();	//������� ���������� ��������
		break;
	case 3:
		DeleteStudentWithIndex(); //������� �������� � ������ ��������
		break;
	case 4:
		DeleteAllStudents();	//������� ������ �������
		break;
	default:
		cout << "������ ������ ������ ���� ���!\n";
		break;
	}//switch
}//PrintDeletingMenu

void InputList(istream& streamIn)
{
	cout << "������ �����! ���� ������ ���������� � �������, ��������� � ����� �����.\n";
	int stopFlag = streamIn.peek(); //��������� ������ ������ �� ������
	int index = 0;
	if (stopFlag != EOF)	//�������� �� ������ ����
	{
		do
		{
			index++;
			list = new student;
			//��������� ��� ���� ��� ��������� �� ����� �������
			streamIn >> list->lastname >> list->name >> list->midname >> list->group
				>> list->gender >> list->birthday >> list->mail >> list->city
				>> list->street >> list->house >> list->flat;
			bool inputIsIncorrect = IsInputIncorrect();	//���������� ��� �������� ������������ �����
			if (inputIsIncorrect)	//������� ��������
			{
				cout << "��������� ������ " << index << "\n";
				stopFlag = streamIn.peek(); //��������� ��������� ������ �� ������
				delete list;
				continue;
			}//if
			lastPtr = list;
			sizeList++;				//����������� ������ ������
			if (beginPtr == NULL)	//���� ������ ������
			{
				//������� ��������� �� ���������� � ��������� ������� ��� ��������� �� ��������� �������
				lastPtr->prev = NULL;
				lastPtr->next = NULL;
				beginPtr = lastPtr;	//������ ��������� �� ������ ������ ������� ��������� �� �����
			}
			else
			{
				list = beginPtr;	//������ ��������� �� ������� ������� ������ ��������� �� ������
				while (list)		//���� ������ �� ��������
				{
					if (beginPtr->next == NULL)		//���� �� ��������� ������ ������� � ������
					{
						lastPtr->next = NULL;			//�������� ��������� �� ��������� ������� ��� ��������� �� �����
						lastPtr->prev = beginPtr;		//���������� ��������� �� ������ ��� ���������� ��� ��������� �� �����
						beginPtr->next = lastPtr;		//���������� ��������� �� ����� ��� ��������� ��� ��������� �� ������
						break;							//������� �� �����
					}//if
					if (list->next == NULL)				//���� ��������� � ����� ������
					{
						lastPtr->next = NULL;			//�������� ��������� �� ��������� ������� ��� ��������� �� �����
						lastPtr->prev = list;			//��������� �� ������� ������� ������ ���������� ��� ��������� �� �����
						list->next = lastPtr;			//��������� �� ����� ������ ��������� ��� ��������
						break;
					}//if
					list = list->next;	//��������� � ���������� ��������
				}//while
			}//if
			stopFlag = streamIn.peek(); //��������� ��������� ������ �� ������
		} while (stopFlag != EOF && stopFlag != '.'); //��������� �� ��� ���, ���� �� �������� ����� ����� ��� �����
		streamIn.ignore();	//���������� ��������� ������ � ������
		cout << "������ ������ ���������!\n";
	}
	else
	{
		cout << "���� ������ ��� ����������!\n";
	}//if
}//InputList

bool IsInputIncorrect()
{
	bool flag = false;	//���������� ��� �������� ������������ �����
	//������� ��� ������ ������� ��������
	if (!regex_match(list->lastname, CYRILLICWORDPATTERN))
	{
		cout << "������ � �������!\n";
		flag = true;
	}//if
	if (!regex_match(list->name, CYRILLICWORDPATTERN))
	{
		cout << "������ � �����!\n";
		flag = true;
	}//if
	if (!regex_match(list->midname, CYRILLICWORDPATTERN))
	{
		cout << "������ � ��������!\n";
		flag = true;
	}//if
	if (!regex_match(list->group, GROUPPATTERN))
	{
		cout << "������ � ������ ������!\n";
		flag = true;
	}//if
	if (list->gender != "�" && list->gender != "�")
	{
		cout << "������ � ����!\n";
		flag = true;
	}//if
	if (!regex_match(list->birthday, BIRTHDAYPATTERN))
	{
		cout << "������ � ��� ��������!\n";
		flag = true;
	}//if
	if (!regex_match(list->city, CYRILLICWORDPATTERN))
	{
		cout << "������ � �������� ������!\n";
		flag = true;
	}//if
	if (!regex_match(list->street, CYRILLICWORDPATTERN))
	{
		cout << "������ � �������� �����!\n";
		flag = true;
	}//if
	return flag;
}//IsInputIncorrect

void OutputList(ostream& streamOut) 
{
	if (beginPtr != NULL)	//��������� ������ �� �������
	{
		cout << "����� ������!\n\n";
		int index = 1;		//������ �������� ������
		list = beginPtr;	//������������ ��������� �� ������� ������� � ��������� �� ������ ������
		while (list)		//���� � ������ ���� ��������
		{
			//������� ������ � �������� � ��������� �����
			streamOut << index << ")"
				<< setw(15) << list->lastname
				<< setw(15) << list->name
				<< setw(20) << list->midname
				<< setw(20) << list->group
				<< setw(3) << list->gender
				<< setw(12) << list->birthday
				<< setw(7) << list->mail
				<< setw(20) << list->city
				<< setw(20) << list->street
				<< setw(4) << list->house
				<< setw(4) << list->flat
				<< endl;
			list = list->next;	//��������� � ���������� ��������
			index++;
		}//while
		cout << "\n����� ������� ��������!\n";
	}
	else 
	{
		cout << "������ �� ������!\n";
	}//if
}//OutputList

void DeleteFirstStudent()
{
	if (beginPtr == nullptr)	//��������� ������ �� �������
	{
		cout << "������ ������!!!\n";
		return;
	}//if
	list = beginPtr->next;		//���������� ������ �������
	delete beginPtr;			//������� ������ �������
	if (list != nullptr) list->prev = nullptr;	//���� ������ ������� ����������, �� �������� ��������� �� ���������� �������
	beginPtr = list;			//���������� ����� ������ �������
	cout << "������� ������!\n";
}//DeleteFirstStudent

void DeleteLastStudent()
{
	if (beginPtr == nullptr)			//��������� ������ �� �������
	{
		cout << "������ ������!!!\n";
		return;
	}//if
	list = lastPtr->prev;				//���������� ��������� �� ������������� �������
	lastPtr->prev->next = nullptr;		//��� ����� ��������� �������� ��������� �� ���������
	delete lastPtr;					//������� ��������� �������
	lastPtr = list;					//���������� ����� ��������� �� ��������� �������
	cout << "������� ������!\n";
}//DeleteLastStudent

void DeleteStudentWithIndex() 
{
	if (beginPtr == nullptr)			//��������� ������ �� �������
	{
		cout << "������ ������!!!\n";
		return;
	}//if
	int index = 1;		//������ �������� ������
	int userIndex = 0;	//������ ��������, �������� ����� �������
	cout << "������� ������ ��������, �������� ����� �������: ";
	cin >> userIndex;
	list = beginPtr;
	while (list)	//���� � ������ ���� ��������
	{
		if (index == userIndex)		//���� ������� �������
		{
			if (list == beginPtr) DeleteFirstStudent();	//���� ��� ������ �������
			else if (list == lastPtr) DeleteLastStudent();		//���� ��� ��������� �������
			else
			{	//���� ����� ��� ���������� ���������
				list->next->prev = list->prev;
				list->prev->next = list->next;
				delete list;	//������� �������
			}//if
			break;
		}//if
		list = list->next;	//��������� � ���������� ��������
		index++;	//����������� ������
	}//while
	if (index != userIndex)		//���� ����� ������� �� ������ ������� ������
	{
		cout << "������ �������� �� �������!\n";
	}
	else
	{
		cout << "������� ������!\n";
	}//if
}//DeleteStudentWithIndex

void DeleteAllStudents()
{
	if (beginPtr == nullptr)	//��������� ������ �� �������
	{
		cout << "������ ������!!!\n";
		return;
	}
	else
	{
		while (beginPtr)	//���������� �� ������ � ������
		{
			list = beginPtr;	//���������� ��������
			beginPtr = beginPtr->next;	//����������� ������ �� ������ �������� ������
			delete list;	//������� ��������
		}//while
		//�������� ��������� �� ������ � �����
		beginPtr = nullptr;
		lastPtr = nullptr;
		cout << "��� �������� �������!\n";
	}//if
}//DeleteAllStudents

template<> string GetValue(student* value, int fieldNumber)
{
	//� ����������� �� ��������� ���� ���������� ��� �������� 
	switch (fieldNumber)
	{
	case 1:
		return value->lastname;
		break;
	case 2:
		return value->name;
		break;
	case 3:
		return value->midname;
		break;
	case 4:
		return value->group;
		break;
	case 5:
		return value->gender;
		break;
	case 6:
		return value->birthday;
		break;
	case 8:
		return value->city;
		break;
	case 9:
		return value->street;
		break;
	default:
		break;
	}//switch
}//<string>GetValue

template<> int GetValue(student* value, int fieldNumber)
{
	//� ����������� �� ��������� ���� ���������� ��� ��������
	switch (fieldNumber)
	{
	case 7:
		return value->mail;
		break;
	case 10:
		return value->house;
		break;
	case 11:
		return value->flat;
		break;
	}//switch
}//<int>GetValue

void PrintRow()
{
	cout << setw(15) << list->lastname
		<< setw(15) << list->name
		<< setw(20) << list->midname
		<< setw(20) << list->group
		<< setw(3) << list->gender
		<< setw(12) << list->birthday
		<< setw(7) << list->mail
		<< setw(20) << list->city
		<< setw(20) << list->street
		<< setw(4) << list->house
		<< setw(4) << list->flat
		<< endl;
}//PrintRow

void SwapValues(student* firstValue, student* secondValue)
{
	if (firstValue->prev == nullptr)	//���� ������ ������� �� ���� ���������
	{
		beginPtr = secondValue;			//���������� ����� ������ ������
		secondValue->prev = nullptr;	//�������� ��������� �� ���������� �������
	}
	else
	{
		firstValue->prev->next = secondValue;	//������ ������ ������� ����� ������
		secondValue->prev = firstValue->prev;	//��������� ���������� ������� ��� ������� ��������� ��� ���������� ��� �������
	}//if
	if (secondValue->next == nullptr)	//���� ������ ������� �� ���� ���������
	{
		lastPtr = firstValue;		//���������� ����� ����� ������
		firstValue->next = nullptr;	//�������� ��������� �� ��������� ������� 
	}
	else
	{
		secondValue->next->prev = firstValue;	//������ ������ ������� ����� ������
		firstValue->next = secondValue->next;	//��������� ��������� ������� ��� ������� ��������� ��� ��������� ��� �������
	}//if
	firstValue->prev = secondValue;	//��������� ������ ������� ��� ���������� ��� �������
	secondValue->next = firstValue;	//��������� ������ ������� ��� ��������� ��� �������
}//SwapValues

template<typename T>
void Selecting(T value, int fieldNumber)
{
	if (beginPtr == nullptr)	//��������� ������ �� �������
	{
		cout << "������ ������!!!\n";
		return;
	}//if
	bool flag = false;	//���������� ��� �������� ������������� ��������
	cout << "����� ���������:\n";
	list = beginPtr;	
	while (list)	//�������� ����� ������ � ������
	{
		if (value == GetValue<T>(list, fieldNumber))	//���� ��������� �������� � �������� ���� �������
		{
			PrintRow();		//�������� ������
			flag = true;	
		}//if
		list = list->next;	//��������� � ���������� ��������
	}//while
	if (!flag) cout << "����� �������� �� �������!\n";
}//Selecting

bool ComparisonOfValues(student* firstValue, int fieldNumber)
{
	//� ���������� �� ���������� ���� ����������� �������� ����� ���� �������� ��������� � ���������� ��
	switch (fieldNumber)
	{
	case 1:
		return GetValue<string>(firstValue, fieldNumber) > GetValue<string>(firstValue->next, fieldNumber);
		break;
	case 2:
		return GetValue<string>(firstValue, fieldNumber) > GetValue<string>(firstValue->next, fieldNumber);
		break;
	case 3:
		return GetValue<string>(firstValue, fieldNumber) > GetValue<string>(firstValue->next, fieldNumber);
		break;
	case 4:
		return GetValue<string>(firstValue, fieldNumber) > GetValue<string>(firstValue->next, fieldNumber);
		break;
	case 5:
		return GetValue<string>(firstValue, fieldNumber) > GetValue<string>(firstValue->next, fieldNumber);
		break;
	case 6:
		return GetValue<string>(firstValue, fieldNumber) > GetValue<string>(firstValue->next, fieldNumber);
		break;
	case 7:
		return GetValue<int>(firstValue, fieldNumber) > GetValue<int>(firstValue->next, fieldNumber);
		break;
	case 8:
		return GetValue<string>(firstValue, fieldNumber) > GetValue<string>(firstValue->next, fieldNumber);
		break;
	case 9:
		return GetValue<string>(firstValue, fieldNumber) > GetValue<string>(firstValue->next, fieldNumber);
		break;
	case 10:
		return GetValue<int>(firstValue, fieldNumber) > GetValue<int>(firstValue->next, fieldNumber);
		break;
	case 11:
		return GetValue<int>(firstValue, fieldNumber) > GetValue<int>(firstValue->next, fieldNumber);
		break;
	}//switch
}//ComparisonOfValues

//���������� ��������� ����� ����� ������ ����������, ��� ����� ����������, ��� � ����� ���������� �������
void BubbleSort(int fieldNumber)
{
	if (beginPtr == nullptr)	//��������� ������ �� �������
	{
		cout << "������ ������!!!\n";
		return;
	}
	else
	{
		cout << "���������� ���� ������!\n";
		for (int i = 0; i < sizeList; i++)
		{
			list = beginPtr;
			for (int j = 0; j < sizeList - 1; j++)
			{
				if (ComparisonOfValues(list, fieldNumber))	//���������� ��������
				{
					SwapValues(list, list->next);	//������ ��������� �������
					list = list->prev;	//������������ �� ���� ������� �����
				}//if
				list = list->next;	//��������� � ���������� ��������
			}//for
		}//for
		cout << "���� ������ �������������!\n";
	}//if
}//BubbleSort
/*-----------End of file Coursework.CPP--------------*/