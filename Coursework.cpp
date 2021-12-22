/***---------------------------------------****
****---------Курс программирование---------****
****---------------------------------------****
* Project Type : Win32 Console Application    *
* Project Name : Coursework.CPP               *
* Language     : CPP, MSVS ver 2015 and above *
* Programmers  : Zhbanov I.A.			      *
* Modified By  : Zhbanov I.A.                 *
* Created      : 20/10/2021                   *
* Last Revision: 30/11/2021                   *
* --------------------------------------------*
* Comment      : Курсовая работа по спискам   *
****------------------------------------------*/
#include <iostream>
#include <fstream>
#include <string>
#include <iomanip>
#include <Windows.h>
#include <regex>
using namespace std;
const regex BIRTHDAYPATTERN(R"([12][0-9][0-9][0-9]\.((0[1-9])|(1[0-2]))\.((0[1-9])|([12][0-9])|(3[01])))");	//шаблон даты рождения
const regex CYRILLICWORDPATTERN(R"([\\wа-яА-Я]+)");		//шаблон для проверки слов на кириллице
const regex GROUPPATTERN(R"(М([1-9]|(1[0-2]))[ОВЗ]-[1-6][0-9][0-9](Б|(Бк)|С|М|(Мк)|А|(Ак)|(Ск))-([0-9][0-9]))");	//шаблон номер группы
ifstream fin("students.txt");		//файл для чтения
ofstream fout("studentsOUT.txt");	//файл для записи

struct student
{
	string lastname; //фамилия
	string name;	 //имя
	string midname;	 //отчество
	string group;	 //номер группы
	string gender;	 //пол
	string birthday; //дата рождения
	int mail;		 //почтовый индекс
	string city;	 //город
	string street;	 //улица
	int house;		 //номер дома
	int flat;		 //номер квартиры
	student* next;	 //указатель на следующий элемент
	student* prev;	 //указатель на предыдущий элемент
};
student* beginPtr = NULL,		//указатель на начало списка
	   * lastPtr = NULL,		//указатель на конец списка
	   * list = new student;	//указатель на текущий элемент списка
int sizeList = 0;				//размер списка

void PrintMainMenu();		//функция печати главного меню
void PrintInputMenu();		//функция печати меню печати
void PrintOutputMenu();		//функция печати меню вывода
void PrintSelectingMenu();	//функция печати меню для выборки элементов
void PrintSortingMenu();	//функция печати меню для сортировки
void PrintDeleteingMenu();	//функция печати меню для удаления
void InputList(istream& streamIn);		//функция ввода списка
bool IsInputIncorrect();				//проверка корректности ввода данных
void OutputList(ostream& streamOut);	//функция вывода списка
template<typename T> T GetValue(student* value, int fieldNumber);	//функция получения значения определенного поля структуры
void DeleteFirstStudent();		//функция удаления первого студента в списке
void DeleteLastStudent();		//функция удаления последнего студента в списке
void DeleteStudentWithIndex();	//функция уаления студента по индексу
void DeleteAllStudents();		//функция удаления всего списка целиком
void PrintRow();				//функция печати строки (информация по одному студенту)
void SwapValues(student* firstValue, student* secondValue);		//функция перестановки значений между двумя соседними элементами
template<typename T> void Selecting(T value, int fieldNumber);	//функция выборки элементов по значению
bool ComparisonOfValues(student* firstValue, int fieldNumber);	//функция сравнения значений элементов списка
void BubbleSort(int fieldNumber);	//пузырьковая сортировка

