#include <iostream> 
#include <omp.h> 
using namespace std;

int main() {
    int N = 3; // размерность матрицы 
    double** A = new double* [N]; // исходная матрица 
    double** B = new double* [N]; // дополненная матрица 
    int i, j, k, tid;

    for (i = 0; i < N; i++) {
        A[i] = new double[N];
        B[i] = new double[N];
        for (j = 0; j < N; j++) {
            cin >> A[i][j] /*= rand() % 10*/;
            B[i][j] = 0;
            if (i == j) B[i][j] = 1;
        }
    }
    double t1 = omp_get_wtime();

    for (k = 0; k < N; k++) {
#pragma omp parallel shared(A, B, k) private(i, j, tid) 
        {
            tid = omp_get_thread_num();
#pragma omp for schedule(static) 
            for (i = 0; i < N; i++) {
                if (i != k) {
                    double coeff = A[i][k] / A[k][k];
                    for (j = 0; j < N; j++) {
                        A[i][j] -= coeff * A[k][j];
                        B[i][j] -= coeff * B[k][j];
                    }
                }
                cout << "num thread is: " << tid<<endl;
            }
        }
    }

#pragma omp parallel shared(A, B, N) private(i, j, tid) 
    {
        tid = omp_get_thread_num();
#pragma omp for schedule(static) 
        for (i = 0; i < N; i++) {
            double div = A[i][i];
            for (j = 0; j < N; j++) {
                A[i][j] /= div;
                B[i][j] /= div;
            }
            cout << "num thread is: " << tid << endl;
        }
    }
    double t2 = omp_get_wtime();

    cout << "Matrix A:\n";
    for (i = 0; i < N; i++) {
        for (j = 0; j < N; j++) {
            cout << A[i][j] << " ";
        }
        cout << endl;
    }

    cout << "Inverse matrix B:\n";
    for (i = 0; i < N; i++) {
        for (j = 0; j < N; j++) {
            cout << B[i][j] << " ";
        }
        cout << endl;
    }
    cout << "Time is " << t2 - t1;
    return 0;
}
