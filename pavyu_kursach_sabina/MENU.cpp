#include "Total_Header.h"  
#include <locale>      // Для работы с локалью (точка как разделитель)
#include <cmath>       // Для math функций
#include <iomanip>     // Для setw, setprecision

using namespace std;


// ОБРАБОТЧИКИ ПУНКТОВ МЕНЮ

// Добавить точку маршрута
void handleAddPoint() {
    system("cls");
    cout << "ДОБАВЛЕНИЕ ТОЧКИ МАРШРУТА\n\n";

    const int m = 4;
    const string menu[m] = {
        "Добавить в НАЧАЛО маршрута",
        "Добавить в КОНЕЦ маршрута",
        "Добавить МЕЖДУ точками (после указанной)",
        "Вернуться в главное меню"
    };

    int n = 0;
    unsigned char ch;

    do {
        system("cls");
        cout << "ДОБАВЛЕНИЕ ТОЧКИ МАРШРУТА\n\n";
        showMenu(menu, m, n);

        ch = _getch();
        if (ch == 224) {
            ch = _getch();
            if (ch == 72) { n--; if (n < 0) n = m - 1; }
            if (ch == 80) { n++; if (n > m - 1) n = 0; }
        }
    } while (ch != 13);

    if (n == 3) return;

    double x = inputDouble("Введите координату X: ");
    double y = inputDouble("Введите координату Y: ");

    if (n == 0) {
        addPointFirst(x, y);// в начало 
    }
    else if (n == 1) {
        addPointLast(x, y);// в конец 
    }
    else if (n == 2) {// после какойто
        if (isEmpty()) {
            cout << "Маршрут пуст! Точка будет добавлена в начало.\n";
            addPointFirst(x, y);
        }
        else {
            int id = inputInt("Введите ID точки, ПОСЛЕ которой вставить новую: ");
            addPointAfter(id, x, y);// добавляем 
        }
    }

    waitForEnter();// ждем эдера 
}

// Найти, редактировать или удалить точку
void handleEditDeletePoint() {
    system("cls");
    cout << "ПОИСК, РЕДАКТИРОВАНИЕ И УДАЛЕНИЕ\n\n";

    if (isEmpty()) {
        cout << "Маршрут пуст! Нечего редактировать.\n";
        waitForEnter();
        return;
    }

    const int m = 4;
    const string menu[m] = {
        "Найти точку по ID",
        "Изменить координаты точки по ID",
        "Удалить точку по ID",
        "Вернуться в главное меню"
    };

    int n = 0;
    unsigned char ch;

    do {
        system("cls");
        cout << "ПОИСК, РЕДАКТИРОВАНИЕ И УДАЛЕНИЕ\n\n";
        showMenu(menu, m, n);
        ch = _getch();
        if (ch == 224) {
            ch = _getch();
            if (ch == 72) { n--; if (n < 0) n = m - 1; }
            if (ch == 80) { n++; if (n > m - 1) n = 0; }
        }
    } while (ch != 13);

    if (n == 3) return;

    int id = inputInt("Введите ID точки: ");
    PointNode* p = findPointById(id);// находим точку 

    if (n == 0) {//найти 
        if (p != nullptr) {// если точка найдена 
            cout << "\nНайдена точка:\n";
            cout << "  ID = " << p->id << "\n";
            cout << "  Исходные координаты: X = " << formatDouble(p->x) << ", Y = " << formatDouble(p->y) << "\n";// красиво выводим 
            if (p->isTransformed) {// если преобразование было пременинно то выводим и эти координатф 
                cout << "  Целевые координаты: TX = " << formatDouble(p->tx) << ", TY = " << formatDouble(p->ty) << "\n";
            }
            else {
                cout << "  Преобразование ещё не применено.\n";
            }
        }
        else {
            cout << "Точка с ID " << id << " не найдена!\n";
        }
    }
    else if (n == 1) {// изменить координаты 
        if (p != nullptr) {
            double nx = inputDouble("Введите новый X: ");
            double ny = inputDouble("Введите новый Y: ");
            editPointById(id, nx, ny);// изменяем 
        }
        else {
            cout << "Точка с ID " << id << " не найдена!\n";
        }
    }
    else if (n == 2) {// удаляем 
        deletePointById(id);
    }

    waitForEnter();
}