int main() 
{
	SetConsoleCP(1251);
	SetConsoleOutputCP(1251);
	int menuKey = 0;		//ключ для меню
	while (menuKey != 6)
	{
		cout << "----------------------------------------------------------------------------------------------------\n";
		PrintMainMenu();	//печатаем главное меню
		cout << "\nВыберите пункт меню: ";
		cin >> menuKey;		//вводим ключ для меню
		switch (menuKey)
		{
		case 1:
			PrintInputMenu();		//выводим меню для ввода списка
			break;
		case 2:
			PrintSelectingMenu();	//выводим меню для выборки элементов
			break;
		case 3:
			PrintOutputMenu();		//выводим меню для вывода списка
			break;
		case 4:
			PrintSortingMenu();		//выводим меню для сортировки списка
			break;
		case 5:
			PrintDeleteingMenu();	//выводим меню для удаления списка
			break;
		case 6:
			cout << "Выход...\n";
			break;
		default:
			cout << "Ошибка такого пункта меню нет!\n";
			break;
		}//switch
	}//while
	return 0;
}//main

void PrintMainMenu() 
{
	cout << "1.Ввод данных\n"
		<< "2.Выборка по заданному условию\n"
		<< "3.Вывод данных\n"
		<< "4.Сортировка по заданному столбцу\n"
		<< "5.Удаление\n"
		<< "6.Выход из программы\n";
}//PrintMenu

void PrintInputMenu() 
{
	cout << "Выберите источник ввода:\n"
		<< "1.Ввод из файла\n"
		<< "2.Ввод с консоли\n";
	cout << "\nВыберите пункт меню: ";
	int menuKey = 0;	//ключ для меню
	cin >> menuKey;		//вводим ключ для меню
	switch (menuKey)
	{
	case 1:
		InputList(fin);		//считываем список из файла
		break;
	case 2:
		InputList(cin);		//считываем список с консоли
		break;
	default:
		cout << "Ошибка такого пункта меню нет!\n";
		break;
	}//switch
}//PrintMenu

void PrintOutputMenu() 
{
	cout << "Выберите способ вывода:\n"
		<< "1.Вывод в файл\n"
		<< "2.Вывод в консоль\n";
	cout << "\nВыберите пункт меню: ";
	int menuKey = 0;	//ключ для меню
	cin >> menuKey;		//вводим ключ для меню
	switch (menuKey)
	{
	case 1:
		OutputList(fout);	//выводим список в файл
		break;
	case 2:
		OutputList(cout);	//выводим список в консоль
		break;
	default:
		cout << "Ошибка такого пункта меню нет!\n";
		break;
	}//switch
}//PrintOutputMenu

void PrintSelectingMenu() 
{
	cout << "Выберите графу, в которой необходимо произвести выборку:\n"
		<< "1.Фамилия\n"
		<< "2.Имя\n"
		<< "3.Отчество\n"
		<< "4.Номер группы\n"
		<< "5.Пол\n"
		<< "6.Дата рождения\n"
		<< "7.Почтовый индекс\n"
		<< "8.Город\n"
		<< "9.Улица\n"
		<< "10.Номер дома\n"
		<< "11.Номер квартиры\n";
	int menuKey = 0;		//ключ для меню
	string valueString;		//строковое значение для выборки
	int valueInt;			//целочисленное значение для выборки
	cout << "\nВыберите пункт меню: ";
	cin >> menuKey;			//вводим ключ для меню
	cout << "Введите искомое значение: ";
	//в зависимости от типа данных поля структуры вводим искомое значение и вызываем функцию выборки
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
		cout << "Ошибка такого пункта меню нет!\n";
		break;
	}//switch
}//PrintSelectingMenu

void PrintSortingMenu() 
{
	cout << "Выберите графу, по которой необходимо произвести сортировку:\n"
		<< "1.Фамилия\n"
		<< "2.Имя\n"
		<< "3.Отчество\n"
		<< "4.Номер группы\n"
		<< "5.Пол\n"
		<< "6.Дата рождения\n"
		<< "7.Почтовый индекс\n"
		<< "8.Город\n"
		<< "9.Улица\n"
		<< "10.Номер дома\n"
		<< "11.Номер квартиры\n";
	cout << "\nВыберите пункт меню: ";
	int menuKey = 0;		//ключ для меню
	cin >> menuKey;			//вводим ключ для меню
	BubbleSort(menuKey);	//сортируем список пузырьком
}//PrintSortingMenu

