#define _CRT_SECURE_NO_WARNINGS
#include <iostream>   
#include <windows.h>      
#include "Total_Header.h"  

using namespace std;

int main() {
  
    setlocale(LC_ALL, "Russian");          
    SetConsoleCP(1251);                    
    SetConsoleOutputCP(1251);           

    bool exitProgram = false;  // управления циклом программы (false = работать, true = выйти)

    // Главный цикл программы
    while (!exitProgram) {
        int choice = menu1Action();  // Показываем главное меню и получаем выбор пользователя (1-10)

        // Обработка выбора пользователя
        switch (choice) {
        case 1:  // Добавить точку
            handleAddPoint();
            break;

        case 2:  // Найти, редактировать, удалить
            handleEditDeletePoint();
            break;

        case 3:  // Просмотр маршрута
            handleViewRoute();
            break;

        case 4:  // Преобразование координат
            handleTransform();
            break;

        case 5:  // Вывод математики
            handleShowMath();
            break;

        case 6:  // Длина маршрута
            handleCalcLength();
            break;

        case 7:  // Сохранить в файл
            handleSaveToFile();
            break;

        case 8:  // Загрузить из файла
            handleLoadFromFile();
            break;

        case 9:  // Очистить список
            clearList();
            cout << "Маршрут полностью очищен! Все точки удалены, память освобождена.\n";
            waitForEnter();
            break;

        case 10:  // Завершить программу
            exitProgram = true;
            break;

        default:  // Неизвестный выбор (на всякий )
            exitProgram = true;
            break;
        }
    }

    // Очищаем структуру и освобождаем ВСЮ выделенную динамическую память
    clearList();

    system("cls");
    cout << "Программа завершена. Вся память освобождена.\n";

    return 0;  
}