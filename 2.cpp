//cd C:\Users\Максим\Desktop\уник\прога практика летняя\MPI_individualka\x64\Debug\
//mpiexec -n 8 MPI_individualka.exe
#include <mpi.h>
#include <iostream>
#include <fstream>
#include <vector>
#include <cmath>
using namespace std;

int main(int argc, char** argv)
{
    MPI_Init(&argc, &argv);

    int rank, size;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);  // номер процесса
    MPI_Comm_size(MPI_COMM_WORLD, &size);  // число процессов

    string filenames[] = { "file1.txt", "file2.txt", "file3.txt" };

    for (int i = 0; i < sizeof(filenames) / sizeof(filenames[0]); i++)
    {
        if (i % size == rank)  // если файл соответствует номеру процесса
        {
            ifstream in(filenames[i]); 
            int n = 0;
            double number;
            while (in >> number) 
                n++;
            in.clear();
            in.seekg(0, ios::beg);

            vector<double> data(n);
            int j = 0;
            while (in >> number)
            {
                data[j] = number; 
                j++;
            }

            in.close();

            for (int i = 0; i < data.size(); i++)
            {
                if (data[i] < 0)
                    data[i] = -data[i];
            }
            string new_filename = filenames[i].substr(0, filenames[i].size() - 4) + "_res.txt";
            ofstream out(new_filename);
            for (double number : data)
                out << number << " ";
            out.close();
        }
    }

    MPI_Finalize();
    return 0;
}