void PrintDeleteingMenu()
{
	cout << "Выберите вид удаления:\n"
		<< "1.Удалить первого студента\n"
		<< "2.Удалить последнего студента\n"
		<< "3.Удалить студента с нужным индексом\n"
		<< "4.Удалить весь список целиком\n";
	cout << "\nВыберите пункт меню: ";
	int menuKey = 0;	//ключ для меню
	cin >> menuKey;		//вводим ключ для меню
	switch (menuKey)
	{
	case 1:
		DeleteFirstStudent();	//удаляем первого студента
		break;
	case 2:
		DeleteLastStudent();	//удаляем последнего студента
		break;
	case 3:
		DeleteStudentWithIndex(); //удаляем студента с нужным индексом
		break;
	case 4:
		DeleteAllStudents();	//удаляем список целиком
		break;
	default:
		cout << "Ошибка такого пункта меню нет!\n";
		break;
	}//switch
}//PrintDeletingMenu

void InputList(istream& streamIn)
{
	cout << "Чтение спика! Если чтение происходит с консоли, поставьте в конце точку.\n";
	int stopFlag = streamIn.peek(); //считываем первый символ из потока
	int index = 0;
	if (stopFlag != EOF)	//проверка на пустой файл
	{
		do
		{
			index++;
			list = new student;
			//заполянем все поля для указателя на новый элемент
			streamIn >> list->lastname >> list->name >> list->midname >> list->group
				>> list->gender >> list->birthday >> list->mail >> list->city
				>> list->street >> list->house >> list->flat;
			bool inputIsIncorrect = IsInputIncorrect();	//переменная для проверки корректности ввода
			if (inputIsIncorrect)	//входной контроль
			{
				cout << "Проверьте строку " << index << "\n";
				stopFlag = streamIn.peek(); //считываем последний символ из потока
				delete list;
				continue;
			}//if
			lastPtr = list;
			sizeList++;				//увеличиваем размер списка
			if (beginPtr == NULL)	//если список пустой
			{
				//обнулем указатели на предыдущий и следующий элемент для указателя на последний элемент
				lastPtr->prev = NULL;
				lastPtr->next = NULL;
				beginPtr = lastPtr;	//делаем указатель на начало списка равному указателю на конец
			}
			else
			{
				list = beginPtr;	//делаем указатель на текущий элемент равный указателю на начало
				while (list)		//пока список не кончился
				{
					if (beginPtr->next == NULL)		//если мы добавляем второй элемент в список
					{
						lastPtr->next = NULL;			//обнуляем указатель на следующий элемент для указателя на конец
						lastPtr->prev = beginPtr;		//записываем указатель на начало как предыдущий для указателя на конец
						beginPtr->next = lastPtr;		//записываем указатель на конец как следующий для указателя на начало
						break;							//выходим из цикла
					}//if
					if (list->next == NULL)				//если вставляем в конец списка
					{
						lastPtr->next = NULL;			//обнуляем указатель на следующий элемент для указателя на конец
						lastPtr->prev = list;			//указатель на текущий элемент делаем предыдущим для указателя на конец
						list->next = lastPtr;			//указатель на конец делаем следующим для текущего
						break;
					}//if
					list = list->next;	//переходим к следующему элементу
				}//while
			}//if
			stopFlag = streamIn.peek(); //считываем последний символ из потока
		} while (stopFlag != EOF && stopFlag != '.'); //выполняем до тех пор, пока не встретим конец файла или точку
		streamIn.ignore();	//игнорируем последний символ в потоке
		cout << "Чтение списка завершено!\n";
	}
	else
	{
		cout << "Файл пустой или закончился!\n";
	}//if
}//InputList

