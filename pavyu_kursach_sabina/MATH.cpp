#include "Total_Header.h"  
#include <locale>      // Для работы с локалью (точка как разделитель)
#include <cmath>       // Для math функций
#include <iomanip>     // Для setw, setprecision

using namespace std;


// МАТЕМАТИЧЕСКИЕ ФУНКЦИИ

//Построить матрицу прямого преобразования по углу и координатам начала из Иходной СК в Целевую
void initTransformMatrix() {
    double rad = angleDeg * M_PI / 180.0;// перевод в радианы
    double c = cos(rad);
    double s = sin(rad);

    matrix.m[0][0] = c;   matrix.m[0][1] = -s;  matrix.m[0][2] = originX;
    matrix.m[1][0] = s;   matrix.m[1][1] = c;   matrix.m[1][2] = originY;
    matrix.m[2][0] = 0;   matrix.m[2][1] = 0;   matrix.m[2][2] = 1;
}

// Построить матрицу обратного преобразования из Целевой в исходную  транспонированная 
void initInverseMatrix() {
    double rad = angleDeg * M_PI / 180.0;// перевод в радианы
    double c = cos(rad);
    double s = sin(rad);

    invMat.m[0][0] = c;   invMat.m[0][1] = s;   invMat.m[0][2] = -c * originX - s * originY;
    invMat.m[1][0] = -s;  invMat.m[1][1] = c;   invMat.m[1][2] = s * originX - c * originY;
    invMat.m[2][0] = 0;   invMat.m[2][1] = 0;   invMat.m[2][2] = 1;
}

// Применить прямое преобразование ко всем точкам маршрута
void applyTransformToAll() {
    PointNode* curr = head;// обход всех точек списка 
    while (curr != nullptr) {
        curr->tx = matrix.m[0][0] * curr->x + matrix.m[0][1] * curr->y + matrix.m[0][2];// матрица* вектор(х у 1)
        curr->ty = matrix.m[1][0] * curr->x + matrix.m[1][1] * curr->y + matrix.m[1][2];
        curr->isTransformed = true;// преобразование применено 
        curr = curr->next;// переход к след элементу 
    }
}

// Применить обратное преобразование ко всем точкам (для проверки)
void applyInverseToAll() {
    PointNode* curr = head;
    while (curr != nullptr) {// пробегаем по каждому элементу 
        if (curr->isTransformed) {// если преобразование было применено то делаем некст 
            double origX = curr->x;// сохр исходные данные 
            double origY = curr->y;

            curr->x = invMat.m[0][0] * curr->tx + invMat.m[0][1] * curr->ty + invMat.m[0][2];// матрица обратного преобразования  на вектор преобразованных 
            curr->y = invMat.m[1][0] * curr->tx + invMat.m[1][1] * curr->ty + invMat.m[1][2];

            curr->x = origX;// возвращаем обратно преобразованные матрицы 
            curr->y = origY;
        }
        curr = curr->next;// переход к след матрице 
    }
}

// Вычислить длину маршрута (сумма расстояний между соседними точками)В
double calculateRouteLength() {
    if (isEmpty() || head->next == nullptr) {// если список пуст или ток одна точка 
        return 0.0;
    }

    double length = 0.0;
    PointNode* curr = head;

    while (curr->next != nullptr) {// пробегаем каждый узел 
        double dx = curr->next->x - curr->x;// разность по горизонтали
        double dy = curr->next->y - curr->y;// разность по вертикали 
        length += sqrt(dx * dx + dy * dy);// складиваем все эти гипотенузки 
        curr = curr->next;// переход к след точке 
    }

    return length;
}