// Просмотреть маршрут
void handleViewRoute() {
    system("cls");
    cout << "ПРОСМОТР МАРШРУТА\n\n";

    if (isEmpty()) {
        cout << "Маршрут пуст! Нет точек для отображения.\n";
        waitForEnter();
        return;
    }

    const int m = 3;
    const string menu[m] = {
        "Просмотр в ПРЯМОМ порядке (от первой точки к последней)",
        "Просмотр в ОБРАТНОМ порядке (от последней точки к первой)",
        "Вернуться в главное меню"
    };

    int n = 0;
    unsigned char ch;

    do {
        system("cls");
        cout << "ПРОСМОТР МАРШРУТА\n\n";
        showMenu(menu, m, n);
        ch = _getch();
        if (ch == 224) {// первый байт спец клавиши 
            ch = _getch();//читаем второй байт 
            if (ch == 72) { n--; if (n < 0) n = m - 1; }// верх
            if (ch == 80) { n++; if (n > m - 1) n = 0; }//вниз
        }
    } while (ch != 13);// пока не нажмём эндер 

    if (n == 2) return;// возвращаемся 

    cout << "\n--- МАРШРУТ ---\n";
    cout << setw(6) << left << "ID"// ширина 6 выравнивание по левому краю 
        << "| " << setw(15) << left << "X (исх)"
        << "| " << setw(15) << left << "Y (исх)"
        << "| " << setw(15) << left << "TX (цел)"
        << "| " << setw(15) << left << "TY (цел)"
        << "\n";//шапочку табл нарисовали 
    cout << string(85, '-') << "\n";// подчеркнули 

    if (n == 0) {// прямой порядок 
        PointNode* curr = head;// с головы 
        while (curr != nullptr) {// и до конца 
            cout << setw(6) << left << curr->id
                << "| " << setw(15) << left << formatDouble(curr->x)
                << "| " << setw(15) << left << formatDouble(curr->y)
                << "| " << setw(15) << left << (curr->isTransformed ? formatDouble(curr->tx) : "---")// если преобразование было выводим занчение 
                << "| " << setw(15) << left << (curr->isTransformed ? formatDouble(curr->ty) : "---")// иначе прочерки 
                << "\n";
            curr = curr->next;// переход к след точку 
        }
    }
    else {// в обратном порядке 
        PointNode* curr = tail;// с конца 
        while (curr != nullptr) {
            cout << setw(6) << left << curr->id
                << "| " << setw(15) << left << formatDouble(curr->x)
                << "| " << setw(15) << left << formatDouble(curr->y)
                << "| " << setw(15) << left << (curr->isTransformed ? formatDouble(curr->tx) : "---")
                << "| " << setw(15) << left << (curr->isTransformed ? formatDouble(curr->ty) : "---")
                << "\n";
            curr = curr->prev;// перебираем 
        }
    }

    waitForEnter();
}

//Задать параметры и применить преобразование
void handleTransform() {
    system("cls");
    cout << "ПРЕОБРАЗОВАНИЕ СИСТЕМ КООРДИНАТ\n\n";

    if (isEmpty()) {
        cout << "Маршрут пуст! Нечего преобразовывать.\n";
        waitForEnter();
        return;
    }

    cout << "Ввод параметров взаимного положения систем координат:\n";
    angleDeg = inputDouble("Введите угол поворота (в градусах, против часовой стрелки): ");
    originX = inputDouble("Введите X начала исходной СК в целевой СК: ");
    originY = inputDouble("Введите Y начала исходной СК в целевой СК: ");

    initTransformMatrix();// создали матрицу прямиго 
    initInverseMatrix();// и обратного преобразования 
    applyTransformToAll();// умножили на вектор 

    cout << "\nПреобразование успешно применено ко всем точкам маршрута!\n";
    waitForEnter();
}

