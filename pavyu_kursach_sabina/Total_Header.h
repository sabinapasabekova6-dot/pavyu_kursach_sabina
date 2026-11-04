#pragma once
#define M_PI 3.14159265358979323846

#pragma once
#define _CRT_SECURE_NO_WARNINGS
#include <iostream>    
#include <string>      
#include <windows.h>    
#include <fstream>           
#include <sstream>     
#include <conio.h>    
#include <cmath>
#include <iomanip>     // setw, left, right 
#include <locale>

using namespace std;

// СТРУКТУРЫ ДАННЫХ

// Узел двусвязного списка (одна точка маршрута)
struct PointNode {
    int id;               // номер точки (каким по счёту добавляли) (1, 2, 3...)
    double x, y;          // Координаты точки в ИСХОДНОЙ СК
    double tx, ty;        // Координаты точки в ЦЕЛЕВОЙ системе (после преобразования)
    bool isTransformed;   // Флаг: true = преобразование уже применено, false = ещё нет
    PointNode* prev;      // Указатель на ПРЕДЫДУЩУЮ точку в списке 
    PointNode* next;      //  на СЛЕДУЮЩУЮ точку в списке (nullptr если это последняя точка)
};

// Матрица преобразования 3x3 (для однородных координат)
//  коэффициенты для формул: X_new = a*X + b*Y + c, Y_new = d*X + e*Y + f
struct TransformMatrix {
    double m[3][3];  
};

// ГЛОБАЛЬНЫЕ ПЕРЕМЕННЫЕ 

extern PointNode* head;         // Указатель на ПЕРВУЮ точку маршрута (голова списка)
extern PointNode* tail;         // Указатель на ПОСЛЕДНЮЮ точку маршрута (хвост списка)
extern int nextId;              // Счётчик для генерации уникальных ID (каждая новая точка получает nextId, потом nextId++)
extern TransformMatrix matrix;  // Матрица ПРЯМОГО преобразования (из исходной СК в целевую)
extern TransformMatrix invMat;  // Матрица ОБРАТНОГО преобразования (из целевой СК обратно в исходную)
extern double angleDeg;         // Угол поворота в градусах (введён пользователем)
extern double originX, originY; // Координаты начала исходной СК в целевой СК (введёны пользователем)

// для меню
void showMenu(const string menu[], int num, int active);  // Показать меню 
int menu1Action();  // Показать главное меню, обработать навигацию стрелками, вернуть выбор пользователя (1-10)

//  ввода с проверкой 
double inputDouble(const string& prompt);  // Запросить число double у пользователя с проверкой корректности
int inputInt(const string& prompt);        // Запросить целое число у пользователя с проверкой корректности
void waitForEnter();  // "Нажмите Enter" и ждать нажатия клавиши

//Функции работы со двусвязанным списоком
void addPointFirst(double x, double y);   // Добавить точку в НАЧАЛО маршрута 
void addPointLast(double x, double y);    // Добавить точку в КОНЕЦ маршрута 
void addPointAfter(int id, double x, double y);  // Добавить точку ПОСЛЕ точки с указанным ID
PointNode* findPointById(int id);         // Найти точку по ID, вернуть указатель на неё (или nullptr если не найдена)
void deletePointById(int id);             // Удалить точку с указанным ID из списка
void editPointById(int id, double x, double y);  // Изменить координаты точки с указанным ID
bool isEmpty();                           // пуст ли список 
void clearList();                         // Удалить ВСЕ точки из списка и освободить память

//Обработчики пунктов меню 
void handleAddPoint();           // Добавить точку маршрута
void handleEditDeletePoint();    // Найти, редактировать или удалить точку
void handleViewRoute();          // Просмотреть маршрут
void handleTransform();          // Задать параметры и применить преобразование
void handleShowMath();           // Вывести матрицу, формулы и обратное преобразование
void handleCalcLength();         // Вычислить длину маршрута
void handleSaveToFile();         // Сохранить данные в текстовый файл
void handleLoadFromFile();       // Загрузить данные из текстового файла

//Математические функции 
void initTransformMatrix();      // Построить матрицу прямого преобразования по углу и координатам начала
void initInverseMatrix();        // Построить матрицу обратного преобразования
void applyTransformToAll();      // Применить прямое преобразование ко всем точкам маршрута
void applyInverseToAll();        // Применить обратное преобразование ко всем точкам (для проверки)
double calculateRouteLength();   // Вычислить длину маршрута (сумма расстояний между соседними точками)

//Вспомогательные функции
string removeSpaces(const string& str);  // Удалить все пробелы и табуляции из строки
string formatDouble(double value);       // Форматировать число для вывода (убрать лишние нули в конце)
bool isValidDouble(const string& str);   // Проверить, является ли строка корректным числом