#include "Total_Header.h"  
#include <locale>      // Для работы с локалью (точка как разделитель)
#include <cmath>       // Для math функций
#include <iomanip>     // Для setw, setprecision

using namespace std;

// ИНИЦИАЛИЗАЦИЯ ГЛОБАЛЬНЫХ ПЕРЕМЕННЫХ

PointNode* head = nullptr;      //  (нет первой точки)
PointNode* tail = nullptr;      //  (нет последней точки)
int nextId = 1;                 // Первая точка получит ID = 1
TransformMatrix matrix = { 0 }; // (из исходной СК в целевую)
TransformMatrix invMat = { 0 }; // (из целевой СК обратно в исходную)
double angleDeg = 0.0;          // Угол поворота
double originX = 0.0;           // Перенос по X 
double originY = 0.0;           // Перенос по Y 

// ВСПОМОГАТЕЛЬНЫЕ ФУНКЦИИ (Ввод, вывод, форматирование)

// Удалить все пробелы и символы табуляции из строки
string removeSpaces(const string& str) {
    string result;
    for (char c : str) {
        if (c != ' ' && c != '\t') {
            result += c;
        }
    }
    return result;
}

// Форматировать число double для вывода: убрать лишние нули в конце
// Примеры: 7.9020 > "7.902", -9.2300 > "-9.23", 5.0 > "5", 0.0 > "0"
string formatDouble(double value) {
    if (value == 0.0) {
        return "0"; // -0 и +0 превращаем в чистый "0"
    }

    ostringstream oss;// поток для записи в строку 
    oss.imbue(locale("C")); //  вложить настройку  анг чтобы точку как разделитель дробной части 
    oss << fixed << setprecision(10) << value;// точность до 10 наков, в обычном выде 
    string str = oss.str();// положили из потока 

    size_t dotPos = str.find('.');// находим позицию точки 
    if (dotPos == string::npos) {// если без точки возвращаем как есть 
        return str;
    }

    // Удаляем нули в конце после точки
    while (str.back() == '0' && str.length() > dotPos + 1) { // пока последний символ 0 и не дошли до точки
        str.pop_back();// удаляем последний символ если он ноль 
    }

    // Если после удаления нулей осталась только точка, удаляем и её
    if (str.back() == '.') {
        str.pop_back();
    }

    return str;
}

// Проверить, является ли строка корректным числом с плавающей точкой
bool isValidDouble(const string& str) {
    if (str.empty()) return false;

    istringstream iss(str);// читаем строку 
    iss.imbue(locale("C")); // что бы считывало после точки 
    double d;
    iss >> d;

    return iss.eof() && !iss.fail();// достигли ли мы конца строки или встр букву и остановились И не произошла ли ошибка при чтении 
}

// Запросить число у пользователя с проверкой корректности
double inputDouble(const string& prompt) {
    while (true) {
        cout << prompt;// подстказка что вводить 
        string input;
        getline(cin, input);

        input = removeSpaces(input);

        // Заменяем запятую на точку для удобства пользователя
        for (char& c : input) {
            if (c == ',') {
                c = '.';
            }
        }

        if (input.empty()) {
            cout << "Ошибка: введите число! А не пустую строку\n";
            continue;
        }

        if (input[0] == '.' || (input[0] == '-' && input.length() > 1 && input[1] == '.')) {
            cout << "Ошибка: число не может начинаться с точки !\n";
            continue;
        }

        if (input.length() > 1 && input[0] == '0' && input[1] != '.') {
            cout << "Ошибка: целочисленное число не может начинаться с нуля!\n";
            continue;
        }

        if (input.length() > 2 && input[0] == '-' && input[1] == '0' && input[2] != '.') {
            cout << "Ошибка: целочисленное число не может начинаться с -0 !\n";
            continue;
        }

        if (!isValidDouble(input)) {
            cout << "Ошибка: введите корректное число!\nДопустимые символы: цифры, точка, запятая, минус (в начале)\n";
            continue;
        }
        //  переводим строку в тип дабле 
        istringstream iss(input);
        iss.imbue(locale("C"));
        double value;
        iss >> value;

        if (value == 0.0) {// убирапем минус и плюс ноль 
            value = 0.0; 
        }

        return value;
    }
}

// Запросить целое число для ид 
int inputInt(const string& prompt) {
    while (true) {
        cout << prompt;
        string input;
        getline(cin, input);

        input = removeSpaces(input);// удаляем пробелы  

        if (!isValidDouble(input)) {
            cout << "Ошибка: введите целое число!\n";
            continue;
        }

        istringstream iss(input);
        iss.imbue(locale("C"));
        double d;
        iss >> d;

        if (d != floor(d)) {// округление в меньшую строну  чтобы 1.0 считалось как 1
            cout << "Ошибка: введите целое число (без дробной части)!\n";
            continue;
        }

        if (d < INT_MIN || d > INT_MAX) {// чтобы тип флоат не переполнил бедный инт 
            cout << "Ошибка: число слишком большое!\n\n";
            continue;
        }

        return static_cast<int>(d);// отбрасываеим дробную часть 
    }
}

// Показать сообщение "Нажмите Enter" и ждать нажатия клавиши
void waitForEnter() {
    cout << "Нажмите Enter для продолжения... ";
    cin.ignore(10000, '\n');//удалие из буфера и остановка на эндеоре 
}

// Вывести меню 
void showMenu(const string menu[], int num, int active) {
    const string act_mark = ">> ";
    const string inact_mark = "   ";

    for (int i = 0; i < num; i++) {
        if (i == active) {
            cout << act_mark;
        }
        else {
            cout << inact_mark;
        }
        cout << menu[i] << endl;
    }
}



// ГЛАВНОЕ МЕНЮ

int menu1Action() {
    const int m = 10;
    const string menu[m] = {
        "Добавить точку маршрута",
        "Найти, редактировать или удалить точку",
        "Просмотреть маршрут (прямой/обратный)",
        "Задать параметры и применить преобразование",
        "Вывести матрицу, формулы и обратное преобразование",
        "Вычислить длину маршрута",
        "Сохранить данные в текстовый файл",
        "Загрузить данные из текстового файла",
        "Очистить маршрут (удалить все точки)",
        "Завершить работу программы"
    };

    int n = 0;
    unsigned char ch;

    do {
        system("cls");
        cout << "УПРАВЛЕНИЕ МАРШРУТОМ И ПРЕОБРАЗОВАНИЕМ КООРДИНАТ\n\n";
        cout << "Выберите действие:\n";
        showMenu(menu, m, n);

        cout << "\nТЕКУЩЕЕ СОСТОЯНИЕ:\n";
        if (isEmpty()) {
            cout << "  Маршрут пуст.\n";
        }
        else {
            int count = 0;// кол-во точек 
            PointNode* curr = head;
            while (curr) {
                count++;
                curr = curr->next;
            }
            cout << "  Количество точек: " << count << "\n";

            if (head->isTransformed) {
                cout << "  Статус: Преобразование применено.\n";
            }
            else {
                cout << "  Статус: Преобразование НЕ применено.\n";
            }
        }
        cout << endl;

        ch = _getch();
        if (ch == 224) {
            ch = _getch();
            if (ch == 72) { n--; if (n < 0) n = m - 1; }
            if (ch == 80) { n++; if (n > m - 1) n = 0; }
        }
    } while (ch != 13);

    return n + 1;
}