//Вывести матрицу, формулы и обратное преобразование
void handleShowMath() {
    system("cls");
    cout << "МАТЕМАТИЧЕСКИЙ АППАРАТ ПРЕОБРАЗОВАНИЙ\n\n";

    cout << "1. Матрица прямого преобразования (3x3):\n";
    for (int i = 0; i < 3; i++) {// строки
        cout << "| ";
        for (int j = 0; j < 3; j++) {
            cout << setw(10) << left << formatDouble(matrix.m[i][j]);
        }
        cout << "|\n";
    }

    //вывод вида 
    //X_new =cosX -sinY +x0
    //Y_new =sinX +cosY +y0
    // из матрицы вида 

    //cos -sin x0
    //sin cos y0
    // 0 0 1
    cout << "\n2. Формулы прямого перехода (с числовыми коэффициентами):\n";
    cout << "   X_new = " << formatDouble(matrix.m[0][0]) << " * X + ("
        << formatDouble(matrix.m[0][1]) << ") * Y + " << formatDouble(matrix.m[0][2]) << "\n";
    cout << "   Y_new = " << formatDouble(matrix.m[1][0]) << " * X + "
        << formatDouble(matrix.m[1][1]) << " * Y + " << formatDouble(matrix.m[1][2]) << "\n";

    cout << "\n3. Проверка обратного преобразования (восстановление исходных координат):\n";

    if (isEmpty()) {
        cout << "   Маршрут пуст.\n";
    }
    else {
        cout << setw(6) << left << "ID"
            << "| " << setw(20) << left << "Исходные (X,Y)"
            << "| " << setw(30) << left << "Целевые (TX,TY)"
            << "| " << setw(20) << left << "Восстановленные"
            << "| " << setw(15) << left << "Погрешность"
            << "\n";
        cout << string(100, '-') << "\n";

        PointNode* curr = head;
        while (curr != nullptr) {
            if (curr->isTransformed) {// проверка преобразованных точек 

                //матрица на вектор
                double revX = invMat.m[0][0] * curr->tx + invMat.m[0][1] * curr->ty + invMat.m[0][2];
                double revY = invMat.m[1][0] * curr->tx + invMat.m[1][1] * curr->ty + invMat.m[1][2];

                // погрешности 
                double errX = abs(curr->x - revX);
                double errY = abs(curr->y - revY);

                cout << setw(6) << left << curr->id
                    << "| " << setw(20) << left << ("(" + formatDouble(curr->x) + ", " + formatDouble(curr->y) + ")")
                    << "| " << setw(30) << left << ("(" + formatDouble(curr->tx) + ", " + formatDouble(curr->ty) + ")")
                    << "| " << setw(20) << left << ("(" + formatDouble(revX) + ", " + formatDouble(revY) + ")")
                    << "| " << setw(15) << left << (formatDouble(errX) + ", " + formatDouble(errY))
                    << "\n";
            }
            curr = curr->next;
        }
    }

    waitForEnter();
}

//Вычислить длину маршрута
void handleCalcLength() {
    system("cls");
    cout << "ВЫЧИСЛЕНИЕ ДЛИНЫ МАРШРУТА\n\n";

    if (isEmpty()) {
        cout << "Маршрут пуст! Длина = 0.\n";
    }
    else {
        double len = calculateRouteLength();
        cout << "Длина маршрута (сумма расстояний между соседними точками): " << formatDouble(len) << "\n";


    }

    waitForEnter();
}

