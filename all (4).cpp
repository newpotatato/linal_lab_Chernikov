#include <iostream>
#include <vector>
#include <map>

class SparseMatrix {
private:
    int rows, cols;
    std::map<std::pair<int,int>, double> elements;

public:
    SparseMatrix(int n, int m) : rows(n), cols(m) {}

    void setElement(int row, int col, double value) {
        if (value != 0) {
            elements[{row, col}] = value;
        }
    }

    double getElement(int row, int col) const {
        auto it = elements.find({row, col});
        if (it != elements.end()) {
            return it->second;
        }
        return 0.0;
    }

    double trace() const {
        double sum = 0.0;
        for (int i = 1; i <= std::min(rows, cols); i++) {
            sum += getElement(i, i);
        }
        return sum;
    }
};

int main() {
    int N, M;
    std::cout << "Введите размеры матрицы (N M): ";
    std::cin >> N >> M;

    SparseMatrix matrix(N, M);

    std::cout << "Введите элементы матрицы:" << std::endl;
    for (int i = 1; i <= N; i++) {
        for (int j = 1; j <= M; j++) {
            double value;
            std::cin >> value;
            matrix.setElement(i, j, value);
        }
    }

    // Пример использования методов
    std::cout << "След матрицы: " << matrix.trace() << std::endl;
    
    int row, col;
    std::cout << "Введите индексы элемента для вывода (строка столбец): ";
    std::cin >> row >> col;
    std::cout << "Элемент [" << row << "][" << col << "]: " 
              << matrix.getElement(row, col) << std::endl;

    return 0;
}
