#include "Total_Header.h"  
#include <locale>      // Для работы с локалью (точка как разделитель)
#include <cmath>       // Для math функций
#include <iomanip>     // Для setw, setprecision

using namespace std;


// ОПЕРАЦИИ С ДВУСВЯЗНЫМ СПИСКОМ

// пустая ли структура 
bool isEmpty() {
    return head == nullptr;
}

// Добавить точку в НАЧАЛО маршрута 
void addPointFirst(double x, double y) {
    PointNode* newNode = new PointNode;// указатель на новый элемент 
    newNode->id = nextId++;// ид добавляемого элемента равед текущий плюс один 
    newNode->x = x;
    newNode->y = y;
    newNode->tx = 0;
    newNode->ty = 0;
    newNode->isTransformed = false;
    newNode->prev = nullptr;// за добовляемым ничего не стоит 
    newNode->next = head;// на  след элемент  указывает на старую голову 

    if (head != nullptr) {// если список не был пустым 
        head->prev = newNode;// то у старой головвы есть предыдущий элемент 
    }
    else {
        tail = newNode;// иначе новая точка и хвост тоже 
    }
    head = newNode;// новая голова 
    cout << "Точка ID " << newNode->id << " успешно добавлена в начало маршрута!\n";
}

// Добавить точку в КОНЕЦ маршрута 
void addPointLast(double x, double y) {
    PointNode* newNode = new PointNode;// указатель на новый элемент 
    newNode->id = nextId++;// ид добавляемого элемента равед текущий плюс один 
    newNode->x = x;
    newNode->y = y;
    newNode->tx = 0;
    newNode->ty = 0;
    newNode->isTransformed = false;
    newNode->next = nullptr;;// после добовляемого ничего не стоит 
    newNode->prev = tail;// на прошлый элемент указывает на хвост 

    if (tail != nullptr) {// если список не был пустум 
        tail->next = newNode;// старый хвост указывет на добавляемый элемент 
    }
    else {
        head = newNode;// иначе новая точка это и голова 
    }
    tail = newNode;// и хвост 
    cout << "Точка ID " << newNode->id << " успешно добавлена в конец маршрута!\n";
}

// Добавить точку ПОСЛЕ точки с указанным ID
void addPointAfter(int id, double x, double y) {
    PointNode* curr = findPointById(id);// возвращаем указатель на точку 
    if (curr == nullptr) {
        cout << "Ошибка: точка с ID " << id << " не найдена!\n";
        return;
    }

    PointNode* newNode = new PointNode;// создаем новую точку
    newNode->id = nextId++;// ид больше на один от последнего
    newNode->x = x;
    newNode->y = y;
    newNode->tx = 0;
    newNode->ty = 0;
    newNode->isTransformed = false;

    newNode->prev = curr;// указатель на прошлую точку указывает на ту после которой добаляем 
    newNode->next = curr->next;// переброс втсавка перешагивание 

    if (curr->next != nullptr) {// если после  не ласт элемент 
        curr->next->prev = newNode;// элемент который стоит после его указатаель на предедущий теперь указывает на наш добавляемый
    }
    else {// мы добавляем после ласт элемента 
        tail = newNode;// наш новый элемент хвост 
    }
    curr->next = newNode;// элемент после указывает на наш добавляемый 

    cout << "Точка ID " << newNode->id << " успешно добавлена после точки ID " << id << "!\n";
}

// Найти точку по ID, вернуть указатель на неё (или nullptr если не найдена)
PointNode* findPointById(int id) {
    PointNode* curr = head;// поиск с головы 
    while (curr != nullptr) {// пока не долшли до конца списка 
        if (curr->id == id) {
            return curr;
        }
        curr = curr->next;// не совпал переход к след точку 
    }
    return nullptr;// не нашли ничего возвращаем указатель 
}

// Удалить точку с указанным ID из списка
void deletePointById(int id) {
    PointNode* curr = findPointById(id);// поиск удаляемой точки
    if (curr == nullptr) {
        cout << "Ошибка: точка с ID " << id << " не найдена!\n";
        return;
    }

    if (curr->prev != nullptr) {// если удал. точка в середине или последняя 
        curr->prev->next = curr->next;// переброс слева направо  указатель на след элемент пролого элемента, указывает на след элемент удаляемого 
    }
    else {// если удаляемый элемент первый 
        head = curr->next;// то голова указывает на след элемент после удаляемого 
    }

    if (curr->next != nullptr) {// если удал. точка в середине или первая 
        curr->next->prev = curr->prev;// переброс справа налево 
    }
    else {// если удал. эл. последний 
        tail = curr->prev;// хвост указывает на прошлый эл. удаляемого 
    }

    delete curr;// удаление из памяти 
    cout << "Точка ID " << id << " успешно удалена!\n";
}

// Изменить координаты точки с указанным ID
void editPointById(int id, double x, double y) {
    PointNode* curr = findPointById(id);
    if (curr == nullptr) {
        cout << "Ошибка: точка с ID " << id << " не найдена!\n";
        return;
    }

    curr->x = x;// меняем на новые координаты 
    curr->y = y;
    curr->isTransformed = false;// для новых точек преобразование не делали по умолчанию 

    cout << "Координаты точки ID " << id << " успешно изменены!\n";
}

// Удалить ВСЕ точки из списка и освободить память
void clearList() {
    PointNode* curr = head;// создаем указатель и начинаем с головы для перебора всех элементов 
    while (curr != nullptr) {
        PointNode* temp = curr;// копируем адрес текущей точки 
        curr = curr->next;// тукущий указатель будет указывать на след элемент пока не дойлем до конца
        delete temp;// чтобы удалить все адреса и они не остались в памяти 
    }
    // после удаления всех элементов тут тоже всё удаляем 
    head = nullptr;
    tail = nullptr;
}