bool IsInputIncorrect()
{
	bool flag = false;	//переменная для проверки корректности ввода
	//выводим все ошибки которые встретим
	if (!regex_match(list->lastname, CYRILLICWORDPATTERN))
	{
		cout << "Ошибка в фамилии!\n";
		flag = true;
	}//if
	if (!regex_match(list->name, CYRILLICWORDPATTERN))
	{
		cout << "Ошибка в имени!\n";
		flag = true;
	}//if
	if (!regex_match(list->midname, CYRILLICWORDPATTERN))
	{
		cout << "Ошибка в отчестве!\n";
		flag = true;
	}//if
	if (!regex_match(list->group, GROUPPATTERN))
	{
		cout << "Ошибка в номере группы!\n";
		flag = true;
	}//if
	if (list->gender != "М" && list->gender != "Ж")
	{
		cout << "Ошибка в поле!\n";
		flag = true;
	}//if
	if (!regex_match(list->birthday, BIRTHDAYPATTERN))
	{
		cout << "Ошибка в дне рождения!\n";
		flag = true;
	}//if
	if (!regex_match(list->city, CYRILLICWORDPATTERN))
	{
		cout << "Ошибка в названии города!\n";
		flag = true;
	}//if
	if (!regex_match(list->street, CYRILLICWORDPATTERN))
	{
		cout << "Ошибка в названии улицы!\n";
		flag = true;
	}//if
	return flag;
}//IsInputIncorrect

void OutputList(ostream& streamOut) 
{
	if (beginPtr != NULL)	//проверяем список на пустоту
	{
		cout << "Вывод списка!\n\n";
		int index = 1;		//индекс элемента списка
		list = beginPtr;	//приравниваем указатель на текущий элемент к указателю на начало списка
		while (list)		//пока в списке есть элементы
		{
			//выводим данные о студенте в выбранный поток
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
			list = list->next;	//переходим к следующему элементу
			index++;
		}//while
		cout << "\nВывод успешно завершен!\n";
	}
	else 
	{
		cout << "Список не создан!\n";
	}//if
}//OutputList

void DeleteFirstStudent()
{
	if (beginPtr == nullptr)	//проверяем список на пустоту
	{
		cout << "Список пустой!!!\n";
		return;
	}//if
	list = beginPtr->next;		//запоминаем второй элемент
	delete beginPtr;			//удаляем первый элемент
	if (list != nullptr) list->prev = nullptr;	//если второй элемент существует, то обнуляем указатель на предыдущий элемент
	beginPtr = list;			//запоминаем новый первый элемент
	cout << "Студент удален!\n";
}//DeleteFirstStudent

void DeleteLastStudent()
{
	if (beginPtr == nullptr)			//проверяем список на пустоту
	{
		cout << "Список пустой!!!\n";
		return;
	}//if
	list = lastPtr->prev;				//запоминаем указатель на предпоследний элемент
	lastPtr->prev->next = nullptr;		//для этого указателя обнуляем указатель на следующий
	delete lastPtr;					//удаляем последний элемент
	lastPtr = list;					//запоминаем новый указатель на последний элемент
	cout << "Студент удален!\n";
}//DeleteLastStudent

void DeleteStudentWithIndex() 
{
	if (beginPtr == nullptr)			//проверяем список на пустоту
	{
		cout << "Список пустой!!!\n";
		return;
	}//if
	int index = 1;		//индекс элемента списка
	int userIndex = 0;	//индекс студента, которого нужно удалить
	cout << "Введите индекс студента, которого нужно удалить: ";
	cin >> userIndex;
	list = beginPtr;
	while (list)	//пока в списке есть элементы
	{
		if (index == userIndex)		//если индексы совпали
		{
			if (list == beginPtr) DeleteFirstStudent();	//если это первый студент
			else if (list == lastPtr) DeleteLastStudent();		//если это послдений студент
			else
			{	//рвем связи для найденного указателя
				list->next->prev = list->prev;
				list->prev->next = list->next;
				delete list;	//удаляем элемент
			}//if
			break;
		}//if
		list = list->next;	//переходим к следующему элементу
		index++;	//увеличиваем индекс
	}//while
	if (index != userIndex)		//если после прохода по списку индексы разные
	{
		cout << "Такого студента не найдено!\n";
	}
	else
	{
		cout << "Студент удален!\n";
	}//if
}//DeleteStudentWithIndex

