#include "pch.h" 
//#include "stdafx.h"
#include <stdio.h> //подключение основных библиотек
#include <stdlib.h>
#include <conio.h>
#include <string.h>
#include <malloc.h>
#include <clocale>

using namespace std;
using namespace System;
using namespace System::IO;
							//задаём значения кнопок
#define ENTER 13 
#define ESC 27
#define UP 72 //38
#define DOWN 80 //40
#define	HOME 37 //клавиша <-
#define END 39 //клавиша ->
//создание пунктов меню
char dan[8][55] = {//при добавлении кейса смена на 21,104,114 строках
			 "Какая ветка наиболее загружена?			",
			 "Ветка с наибольшим количеством станций?		",
			 "Какая ветка была открыта раньше всех?		",
			 "Алфавитный список веток московского метро		",
			 "Диаграмма. Процентное соотношение потоков пассажиров ",
			 "Ветки с одинаковой загруженностью и датой открытия   ",
			 "Обратный алфавитный список веток московского метро	",
			 "Выход						",
};
char BlankLine[] = "                                                       ";
//  BlankLine сотоит из 56 пробелов 
//  или так: char BlankLine[80]; memset (BlankLine,’ ‘,56); BlankLine[56]=0;
int NC;

struct z {  
	char name[30]; //название ветки метро
	char tsvet[20]; //цвет ветки
	long potok; //поток(загруженность) пассажиров на ветки - тыс.чел./сутки 
	char data[11]; //дата запуска ветки
	long stanc; //количество станций на ветке
}; 

struct sp {  //шаблон структуры
	char nazv[30];
	long potok;
	struct sp* sled;
	struct sp* pred;
} *spisok;

//объявление функций
int menu(int);
void maxim(struct z*);
void ctans(struct z*);
void first(struct z*);
void text_data(char*, char*);
void alfalist(struct z*);
void vstavka(struct z*, char*);
void diagram(struct z*);
void hard(struct z*);
void obrat(struct z*);

int main(array<System::String^>^ args)  
{
	int i, n;
	FILE* in;
	struct z* vetki; //объявление структурной переменной
	setlocale(LC_CTYPE, "Russian");  //поключение русского языка
	Console::CursorVisible::set(false);
	Console::WindowHeight = 30;
	Console::WindowWidth = 100;
	Console::BufferHeight = Console::WindowHeight;
	Console::BufferWidth = Console::WindowWidth;
	if ((in = fopen("Metro.dat", "r")) == NULL)  //открытие файла
	{
		printf("\nФайл Metro.dat не открыт ! (Проверьте добавлен ли файл Metro.dat в Debug)");
		getch(); exit(1);
	}

	fscanf(in, "%d", &NC);
	vetki = (struct z*)malloc(NC * sizeof(struct z));

	for (i = 0; i < NC; i++)
		fscanf(in, "%s%s%ld%s%ld", vetki[i].name,
			vetki[i].tsvet, &vetki[i].potok,
			vetki[i].data, &vetki[i].stanc);

	/*for (i = 0; i < NC; i++)
		printf("\n%-30s %-25s %-10ld %-20s %ld", vetki[i].name, //расстояние между столбиками в dat
			vetki[i].tsvet, vetki[i].potok,
			vetki[i].data,vetki[i].stanc);
	getch(); */
//Отрисовка меню: задание области для вопросов и закраска выбранного вопроса
	while (1)
	{
		Console::ForegroundColor = ConsoleColor::White;
		Console::BackgroundColor = ConsoleColor::DarkGray;
		Console::Clear();
		Console::ForegroundColor = ConsoleColor::DarkGray;
		Console::BackgroundColor = ConsoleColor::White;
		Console::CursorLeft = 10;
		Console::CursorTop = 4; //уровень закраски первой (пустой) строки в меню
		printf(BlankLine); 

		for (i = 0; i < 8; i++)
		{
			Console::CursorLeft = 10;
			Console::CursorTop = i + 5;
			printf(" %s ", dan[i]);
		}
		Console::CursorLeft = 10;
		Console::CursorTop = 13;  //уровень закраски последней (пустой) строки в меню
		printf(BlankLine);
//Выбор номера меню
		n = menu(8);
		switch (n)
		{
			case 1: maxim(vetki); break;
			case 2: ctans(vetki); break;
			case 3: first(vetki); break;
			case 4: alfalist(vetki); break;
			case 5: diagram(vetki); break;
			case 6: hard(vetki); break;
			case 7: obrat(vetki); break;
			case 8: exit(0);
		}
	}  // конец while(1)... 
	return 0;
} // конец main()   после переноса второго кейса на 2 место в вопросах курсор идёт с начала

