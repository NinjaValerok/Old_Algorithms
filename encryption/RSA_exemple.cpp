п»ї#include<iostream>
#include<cmath>
using namespace std;

int Nod(int a, int b)
{
	while (a && b)
	if (a >= b)
		a %= b;
	else
		b %= a;	
	return a | b;
}

const int FermatNumber[6] = { 3,5, 17, 257, 65537, 4294967297 };

int extendedEuclid(long a, long b /*long *x, long *y, long *d*/)

/* calculates a * *x + b * *y = gcd(a, b) = *d */

{
	long q, r, x1, x2, y1, y2, x, y, d;
	x2 = 1;
	x1 = 0;
	y2 = 0;
	y1 = 1;

	while (b > 0) {

		q = a / b, r = a - q * b;

		x = x2 - q * x1;
		y = y2 - q * y1;

		a = b;
		b = r;

		x2 = x1;
		x1 = x;
		y2 = y1;
		y1 = y;
	}

	//d = a, x = x2, y = y2;
	return  y2;
}

int main()
{	
	unsigned int p;
	unsigned int q;
	cout << "enter \"q\" and \"p\"" << '\n';
	cin >> p>>q;

	unsigned  int n;
	n =p * q;
	cout << "n: " << n << '\n';
	unsigned  int euler = (p - 1)*(q - 1);
	cout << "Euler :" << euler << '\n';

	unsigned  int e;//public exponent 
	for (int i = 0; i < 6; i++)
	{
		if (FermatNumber[i] < euler)
		{
			e = FermatNumber[i];
			break;
		}
	}
	cout << "e (public exponent) : " << e << '\n';

	/*double d = 0, d_simple = 0;
	while (d_simple != 1)
	{
		d += 1;
		d_simple = fmod((e*d) , (euler));
	}*/
	double d = extendedEuclid(euler, e);
	cout << "d (secret exponent) : " << d << '\n';


	cout << "public key: ( " << e << ", " << n << ") " << '\n';
	cout << "private key: ( " << d << ", " << n << ") " << '\n';


	for (int i = 0; i < 3; i++)
	{

		cout << "enter messege" << '\n';
		unsigned int m;
		cin >> m;

		double c = fmod(pow(m, e), n);
		cout << "coded messege (c) :" << c << '\n';

		double m2 = fmod(pow(c, d), n);
		cout << "Encoded messege (m) :" << m2 << '\n';
	}

	system("pause");
}





