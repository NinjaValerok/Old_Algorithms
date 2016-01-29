#include "mpi.h"
#include <mpi.h>
#include<omp.h>
#include<iostream>
#include<fstream>
#include<string>
#include<cstdlib>
#include<ctime>



using namespace std;



void ArrayInit(int *A, int N,int m)
{
	srand(time(0));
	for (int i = 0; i<m; i++)
	for (int j = 0; j<N; j++)
		A[i*N+j] = rand() % 5 + 1;
}
void ArrayShow(int *A, int N,int m)
{
	for (int i = 0; i<m; i++)
	for (int j = 0; j<N; j++)
	{
		if (!(j%N)) cout << endl;
		cout << A[i*N+j] << '\t';
	}
	cout << endl;
}
bool  ArrayFind(int *A, int *B, int *C, int i, int j, int n,int m)//n-кол-во столбцов,m-кол строк
{
	int count(0);
	B[i*n+j] = 1;
	C[i*n+j] = A[i*n+j];
	int exit(1);
	while (exit)
	{
		//вправо
		if (j<(n - 1))
		if (A[i*n+j] == A[i*n+j + 1])
		if (B[i*n + j + 1] != 1)
		{
			count++;
			B[i*n + j + 1] = 1;
			C[i*n + j + 1] = A[i*n + j + 1];
			j++;
			continue;
		}
		//вниз
		if (i<(m - 1))
		if (A[i*n + j] == A[(i + 1 )* n + j])
		if (B[(i + 1 )* n + j] != 1)
		{
			count++;
			B[(i + 1)* n + j] = 1;
			C[(i + 1)* n + j] = A[(i + 1)* n + j];
			i++;
			continue;
		}
		//влево
		if (j>0)
		if (A[i*n+j] == A[i*n+j - 1])
		if (B[i*n + j - 1] != 1)
		{
			count++;
			B[i*n + j - 1] = 1;
			C[i*n + j - 1] = A[i*n + j - 1];
			j--;
			continue;
		}
		//вверх
		if (i>0)
		if (A[i*n + j] == A[(i - 1) * n + j])
		if (B[(i - 1) * n + j] != 1)
		{
			count++;
			B[(i - 1) * n + j] = 1;
			C[(i - 1) * n + j] = A[(i - 1) * n + j];
			i--;
			continue;
		}
		exit = 0;
	}
	
	if (count >0)
		return true;
	else
	{
		C[i*n + j] = 0;
		return false;
	}
		
}


int main(int argc, char* argv[])
{
	int n = 8;
	int *A, *B, *C;
	A = new int [n*n];
	B = new int[n*n];
	C = new int[n*n];	
	int rank, prokSize;
	//double timeStart = omp_get_wtime();

	MPI_Init(&argc, &argv);
	MPI_Comm_rank(MPI_COMM_WORLD, &rank);
	MPI_Comm_size(MPI_COMM_WORLD, &prokSize);

	int local_row = (n ) / prokSize;
	int* local_a = new int[local_row*n];
	int* local_C = new int[local_row*n];
	int rest = (n) % prokSize;//оставшиеся строки,если коли-во проц. и строк не кратн

	if (0 == rank)
	{
		cout << "It's init" << '\n';
		ArrayInit(A, n, n);
		cout << '\n' << '\n';
		ArrayShow(A, n, n);
		cout << '\n' << '\n';
	}

	for (int i = 0; i<n; i++)
	for (int j = 0; j<n; j++)
		C[i*n+j] = 0;
	
	for (int i = 0; i < n; i++)
	{
		for (int j = 0; j<n; j++)
			B[i*n + j] = 0;
	}	

	
	double timeStart = omp_get_wtime();

	MPI_Scatter(A, local_row*n, MPI_INT, local_a, local_row*n, MPI_INT, 0, MPI_COMM_WORLD);//нарезаем по строкам матр А в массив local_a


	for (int i = 0; i < local_row; i++)//
	{
		for (int j = 0; j < n; j++)//
		{
			if (B[i*n + j] == 0)
			{
				ArrayFind(local_a, B, local_C, i, j, n, local_row);
			}
		}		
	}
	
	MPI_Gather(local_C, local_row*n, MPI_INT, C, local_row*n, MPI_INT, 0, MPI_COMM_WORLD);//собирает массивы в цельный массив


	if (rest)
	{
		for (size_t i = n - rest; i < n; i++)
		{
			for (int j = 0; j < n; j++)//
			{
				if (B[i*n + j] == 0)
				{
					ArrayFind(A, B, C, i, j, n, n);
				}
			}
		}
	}
	double timeEnd = omp_get_wtime();
	if (0 == rank)
	{
		cout << '\n' << '\n';
		cout << "It's result" << '\n';
		ArrayShow(C, n, n);
		cout << '\n' << '\n';

		cout << '\n' << "End" << '\n';
		std::cout << '\n' << "It took us  ";
		std::cout << timeEnd - timeStart << "  seconds of time" << '\n';
	}	
	
	

	MPI_Finalize();
	system("pause");
	return 0;
}