//Программирование меню
int menu(int n)
{
	int y1 = 0, y2 = n - 1;
	char c = 1;
	while (c != ESC)
	{
		switch (c) 
		{//задание действий при нажатии клавиш
			case DOWN: y2 = y1; y1++; break; //переход на следующую строку 
			case UP: y2 = y1; y1--; break; //переход на предыдущую строку
			case END: y2 = y1; y1 = 5; break; //переход на последнюю строку ->
			case HOME: y2 = y1; y1 = 0; break; //переход на первую строку <-
			case ENTER: return y1 + 1; //выбор
		}
		if (y1 > n - 1) //исключения для перехода вниз
		{ 
			y2 = n - 1; 
			y1 = 0; 
		}
		if (y1 < 0) //исключения для перехода вверх
		{ 
			y2 = 0;
			y1 = n - 1; 
		} //задание цветов на выбранном вопросе
		Console::ForegroundColor = ConsoleColor::Gray;
		Console::BackgroundColor = ConsoleColor::Green;
		Console::CursorLeft = 11;
		Console::CursorTop = y1 + 5;
		printf("%s", dan[y1]);
		Console::ForegroundColor = ConsoleColor::DarkGray; //возврат на исходные цвета
		Console::BackgroundColor = ConsoleColor::White;
		Console::CursorLeft = 11;
		Console::CursorTop = y2 + 5;
		printf("%s", dan[y2]);
		c = getch();
	} // конец while(c!=ESC)...
	exit(0);
}
 
//Поиск максимального размера вклада

void maxim(struct z* vetka) //рем: наиболее подробно опишу первую функцию, остальные похожи
{
	int i = 0; struct z best; //объявление структуры best на основе шаблона z, и переменно i для цикла
	strcpy(best.name, vetka[0].name); //задаём нулевое значение эелемента для переменной name, структуры best
	best.potok = vetka[0].potok; //также задаём для переменной potok. Мы можем обращаться к best без открытия доступа т.к. это Struct => Public
	for (i = 1; i < NC; i++) //запускаем цикл для поиска max по потоку, с проходом всех исходных элементов(i++) 
		if (vetka[i].potok > best.potok) //сравниваем значения переменной potok у разных исходных элементов
		{
			strcpy(best.name, vetka[i].name); //копируем название...
			best.potok = vetka[i].potok; //...и поток найденного элемента в обнулёные ранее переменные структуры
		}
	Console::ForegroundColor = ConsoleColor::Yellow; //закраска окна ответа
	Console::BackgroundColor = ConsoleColor::Black;
	Console::CursorLeft = 10; //указание точки начала закраски и вывода части ответа
	Console::CursorTop = 15;
	printf("Максимальный поток пассажиров %ld тыс.чел/сутки", best.potok); //вывод ответа из переменной potok
	Console::CursorLeft = 10; //указание точки начала закраски и вывода второй части ответа
	Console::CursorTop = 16;
	printf("На ветке %s", best.name); //вывод ответа из переменной name
	_getch();
}
//Указание количества станций на ветке
void ctans(struct z* vetka)
{
	int i = 0; struct z best;
	strcpy(best.name, vetka[0].name);
	best.stanc = vetka[0].stanc;
	for (i = 1; i < NC; i++)
		if (vetka[i].stanc > best.stanc)
		{
			strcpy(best.name, vetka[i].name);
			best.stanc = vetka[i].stanc;
		}
	Console::ForegroundColor = ConsoleColor::Yellow;
	Console::BackgroundColor = ConsoleColor::Black;
	Console::CursorLeft = 10;
	Console::CursorTop = 15;
	printf("Максимальное количество станций %ld", best.stanc);
	Console::CursorLeft = 10;
	Console::CursorTop = 16;
	printf("На ветке %s", best.name);
	_getch();
}