// Сохранить данные в текстовый файл
void handleSaveToFile() {
    system("cls");
    cout << "СОХРАНЕНИЕ В ФАЙЛ\n\n";

    //пустой ли маршрут
    if (isEmpty()) {
        cout << "Маршрут пуст! Нечего сохранять.\n";
        waitForEnter();
        return;
    }

    string filename;

    // Цикл повторяется до тех пор, пока не будет введено корректное имя или '#'
    while (true) {
        cout << "Введите имя файла для сохранения (или нажмите # для возврата в меню) : ";
        getline(cin, filename);

        // Проверка на возврат в главное меню
        if (filename == "#") {
            return;
        }

        //  Проверка на наличие пробелов или табуляции
        bool hasSpace = false;// были ли пробелы 
        for (char c : filename) {
            if (c == ' ' || c == '\t') {
                hasSpace = true;// да были 
                break;
            }
        }
        if (hasSpace) {
            cout << "Ошибка: имя файла не должно содержать пробелы!\n";
            continue; // Возвращаемся в начало цикла, чтобы запросить имя снова
        }

        //Проверка на запрещённые символы
        const string invalidChars = "<>:\\\"/\\|?*+=";
        char badChar = '\0';// запоминаем плохой символ 
        for (char c : filename) {
            if (invalidChars.find(c) != string::npos) {// если текущий символ есть в строке с запрещёнкой 
                badChar = c;// то запоминаем этот цикл 
                break;
            }
        }

        if (badChar != '\0') {// если нашли плохой симвоол 
            cout << "Ошибка: имя файла содержит запрещенный символ '" << badChar << "'!\n";
            cout << "Разрешены только буквы, цифры, точка, дефис и подчеркивание.\n\n";
            continue; // Возвращаемся в начало цикла
        }

        // Проверка на пустую строку (если пользователь ввёл только запрещённые символы, которые мы не отловили, или просто нажал Enter)
        if (filename.empty()) {
            cout << "Ошибка: имя файла не может быть пустым!\n\n";
            continue;
        }

        //  Проверка и коррекция расширения .txt
        if (filename.find('.') == string::npos) {
            // Если точки нет вообще, добавляем .txt
            filename += ".txt";
        }
        else {
            // Если точка есть, проверяем, что это именно расширение .txt в конце
            size_t dotPos = filename.find_last_of('.');// позиция ласт точки в строке 
            string ext = filename.substr(dotPos);//извлекаем все что после ласт точки

            if (ext != ".txt") {
                cout << "Ошибка: файл должен иметь расширение .txt (введено расширение: " << ext << ")!\n\n";
                continue;
            }
            if (dotPos == 0) {
                cout << "Ошибка: базовое имя файла не может быть пустым (нельзя назвать файл просто '.txt')!\n\n";
                continue;
            }
        }

        //  все проверки пройдены 

        break;
    }

    //  Попытка открыть файл для записи (имя гарантированно корректно)
    ofstream file(filename);
    if (!file.is_open()) {
        cout << "Ошибка: не удалось открыть файл \"" << filename << "\" для записи!\n";
        waitForEnter();
        return;
    }

    // Запись данных в файл
    file << "PARAMS " << angleDeg << " " << originX << " " << originY << "\n";

    PointNode* curr = head;
    while (curr != nullptr) {
        file << curr->id << " " << curr->x << " " << curr->y;
        if (curr->isTransformed) {
            file << " " << curr->tx << " " << curr->ty;
        }
        file << "\n";
        curr = curr->next;
    }

    file.close();


    cout << "\nДанные успешно сохранены в файл \"" << filename << "\"!\n";
    waitForEnter();
}

