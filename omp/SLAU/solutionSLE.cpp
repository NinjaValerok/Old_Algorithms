#include "solutionSLE.h"


solutionSLE::solutionSLE(const char* nameInputFail, const char* nameOutputFail)
{
	this->nameInputFail = nameInputFail;
	this->nameOutputFail = nameOutputFail;
}

bool  solutionSLE::solution(){
	
	//вниз
	double timeStart = omp_get_wtime();
	
	for (int i = 0; i < number-1; i++)//итерация означает вычёркивание столбца и строки
	{//итерации только после приёма данных от все процессов
		//----------------------------------------------------------------
		//процесс 0
		//outInf();
		if (0==matrix[i][i])//(1)
		{
			if (!swapRow(i))//вычёркиваем,с тем исклчением,что щаместо 1 будет 0 (2)
				continue;
			//
		}
		//---------------------------------------------------------------------------

		//(3)
		//-----------------------------------------------------------------------		
		{
			matrixValues[i] = matrixValues[i] / matrix[i][i];
			double value = matrix[i][i];

			#pragma omp parallel for 
			for (int j = number - 1; j >= i; j--)//левее i-нули 
			{			
				matrix[i][j] = matrix[i][j] / value; //matrix[i][i];	
				
			}
		}
		//-----------------------------------------------------------------------

		//(4)
		//----------------------------------------------------------------------
		#pragma omp parallel for
		for (int ii = i + 1; ii < number; ii++)
		{ 			
			matrixValues[ii] -= matrixValues[i] * matrix[ii][i];
			// первый элемент соответствующей строки должен изменяться последним
			//этот цикл выполяется последовательно в созданном потоке
			for (int jj = number - 1; jj >= i; jj--)//левее i все нули
			{				
				matrix[ii][jj] -= matrix[ii][i]/*первый элемент соответствующей строки*/ * matrix[i][jj]/*первая строка*/; 				
			}						
		}
		//----------------------------------------------------------------------
	}
	//конец хода вниз
	//----------------------------------------------------------------------


	if (0!=matrix[number-1][number-1])
	{
		matrixValues[number-1] /= matrix[number-1][number-1];
		matrix[number-1][number-1] = 1;
	}		
	//-----------------------------------------------
	//(7)
	for (int i = number - 1; i > 0; i--)
	{
		if (0==matrix[i][i])
		{
			bool critical = false;
			#pragma omp parallel for
			for (int ii = i - 1; ii >= 0; ii--)
			{
				if (matrix[ii][i])//если в столбце есть хотя бы одно отличное от нуля значение
				{
					#pragma omp critical
					critical = true;					
				}					
			}
			if (critical)
				throw("имеются подобные уравнения,данных недостаточно");
			continue;//иначе весь столбец нулевой,его можно пропустить(любое значение данной переменной)
		}	 		
		#pragma omp parallel for
		for (int ii = i - 1; ii >= 0; ii--)
		{
			matrixValues[ii] -= matrixValues[i] * matrix[ii][i];
			matrix[ii][i] = 0;
		}
	}
	double timeEnd = omp_get_wtime();
	std::cout << "It took us  ";
	std::cout << timeEnd - timeStart << "  seconds of time" << '\n';
	
	return true;
}

bool solutionSLE::swapRow(int row)//если все нулевые,то фалсе
{
	int newRow=0;
	for (int i = row; i < number; i++)//ниже row-все нули
	{
		if (i!=row&&matrix[i][row])
		{
			newRow = i;
			break;
		}			
	}
	if (!newRow)//не нашди такой строки
		return false;

	double* m = matrix[row];
	matrix[row] = matrix[newRow];
	matrix[newRow] = m;
	
	double s = matrixValues[row];
	matrixValues[row] = matrixValues[newRow];
	matrixValues[newRow] = s;


	return true;
}


void solutionSLE::load()
{
	std::ifstream in(nameInputFail);
	if (!in)
		throw("Не найдет входной файл");
	in >> number;
	matrix = new double*[number];
	for (int i = 0; i < number; i++)
		matrix[i] = new double[number];

	for (int i = 0; i < number; i++)
	{
		for (int j = 0; j < number; j++)
		{
			in >> matrix[i][j];
			if (!in.good())
				throw("Не достаточное количество элементов во входном файле");
		}
	}

	matrixValues = new double[number];
	for (int i = 0; i < number; i++)
	{
		if (!in.good())
			throw("Не достаточное количество элементов (решений) во входном файле");
		in >> matrixValues[i];		
	}	
}

void solutionSLE::saveRandomeMatrixToFile(const char* nameOutputFail)
{
	std::ofstream out(nameOutputFail);
	out << number << '\n';
	for (int i = 0; i < number; i++)
	{
		for (int j = 0; j < number; j++)
		{
			out << matrix[i][j] << "  ";
		} 
		out << '\n';
	}
	for (int i = 0; i < number; i++)
		out<< matrixValues[i] << '\n';
	out.close();
}

void solutionSLE::outInf()
{
	static int n = 0;
	std::string s = "sol" + std::to_string(n) + ".txt";
	
	std::ofstream out(s);

	for (int i = 0; i < number; i++)
	{
		for (int j = 0; j < number; j++)
		{
			out << matrix[i][j] << "  ";
		}
		out << '\t' << matrixValues[i] << '\n';
	}
	n += 1;
	out.close();
}

void solutionSLE::saveSolutionToFile(const char* nameOutputFail)
{  
	std::ofstream out(nameOutputFail);

	for (int i = 0; i < number; i++)
	{
		for (int j = 0; j < number; j++)
		{
			out << matrix[i][j] << "  ";
		}
		out << '\t' << matrixValues[i] << '\n';
	}	
	out.close();
}

void solutionSLE::outInformationInConsole()
{
	for (int i = 0; i < number; i++)
	{
		for (int j = 0; j < number; j++)
		{
			std::cout <<std::setw(10)<< matrix[i][j] ;
		}
		std::cout << '\t' << '|' << std::setw(10) << matrixValues[i] << '\n';
	}
}
void solutionSLE::outInformationInConsole(int n)
{
	if (n>number)
		n = number;
	for (int i = 0; i < n; i++)
	{
		std::cout << std::setw(10) << i;
		std::cout << '\t' << '|' << std::setw(10) << matrixValues[i] << '\n';
	}
}

void solutionSLE::getRandomMatrix(int n)
{
	//std::srand(time(NULL));
	number = n;
	srand(time(NULL));
	//rand() % 100
	

	if (matrix != nullptr)
		delete [] matrix;

	matrix = new double*[number];
	for (int i = 0; i < number; i++)
		matrix[i] = new double[number];

	matrixValues = new double[number];

	for (int i = 0; i < number; i++)
	{ 
		matrixValues[i] = rand() % 100;
		for (size_t j = 0; j < number; j++)
		{
			matrix[i][j] = std::rand() % 100;			
		}
	}
}