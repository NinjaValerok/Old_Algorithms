п»ї#include"solutionSLE.h"

int main()
{
	#pragma omp parallel 
	std::cout << "i like parallel"<<'\n';

	solutionSLE s("myRandomMatrix.txt", "out.txt");
	int yn;
	std::cout << "Do you want to generate a random matrix?(1/0)" << '\n';
	std::cin >> yn;

	if (yn)
	{
		int n;
		std::cout << "tell the size" << '\n';
		std::cin >> n;
		s.getRandomMatrix(n);
		s.saveRandomeMatrixToFile("myRandomMatrix.txt");
	}
	else
		s.load();

	
	try
	{
		s.solution();
		//s.outInformationInConsole();
	}
	catch (std::string s)
	{
		std::cout << s;
	} 
	//s.outInformationInConsole(10);
	s.saveSolutionToFile("myRandomMatrixSolution.txt");
	system("pause");		
}