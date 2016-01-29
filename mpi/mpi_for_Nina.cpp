#include<iostream>
#include<cstdlib>
#include<ctime>
#include"mpi.h"
#include<omp.h>
#include<fstream>
using namespace std;

int* merge(int*a, int n, int localN)
{
	int amountArrays = n / localN;	
	int* resultArray = new int[n];
	int resultArrayIndex = 0;
	int rest = n%localN;//остатак,длина последнего массива position
	if (rest)
	{
		++amountArrays;
	}
	int* arraysPosition = new int[amountArrays];//позиции в массивах,т.е. определяет просмотренные элем
	// int k=arraysPosition[2]
	for (size_t i = 0; i < amountArrays; i++)
	{
		arraysPosition[i] = 0;
	}
	do
	{
		//-------------------------------------------------------------------------------
		//находим первый не слитый элемент и считаем его минимальным
		int min, index=-1;//минимальный элемент(первый из не просмотренных) и индекс массива это мин элемента
		for (size_t i = 0; i < amountArrays-1; i++)//последний масив не смотрим,т.к. он может быть остатковый
		{
			if (arraysPosition[i] < localN)
			{
				index = i;
				min = a[localN*i/*смещение(начало массива)*/ + arraysPosition[i]/*позиция в тек мас*/];
				break;
			}
		}
		if (-1 == index)
		{
			int i = amountArrays - 1;
			if (!rest)//все массивы одинаковые,остаткового нет
			{				
				if (arraysPosition[i] < localN)
				{
					index = i;
					min = a[localN*i/*смещение(начало массива)*/ + arraysPosition[i]/*позиция в тек мас*/];
				}
			}
			else//пытаемся найти в остатковом массиме
			{
				if (arraysPosition[i] < rest)
				{
					index = i;
					min = a[localN*i/*смещение(начало массива)*/ + arraysPosition[i]/*позиция в тек мас*/];
				}
			}
		}
		//-----------------------------------------------------------------------------------


		//------------------------------------------------------------------------
		//поиск наименьшего элемента,его подмассива и увеличение позиции для этого подмассива
		if (-1 != index)
		{
			for (size_t i = index+1; i < amountArrays - 1; i++)//последний масив не смотрим,т.к. он может быть остатковый
			{
				if (arraysPosition[i] < localN )
				{
					if (min > a[localN*i/*смещение(начало массива)*/ + arraysPosition[i]/*позиция в тек мас*/])
					{
						index = i;
						min = a[localN*i/*смещение(начало массива)*/ + arraysPosition[i]/*позиция в тек мас*/];
					}
				}
			}

			//просмотр последнего подмассива,который может быть остатковым
			int i = amountArrays - 1;
			if (!rest)//все массивы одинаковые,остаткового нет
			{
				if (arraysPosition[i] < localN)
				{
					if (min > a[localN*i/*смещение(начало массива)*/ + arraysPosition[i]/*позиция в тек мас*/])
					{
						index = i;
						min = a[localN*i/*смещение(начало массива)*/ + arraysPosition[i]/*позиция в тек мас*/];
					}
				}
			}
			else//пытаемся найти в остатковом массиме
			{
				if (arraysPosition[i] < rest)
				{
					if (min > a[localN*i/*смещение(начало массива)*/ + arraysPosition[i]/*позиция в тек мас*/])
					{
						index = i;
						min = a[localN*i/*смещение(начало массива)*/ + arraysPosition[i]/*позиция в тек мас*/];
					}
				}
			}
			
			//занесение мин элем в рез мас и увеличение индекса подмассива
			resultArray[resultArrayIndex] = min;
			++resultArrayIndex;
			arraysPosition[index] = arraysPosition[index] + 1;
		}
		
		//----------------------------------------------------------------
		
		
		//------------------------------------------------------------------------------------------
		else
		{			
			delete[] arraysPosition;
			return resultArray;;

		}
	} while (true);

}

void sortp(int* a, int n)
{
	bool fl = 0;
	int b;
	do
	{
		fl = 1;
		for (int i = 0; i<n - 1; i++)
		if (a[i]>a[i + 1])//сравниваем соседние
		{
			swap(a[i], a[i + 1]);
			fl = 0;
		}
	} while (!fl);
}

void saveRandomeMatrixToFile(const char* nameOutputFail, int* ar, int n)
{
	std::ofstream out(nameOutputFail);

	for (int i = 0; i < n; i++)
	{
		out << ar[i] << "  ";
	}
	out << '\n';
}
void dataIni(int* ar, int n)
{
	srand(time(NULL));
	for (size_t i = 0; i < n; i++)
	{
		ar[i]=std::rand()%1000;
	}
}
void outInfToConsole(int* ar, int n)
{
	std::cout << '\n';
	for (size_t i = 0; i < n; i++)
	{
		cout << ar[i] << ' ';

	}
}


void shell(int* A, int n)
{
	for (int d = n / 2; d>0; d /= 2) //расстояние
	for (int i = 0; i<n - d; i++)//выбор номера элемента
	for (int j = i; j >= 0 && A[j]>A[j + d]; j--)//работа со вторым индексом
		swap(A[j], A[j + d]);// меняем местами
}


int main(int argc, char* argv[])
{
	int n = 12;	
	int* ar = new int[n];
	int rank, prokSize;//ранк процесса,количество процессов
	
	
	MPI_Init(&argc, &argv);
	MPI_Comm_rank(MPI_COMM_WORLD, &rank);
	MPI_Comm_size(MPI_COMM_WORLD, &prokSize);

	if (0 == rank)
	{
		dataIni(ar, n);
	}

	double timeStart = omp_get_wtime();
	int localN = n / prokSize;//количество элементов подмассивах
	int* localArray = new int[localN];//массив в каждлом процессе
	MPI_Scatter(ar, localN, MPI_INT, localArray, localN, MPI_INT, 0, MPI_COMM_WORLD);
	sortp(localArray, localN);
	//shell(localArray, localN);
	MPI_Gather(localArray, localN, MPI_INT, ar, localN, MPI_INT, 0, MPI_COMM_WORLD);

	if (0 == rank)
	{
		int rest = n%localN;
		if (rest)
		{			
			sortp(ar + prokSize*localN, rest);
		}
		
		int* resAr = merge(ar, n, localN);

		double timeEnd = omp_get_wtime();
		
		//outInfToConsole(ar, n);
		std::cout<<'\n' << "It took us  ";
		std::cout << timeEnd - timeStart << "  seconds of time" << '\n';

		saveRandomeMatrixToFile("myrandomeMatrixe.txt", resAr, n);
		
	}	
	
	MPI_Finalize();
	//system("pause");

}