void DeleteAllStudents()
{
	if (beginPtr == nullptr)	//проверяем список на пустоту
	{
		cout << "Список пустой!!!\n";
		return;
	}
	else
	{
		while (beginPtr)	//проходимся по списку с начала
		{
			list = beginPtr;	//запоминаем студента
			beginPtr = beginPtr->next;	//передвигаем начало на одного студента вперед
			delete list;	//удаляем студента
		}//while
		//обнуляем указатели на начало и конец
		beginPtr = nullptr;
		lastPtr = nullptr;
		cout << "Все студенты удалены!\n";
	}//if
}//DeleteAllStudents

template<> string GetValue(student* value, int fieldNumber)
{
	//в зависимости от выбраного поля возвращаем его значение 
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
	//в зависимости от выбраного поля возвращаем его значение
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
	if (firstValue->prev == nullptr)	//если первый элемент из двух начальный
	{
		beginPtr = secondValue;			//запоминаем новое начало списка
		secondValue->prev = nullptr;	//обнуляем указатель на предыдущий элемент
	}
	else
	{
		firstValue->prev->next = secondValue;	//ставим второй элемент перед первым
		secondValue->prev = firstValue->prev;	//указываем предыдущий элемент для первого указателя как предыдущий для второго
	}//if
	if (secondValue->next == nullptr)	//если второй элемент из двух последний
	{
		lastPtr = firstValue;		//запоминаем новый конец списка
		firstValue->next = nullptr;	//обнуляем указатель на следующий элемент 
	}
	else
	{
		secondValue->next->prev = firstValue;	//ставим первый элемент перед вторым
		firstValue->next = secondValue->next;	//указываем следующий элемент для второго указателя как следующий для первого
	}//if
	firstValue->prev = secondValue;	//указываем второй элемент как предыдущий для первого
	secondValue->next = firstValue;	//указываем первый элемент как следующий для второго
}//SwapValues

template<typename T>
void Selecting(T value, int fieldNumber)
{
	if (beginPtr == nullptr)	//проверяем список на пустоту
	{
		cout << "Список пустой!!!\n";
		return;
	}//if
	bool flag = false;	//переменная для проверки существования студента
	cout << "Поиск студентов:\n";
	list = beginPtr;	
	while (list)	//начинаем обход списка с начала
	{
		if (value == GetValue<T>(list, fieldNumber))	//если введенное значение и значение поля совпали
		{
			PrintRow();		//печатаем строку
			flag = true;	
		}//if
		list = list->next;	//переходим к следующему элементу
	}//while
	if (!flag) cout << "Такие студенты не найдены!\n";
}//Selecting

bool ComparisonOfValues(student* firstValue, int fieldNumber)
{
	//в зависимсти от выбранного поля запрашиваем значения полей двух соседних элементов и сравниваем их
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

//сортировка пузырьком ввиду более легкой реализации, как самой сортировки, так и смены указателей местами
void BubbleSort(int fieldNumber)
{
	if (beginPtr == nullptr)	//проверяем список на пустоту
	{
		cout << "Список пустой!!!\n";
		return;
	}
	else
	{
		cout << "Сортировка базы данных!\n";
		for (int i = 0; i < sizeList; i++)
		{
			list = beginPtr;
			for (int j = 0; j < sizeList - 1; j++)
			{
				if (ComparisonOfValues(list, fieldNumber))	//сравниваем значения
				{
					SwapValues(list, list->next);	//меняем указатели местами
					list = list->prev;	//возвращаемся на один элемент назад
				}//if
				list = list->next;	//переходим к следующему элементу
			}//for
		}//for
		cout << "База данных отсортирована!\n";
	}//if
}//BubbleSort
/*-----------End of file Coursework.CPP--------------*/