//Преобразование даты 2011-02-23 (sd) => 23 февраля 2011 (s)

void text_data(char* s, char* sd)
{
	char s0[3], month[12][9] = { //создание массива и месяцами
		"января","февраля","марта","апреля","мая","июня",
		"июля","августа","сентября","октября","ноября","декабря" };
	strcpy(s, sd + 8); //копирует месяц из массива в s
	strcat(s, " "); //присоединяем пробел
	strncpy(s0, sd + 5, 2); //вносим число в перем s0
	s0[2] = 0; //вносим 0 в завершение
	strcat(s, month[atoi(s0) - 1]); //присоединяем число и месяц
	strcat(s, " ");//присоединяем пробел после
	strncat(s, sd, 4); //присоединяем ко всему год
	return;
}

//Поиск самой ранней даты вклада
void first(struct z* vetka)
{
	int i;
	char s[17]; //объявление локальной переменной типа char и выделение памяти(17) для неё
	struct z* best = vetka;
	for (i = 1; i < NC; i++)
		if (strcmp(vetka[i].data, best->data) < 0)
			best = &vetka[i];
	text_data(s, best->data); //используем раннее описанную функцию text_data
	Console::ForegroundColor = ConsoleColor::Yellow;
	Console::BackgroundColor = ConsoleColor::Black;
	Console::CursorLeft = 10;
	Console::CursorTop = 15;
	printf("Самый \"старая\" ветка %s (%s)",
		best->name,best->tsvet); //вывод двух переменных в ответ
	//Console::CursorLeft = 10;
	//Console::CursorTop = 16;
	//printf("Ветка %s", best->name);
	Console::CursorLeft = 10;
	Console::CursorTop = 16;
	printf("Открыта  %s года", s);
	_getch();
}

//Составление алфавитного списка веток
void alfalist(struct z* vetka)
{
	int i;
	struct sp* nt; //объявление структуры nt на основе второго шаблона sp
	Console::ForegroundColor = ConsoleColor::Black;
	Console::BackgroundColor = ConsoleColor::Gray;
	Console::Clear();
	if (!spisok)
		for (i = 0; i < NC; i++)
			vstavka(vetka, vetka[i].name); //использование созданной функции

	Console::Clear();
		printf("\n    Алфавитный список веток московского метро");
		printf("\n    =========================================\n");
	for (nt = spisok; nt != 0; nt = nt->sled)
		printf("\n %-25s	Пассажиропоток: %5ld тыс.чел/сут" , nt->nazv, nt->potok);
	_getch();
}
 //Составление обратного алфавитного списка веток
void obrat(struct z* vetka)
{
	int i;
	struct sp *nt, *z;
	Console::ForegroundColor = ConsoleColor::Black;
	Console::BackgroundColor = ConsoleColor::Gray;
	Console::Clear();
	if (!spisok)
		for (i = 0; i < NC; i++)
			vstavka(vetka, vetka[i].name);
	Console::Clear();
		printf("\n Обратный алфавитный список веток московского метро");
		printf("\n ==================================================\n");
	for (nt = spisok, z = 0; nt!= 0; z = nt, nt = nt->sled);
	for (nt = z; nt != 0; nt = nt->pred) //печатаем обычный список с конца			 
		printf("\n %-25s	 Пассажиропоток: %ld тыс.чел/сут", nt->nazv, nt->potok);
	_getch();
}


//Вставка вкладчика в алфавитный список
void vstavka(struct z* vetka, char* nazv)
{
	int i;
	struct sp* nov, * nt, * z = 0;
	for (nt = spisok; nt != 0 && strcmp(nt->nazv, nazv) < 0; z = nt, nt = nt->sled);
	if (nt && strcmp(nt->nazv, nazv) == 0) return; //ищем совпадения
	nov = (struct sp*) malloc(sizeof(struct sp)); //выделяем память для nov
	strcpy(nov->nazv, nazv); //копируем в nov название
	nov->sled = nt; //переходим к след.
	nov->potok = 0;
	for (i = 0; i < NC; i++) //запускаем цикл для поиска совпадений
		if (strcmp(vetka[i].name, nazv) == 0)
			nov->potok += vetka[i].potok; //вывод сумму потоков при совпадении
	nov->sled = nt;
	nov->pred = z;
	if (!z) spisok = nov; //заносим в nov при не совпадении.
	else z->sled = nov;//в обратном случае заносим следующее значение
	if (nt) nt->pred = nov;
	return;
}

