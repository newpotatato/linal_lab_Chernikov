#include <bits/stdc++.h>
#include <windows.h>
#include <vector>    
using namespace std;

// класс для хранения разряженный матрицы
class sparce_matrix {
private:
    vector<int> rows;
    vector<int> cols;
    vector<double> values;
    int n, m;
    bool is_square;
public:
    sparce_matrix(int n, int m, vector<vector<double>> matrix) {
        this->n=n;
        this->m=m;
        rows.resize(n+1,0);
        int count = 0;
        for (int i=0; i<n; i++){
        count = 0;
            for (int j=0; j<m; j++){
                if (matrix[i][j]!=0){
                    count++;
                    values.push_back(matrix[i][j]); 
                    cols.push_back(j);
            }
        }
        rows[i+1] = rows[i] + count;
        is_square = (n == m);
    }
    }
    int get_N(){
        return n;
    }
    int get_M(){
        return m;
    }
    double get_element(int row, int col) {
        if(row < 1 || row > n || col < 1 || col > m) {
            throw out_of_range("Индексы выходят за пределы матрицы");
        }
        
        row--; // переход к нумерации с 0
        col--;
        
        for(int i = rows[row]; i < rows[row+1]; i++) {
            if(cols[i] == col) {
                return values[i];
            }
        }
        return 0;
    }

    vector <double> trace() {
        vector <double> ans;
        if(!is_square) {
            ans ={0.0,0.0};
            return ans;
        }   
        double sum = 0;
        for(int i = 0; i < n; i++) {
            for(int j = rows[i]; j < rows[i+1]; j++) {
                if(cols[j] == i) {
                    sum += values[j];
                    break;
                }
            }
        }
        ans= {1.0,sum};
        return ans;
    }
};

// сложение матриц
sparce_matrix matrix_add(sparce_matrix matrix1, sparce_matrix matrix2) {
    if (matrix1.get_N() != matrix2.get_N() || matrix1.get_M() != matrix2.get_M()) {
        throw invalid_argument("Матрицы должны иметь одинаковые размеры");
    }

    vector<vector<double>> result_matrix(matrix1.get_N(), vector<double>(matrix1.get_M(), 0));

    for (int i = 0; i < matrix1.get_N(); i++) {
        for (int j = 0; j < matrix1.get_M(); j++) {
            result_matrix[i][j] = matrix1.get_element(i+1, j+1) + matrix2.get_element(i+1, j+1);
        }
    }

    return sparce_matrix(matrix1.get_N(), matrix1.get_M(), result_matrix);
}

// умножение матрицы на скаляр
sparce_matrix matrix_scalar_multiply(sparce_matrix matrix, double scalar) {
    vector<vector<double>> result_matrix(matrix.get_N(), vector<double>(matrix.get_M(), 0));

    for (int i = 0; i < matrix.get_N(); i++) {
        for (int j = 0; j < matrix.get_M(); j++) {
            result_matrix[i][j] = matrix.get_element(i+1, j+1) * scalar;
        }
    }

    return sparce_matrix(matrix.get_N(), matrix.get_M(), result_matrix);
}

// умножение матриц
sparce_matrix matrix_multiply(sparce_matrix matrix1, sparce_matrix matrix2) {
    if (matrix1.get_M() != matrix2.get_N()) {
        throw invalid_argument("Матрицы должны иметь размеры, пригодные для умножения");
    }

    vector<vector<double>> result_matrix(matrix1.get_N(), vector<double>(matrix2.get_M(), 0));

    for (int i = 0; i < matrix1.get_N(); i++) {
        for (int j = 0; j < matrix2.get_M(); j++) {
            for (int k = 0; k < matrix1.get_M(); k++) {
                result_matrix[i][j] += matrix1.get_element(i+1, k+1) * matrix2.get_element(k+1, j+1);
            }
        }
    }

    return sparce_matrix(matrix1.get_N(), matrix2.get_M(), result_matrix);
}

// сравнение матриц
bool compare_matrices(sparce_matrix matrix1, sparce_matrix matrix2) {
        if (matrix1.get_N() != matrix2.get_N() || matrix1.get_M() != matrix2.get_M()) {
            return false;
        }
        for (int i = 1; i < matrix1.get_N()+1; i++) {
            for (int j = 1; j < matrix1.get_M()+1; j++) {
                if (matrix1.get_element(i, j) != matrix2.get_element(i, j)) {
                    return false;
                }
            }
        }
        return true;
    }

