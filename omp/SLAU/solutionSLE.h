#pragma once
#include<fstream>
//#include <algorithm> 
#include<iostream>
#include<string>
#include<iomanip>
#include<omp.h>
#include<cstdlib>
#include<ctime>
//при не возможности открыть входной файл посылается исключение типа стринг

//не распараллеливающиеся части выполняются только в потоке 0
//в том числе и загрузка данных
//далее часть данных высылается каждому процессу в соответствии с рангом
// т.е. процесс с рангом m выполняет 


//процесс 0собирает данные,при необходимости меняет строки местами
class solutionSLE
{
public:
	solutionSLE(const char* nameInputFail, const char* nameOutputFail);
	bool solution();
	void outInformationInConsole();
	void outInformationInConsole(int n);
	void outInf();
	void getRandomMatrix(int n);
	void load();
	void saveSolutionToFile(const char* nameOutputFail);
	void saveRandomeMatrixToFile(const char* nameOutputFail);
private:
	int number;
	double** matrix;
	double* matrixValues; 
	const char* nameInputFail;
	const char* nameOutputFail;	 	
	bool swapRow(int);	
};