// Загрузить данные из текстового файла
void handleLoadFromFile() {
    system("cls");
    cout << "ЗАГРУЗКА ИЗ ФАЙЛА\n\n";

    string filename;

    //  Цикл повторяется до тех пор, пока не будет введен корректный файл или '#'
    while (true) {
        cout << "Введите имя файла для загрузки (или введите # для возврата в меню): ";
        getline(cin, filename);

        // Проверка на возврат в главное меню
        if (filename == "#") {
            return;
        }

        // Удаляем пробелы по краям имени файла
        filename = removeSpaces(filename);

        if (filename.empty()) {
            cout << "Ошибка: имя файла не может быть пустым!\n\n";
            continue;
        }

        //  Попытка открыть файл
        ifstream file(filename);
        if (!file.is_open()) {
            cout << "Ошибка: файл \"" << filename << "\" не найден или не может быть открыт!\n";
            continue;
        }

        // пустой ли файл и начинается ли он с PARAMS
        string firstLine;
        bool foundFirstLine = false;// есть ли файле хотя бы одна строка с текстом 
        string firstToken = "";// первое слово должно быть не мама..

        // Читаем файл построчно, чтобы найти первую НЕПУСТУЮ строку
        while (getline(file, firstLine)) {
            istringstream tempIss(firstLine);// читаем из строки 
            if (tempIss >> firstToken) { // Если удалось прочитать первое слово
                foundFirstLine = true;// да файл не пустой 
                break;
            }
        }

        if (!foundFirstLine) {
            cout << "Ошибка: файл \"" << filename << "\" пуст!\n";
            file.close();
            continue;
        }

        // начинается ли первая строка с ключевого слова PARAMS
        if (firstToken != "PARAMS") {
            cout << "Ошибка: файл \"" << filename << "\" содержит некорректные данные!\n";
            file.close();
            continue;
        }

        //  файл прошел все проверки
        file.close(); // Закрываем файл, чтобы позже открыть его заново для полного чтения
        break;
    }

    //  Файл гарантированно корректен. Начинаем загрузку
    // 
    // Очищаем текущий список перед загрузкой новых данных
    clearList();// удаляем старый маршут 
    nextId = 1; // Сбрасываем счётчик ID

    //PARAMS angleDeg originX originY (если нет преобразования то по 000)
    //id x y tx ty (если преобразования нет то последнх двух слов нет )


    ifstream file(filename); // Открываем файл заново для чтения
    bool paramsLoaded = false;// флак есть ли во входном файле преобразования 
    string line;

    while (getline(file, line)) {// читаем построчно 
        istringstream iss(line);// открываем поток 
        iss.imbue(locale("C")); // читаем числа с точкой
        string token;// хранит слово из строки или число 
        iss >> token;

        if (token == "PARAMS") {// значит после идут угол и координаты сдвига 
            iss >> angleDeg >> originX >> originY;
            initTransformMatrix();// строим сазу матрицы для дальнейшей работы
            initInverseMatrix();
            paramsLoaded = true;
        }
        else {
            // Обработка строки с координатами 
            try {
                int id = stoi(token);// ид деламе целым на всякий а то вдруг вмсето цифр там буквы
                double x, y;
                iss >> x >> y;

                PointNode* newNode = new PointNode;
                newNode->id = id;
                newNode->x = x;
                newNode->y = y;
                newNode->tx = 0;
                newNode->ty = 0;
                newNode->isTransformed = false;
                newNode->prev = tail;// добавляем вконец 
                newNode->next = nullptr;// у новой точки нет след элемента

                if (tail != nullptr) {// если список не был пустым  
                    tail->next = newNode;// теперь  стаый хвост указывает на нашу новую точку
                }
                else {// если был пустым 
                    head = newNode;// то новая точка теперь голова 
                }
                tail = newNode;// указатель на хвость указывает на нашу точку 

                double tx, ty;
                if (iss >> tx >> ty) {// пытаемся прочитать преобразованные точки 
                    newNode->tx = tx;
                    newNode->ty = ty;
                    newNode->isTransformed = true;
                }

                if (id >= nextId) {// если прочитанный ид больше нашего глобального след айди 
                    nextId = id + 1;// то перебрасываем 
                }
            }
            catch (...) {
                // Если в строке с точкой данные повреждены, просто пропускаем её, 
                continue;
            }
        }
    }

    file.close();


    cout << "\nМаршрут успешно загружен из файла \"" << filename << "\"!\n";
    if (paramsLoaded) {
        cout << "Параметры преобразования также восстановлены.\n";
    }

    waitForEnter();
}