// вычисление определителя
double determinant(vector<vector<double>>& matrix, int n) {
    double det = 0;
    if (n == 1) {
        return matrix[0][0];
    }
    vector<vector<double>> submatrix(n, vector<double>(n));
    int sign = 1;
    for (int f = 0; f < n; f++) {
        int subi = 0;
        for (int i = 1; i < n; i++) {
            int subj = 0;
            for (int j = 0; j < n; j++) {
                if (j == f)
                    continue;
                submatrix[subi][subj] = matrix[i][j];
                subj++;
            }
            subi++;
        }
        det += sign * matrix[0][f] * determinant(submatrix, n - 1);
        sign = -sign;
    }
    return det;
}

// проверка на обратимость
bool is_invertible(vector<vector<double>>& matrix, int n) {
    double det = determinant(matrix, n);
    return det != 0;
}

// тестирование
void test_from_file(const string& filename, int num_task) {
    ifstream file(filename);
    if (!file.is_open()) {
        throw runtime_error("Не удалось открыть файл");
    }
    int n_test;
    file >> n_test;
    for (int i=0; i<n_test; i++){
    int n, m;
    file >> n >> m;
    
    vector<vector<double>> matrix(n, vector<double>(m));
    for (int i = 0; i < n; i++) {
        for (int j = 0; j < m; j++) {
            file >> matrix[i][j];
        }
    }
    // 1 task
    if (num_task == 1) {
        sparce_matrix test_matrix(n, m, matrix);
    
    cout << "Тестирование матрицы " << n << "x" << m << " из файла:" << endl;
    int row, col;
    double expected_value;
    file >> row >> col >> expected_value;
    
    double actual_value = test_matrix.get_element(row, col);
    if (actual_value==expected_value){
        cout << "Тест на получение элемента пройден"<<endl;
    }
    else{
        cout << "Тест на получение элемента не пройден"<<endl;
    }
    
    vector <double> trace_with_flag= test_matrix.trace();
    double flag_for_trace; double expected_trace;
    file >> flag_for_trace >> expected_trace;
    if (trace_with_flag[0] == flag_for_trace) {
        if (trace_with_flag[1]==expected_trace){
            cout << "Тест следа пройден"<< endl;
        }
        else{
            cout << "Тест следа не пройден"<<endl;
        }
    }

    }
    //task 2
    if (num_task == 2) {
    int n1, m1, n2, m2;
    file >> n1 >> m1;
    
    vector<vector<double>> matrix1(n1, vector<double>(m1));
    for (int i = 0; i < n1; i++) {
        for (int j = 0; j < m1; j++) {
            file >> matrix1[i][j];
        }
    }
     
    sparce_matrix smatrix1(n, m, matrix);
    sparce_matrix smatrix2(n1, m1, matrix1);
    int n_test; int m_test;
    file >> n_test >> m_test;
    vector<vector<double>> result_matrix(n_test, vector<double>(m_test));
    for (int i = 0; i < n_test; i++) {
        for (int j = 0; j < m_test; j++) {
            file >> result_matrix[i][j];
        }
    }

    int flag_for_add=1;
    sparce_matrix result = matrix_add(smatrix1, smatrix2);
    if (result.get_N() == n_test && result.get_M() == m_test) {
        for (int i = 0; i < n_test; i++) {
            for (int j = 0; j < m_test; j++) {
                
                if (result.get_element(i+1, j+1) == result_matrix[i][j]) {
                    continue;
                }   
                else {
                    flag_for_add=0;
                }
            }

        }
    }

    if (flag_for_add==1){
        cout << "Тест сложения пройден"<<endl;
    }
    else{
        cout << "Тест сложения не пройден"<<endl;
    }
    //test scalar
    int n_for_scalar; int m_for_scalar;
    file >> n_for_scalar >> m_for_scalar;
    double scalar;
    
    vector<vector<double>> input_matrix_scalar(n_for_scalar, vector<double>(m_for_scalar));
    for (int i = 0; i < n_for_scalar; i++) {
        for (int j = 0; j < m_for_scalar; j++) {
            file >> input_matrix_scalar[i][j];
        }
    }
    file >> scalar;
    sparce_matrix smatrix_scalar(n_for_scalar, m_for_scalar, input_matrix_scalar);
    sparce_matrix result_scalar = matrix_scalar_multiply(smatrix_scalar, scalar);

    int n_for_scalar_test; int m_for_scalar_test;
    file >> n_for_scalar_test >> m_for_scalar_test;
    vector<vector<double>> result_matrix_scalar(n_for_scalar_test, vector<double>(m_for_scalar_test));
    for (int i = 0; i < n_for_scalar_test; i++) {
        for (int j = 0; j < m_for_scalar_test; j++) {
            file >> result_matrix_scalar[i][j];
        }
    }

    int flag_for_scalar=1;
    for (int i = 0; i < n_for_scalar; i++) {
        for (int j = 0; j < m_for_scalar; j++) {   
            if (result_scalar.get_element(i+1, j+1) == result_matrix_scalar[i][j]) {
                continue;
            }   
            else {
                flag_for_scalar=0;
            }
        }
    }

    if (flag_for_scalar==1){
        cout << "Тест умножения на скаляр пройден"<<endl;
    }
    else{
        cout << "Тест умножения на скаляр не пройден"<<endl;
    }
    int n_for_multiply1; int m_for_multiply2;
    file >> n_for_multiply1 >> m_for_multiply2;
    vector<vector<double>> input_matrix_multiply1(n_for_multiply1, vector<double>(m_for_multiply2));
    for (int i = 0; i < n_for_multiply1; i++) {
        for (int j = 0; j < m_for_multiply2; j++) {
            file >> input_matrix_multiply1[i][j];
        }
    }
    sparce_matrix smatrix_multiply1(n_for_multiply1, m_for_multiply2, input_matrix_multiply1);
    int n_for_multiply2; int m_for_multiply1;
    file >> n_for_multiply2 >> m_for_multiply1;
    vector<vector<double>> input_matrix_multiply2(n_for_multiply2, vector<double>(m_for_multiply1));
    for (int i = 0; i < n_for_multiply2; i++) {
        for (int j = 0; j < m_for_multiply1; j++) {
            file >> input_matrix_multiply2[i][j];
        }
    }
    sparce_matrix smatrix_multiply2(n_for_multiply2, m_for_multiply1, input_matrix_multiply2);
    sparce_matrix result_multiply = matrix_multiply(smatrix_multiply1, smatrix_multiply2);
    int result_multiply_n; int result_multiply_m;
    file >> result_multiply_n >> result_multiply_m;
    vector<vector<double>> result_matrix_multiply(result_multiply_n, vector<double>(result_multiply_m));
    for (int i = 0; i < result_multiply_n; i++) {
        for (int j = 0; j < result_multiply_m; j++) {
            file >> result_matrix_multiply[i][j];
        }
    }
    int flag_for_multiply=1;
    for (int i = 0; i < result_multiply_n; i++) {
        for (int j = 0; j < result_multiply_m; j++) {   
            if (result_multiply.get_element(i+1, j+1) == result_matrix_multiply[i][j]) {
                continue;
            }   
            else {
                flag_for_multiply=0;
            }
        }   

    }

    if (flag_for_multiply==1){
        cout << "Тест умножения матриц пройден"<<endl;
    }
    else{
        cout << "Тест умножения матриц не пройден"<<endl;
    }
    }
    //task 3
    if (num_task == 3) {
        double expected_determinant;
        file >> expected_determinant;
        double actual_determinant = determinant(matrix, n);
        if (actual_determinant==expected_determinant){
            cout << "Тест определителя пройден"<<endl;
        }
        else{
            cout << "Тест определителя не пройден"<<endl;
        }
        
        int expected_inverse;
        file >> expected_inverse;
        int actual_inverse;
        if (actual_determinant == 0) {
            actual_inverse = 0;
        
        }
        else{
            actual_inverse = 1;
        }
        if (actual_inverse==expected_inverse){
            cout << "Тест на существование обратной матрицы пройден"<<endl;
        }
        else{
            cout << "Тест на существование обратной матрицы не пройден"<<endl;
        }
        
    }
    }
    
    file.close();
}
//проверка ввода
int enter(){
    
    cout << "Введите размер матрицы N и M: ";
    int N, M;
    cin >> N >> M;
    
    vector<vector<double>> input_matrix(N, vector<double>(M));
    cout << "Введите элементы матрицы " << N << "x" << M << ":" << endl;
    for(int i = 0; i < N; i++) {
        for(int j = 0; j < M; j++) {
            cin >> input_matrix[i][j];
        }
    }
    
    sparce_matrix matrix_from_input(N, M, input_matrix);
    
    int row, col;
    
    cout << "Введите индексы элемента матрицы (строка и столбец): ";
    cin >> row >> col;
    // Вывод элемента матрицы
    cout << "Элемент [" << row << "][" << col << "]: " << matrix_from_input.get_element(row, col) << endl;
    // Вычисление следа матрицы
    if (N==M){
        cout << "След матрицы: " << matrix_from_input.trace()[1] << endl;
        cout << "Определитель матрицы: " << determinant(input_matrix,N) << endl;
        cout << "Существует ли обратная матрица? " << (fabs(determinant(input_matrix,N)) !=0 ? "да" : "нет") << endl;

    }
    return 0;
}

int main() {
    //test_from_file("C:\\projects\\test_matrix.txt",1);
    //test_from_file("C:\\projects\\test_multipl.txt",2);
    //test_from_file("C:\\projects\\tests_det.txt",3);
    enter();
    return 0;    

}