// Диаграмму строим на основе псевдографики
// Столбик формируется цветной полоской из N
// пробелов

//построение диаграммы
void diagram(struct z* vetka)
{
	struct sp* nt;
	int len, i, NColor,s;
	long sum = 0;
	char str1[40];
	char str2[20];
	System::ConsoleColor Color; //объявляем постоянные переменный для цвета
	Console::ForegroundColor = ConsoleColor::Black; //выбор цвета для символов
	Console::BackgroundColor = ConsoleColor::White; //закраска фона
	Console::Clear();
	for (i = 0; i < NC; i++) 
		sum = sum + vetka[i].potok; //циклом заполняем переменную sum из всех элементов
	if (!spisok)
		for (i = 0; i < NC; i++)
			vstavka(vetka, vetka[i].name); //вставка названия помощи функции
	Color = ConsoleColor::Black; NColor = 0; //покраска символов и обнуление счётчика
	for (nt = spisok, i = 0; nt != 0; nt = nt->sled, i++)
	{
		sprintf(str1, "%s", nt->nazv);
		sprintf(str2, "%3.1f%%", (nt->potok * 100. / sum));  //расчет процента от общей суммы
		Console::ForegroundColor = ConsoleColor::Black;
		Console::BackgroundColor = ConsoleColor::White;
		Console::CursorLeft = 5;	    Console::CursorTop = i + 1; 
		printf(str1);
		Console::CursorLeft = 35;
		printf("%s", str2);
		Console::BackgroundColor = ++Color; NColor++;
		Console::CursorLeft = 45;
		for (len = 0; len < nt->potok * 100 / sum; len++) //цикл для расчета
		printf(" ");
		if (NColor == 14) //выделение различных цветов
		{
			Color = ConsoleColor::Black;  //возвращаем черный
			NColor = 0; //обнуляем счётчик
		}
	}
	_getch();
	return;
}
//Ветки с одинаковой загруженностью и датой открытия
void hard(struct z* vetka)
{
	int l, i;
	int y=0, k=1; //объявление двух счётчиков
	Console::ForegroundColor = ConsoleColor::DarkBlue;
	Console::BackgroundColor = ConsoleColor::White;
	for (i=0; i<NC; i++) //общий цикл
	{
		for (l=0; l<NC-i-1; l++) //запускаем цикл для сравнение с последним
		{
			if (vetka[i].potok == vetka[i +1 +l].potok && (strcmp(vetka[i].data, vetka[i +1 +l].data) == 0)) //сравнение по двум переменным
			{ 
				Console::CursorLeft = 10;		//установка курсора
				Console::CursorTop = 15+y;
				printf("%d Совпадение:", k);
				Console::CursorLeft = 10;
				Console::CursorTop = 16+y;
				printf("Поток пассажиров: %ld тыс.чел.", vetka[i].potok);
				Console::CursorLeft = 10;
				Console::CursorTop = 17+y;
				printf("Дата открытия ветки: %s года", vetka[i].data);
				Console::CursorLeft = 10;
				Console::CursorTop = 18+y;
				printf("Название первой ветки: %s", vetka[i].name); //вывод 1-ой совпавшей по потоку и дате ветки 
				Console::CursorLeft = 10;
				Console::CursorTop = 19+y;
				printf("Название второй ветки: %s", vetka[l +1 +i].name); //вывод 2-ой совпавшей по потоку и дате ветки 
				y+=5; 
				k++; //Кол-во совпадений
			}
		}
	}
	if (y == 0) 
		{ //вывод при отсутсвие совпадений
			Console::CursorLeft = 10;
			Console::CursorTop = 15;
			Console::ForegroundColor = ConsoleColor::Yellow;
			Console::BackgroundColor = ConsoleColor::Black;
			printf("Совпадений нет"); 
		}
	_getch();
}

//int main(array<System::String ^> ^args)
//{
//return 0;
//}
