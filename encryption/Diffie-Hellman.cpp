#include<iostream>
#include<string>
#include<list>
#include <algorithm>
#include <ctime> 
#include<cmath>
using namespace std;

class DHkey
{
	unsigned int modul;
	unsigned int generator;
	unsigned int a;
	unsigned int b;

public:
	DHkey(){ srand(time(0)); }
	DHkey(unsigned int g, unsigned int m)
	{
		modul = m;
		generator = g;
		srand(time(0));
		do
		{
			a = rand() % 100;
		} while (0 == a);

		do
		{
			b = rand() % 100;
		} while (0 == b);
		cout << "Generate \"a\" :" << a << '\n';
		cout << "Generate \"b\" :" << b << '\n';
	}
	void setGeneratorAndModule(unsigned int g, unsigned int m)
	{
		modul = m;
		generator = g;
	}
	unsigned int getKey()
	{
		unsigned int A = generateA();
		cout << "A : " << A << '\n';

		unsigned int B = generateB();
		cout << "B : " << B << '\n';

		unsigned int k1 = pow(B, a); //% modul;
		k1 = k1 % modul;

		cout << "key 1 : " << k1 << '\n';

		unsigned int k2 = pow(A, b); //% modul;
		k2 = k2 % modul;

		cout << "key 2 : " << k2 << '\n';

		if (k1 == k2)
		{
			cout << "k1==k1 " << '\n';
			return k1;
		}
		else
		{
			cout << "Problem" << '\n';
			return 0;
		}
	}

private:
	unsigned int generateA()
	{
		unsigned int A = pow(generator, a);
		A = A%modul;
		return A;
	}
	unsigned int generateB()
	{
		unsigned int B = pow(generator, b);
		B = B%modul;
		return B;
	}

	
};

