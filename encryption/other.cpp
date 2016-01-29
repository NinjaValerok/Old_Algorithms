#include<iostream>
#include<string>
#include<list>
#include <algorithm>
#include <ctime> 
#include<cmath>
using namespace std;

//цезарь
string* encode(string& s, int key)
{
	string* ns = new string;
	for (unsigned int i = 0; i < s.size(); i++)
	{
		char ch = s[i];		
		ch = (ch + key) % 128;
		(*ns) += ch;
	}
	return ns;
}
string* decode(string& s, int key)
{
	return encode(s, (-1)*key+128);
}
//-----------------------------------------------------------

//виженер
string* vigCode(string& s, string& key)
{
	string* ns = new string;
	int keySize = key.size();
	for (unsigned int i = 0; i < s.size(); i++)
	{
		int ch= s[i];
		int keyn = key[i % keySize];
		char nc = ((ch + keyn)%128);
		(*ns) += nc;
	}
	return ns;
}
string* vigDeCode(string& s, string& key)
{
	string newKey;
	for (unsigned int i = 0; i < key.size(); i++)
	{
		char ch = key[i];
		newKey += -1 * ch + 128;
	}
	return vigCode(s,newKey);
}
//---------------------------------------------------------------

//скитал
string* skitCode(string& s, int const numberRows)
{
	int colums = s.size() / numberRows;
	colums++;

	char** text = new char*[numberRows];
	for (int i = 0; i < numberRows; i++)
	{
		text[i] = new char[colums];
	}

	for (int i = 0; i < numberRows; i++)
	{
		for (int j = 0; j < colums; j++)
		{
			int n = i*colums + j;
			if (s.size() <= i*colums + j)
			{
				text[i][j] = ' ';
			}
			else
				text[i][j] = s[i*colums + j];
		}
	}

	for (int i = 0; i < numberRows; i++)
	{
		for (int j = 0; j < colums; j++)
			cout << text[i][j];
		cout << '\n';
	}
	string* st = new string;
	for (int j = 0; j < colums; j++)
	{
		for (int i = 0; i < numberRows; i++)
		{
			(*st) += text[i][j];
		}
	}
	delete text;

	return st;
}
string* skitDeCode(string& s, int const numberRows)
{
	int colums = s.size() / numberRows;
	//colums++;

	char** text = new char*[numberRows];
	for (int i = 0; i < numberRows; i++)
	{
		text[i] = new char[colums];
	}
	int col = -1;
	for (int i = 0; i < s.size(); i++)
	{
		int n = i%numberRows;
		if (!n)
			col++;
		text[n][col] = s[i];
	}
	
	for (int i = 0; i < numberRows; i++)
	{
		for (int j = 0; j < colums; j++)
			cout << text[i][j];
		cout << '\n';
	}

	string* ss = new string;
	for (int i = 0; i < numberRows; i++)
	{
		for (int j = 0; j < colums; j++)
			(*ss)+=text[i][j];		
	}	
	return ss;
	
}
//-------------------------------------------------------------------


//-------------------------------------------------------------------------
//двойной квадрат
bool findSimbolInList(std::list<char>& l,char ch) 
{
	for (std::list<char>::iterator I = l.begin(); I != l.end(); I++)
	{
		if ((*I) == ch)
		{
			return true;
		}
	}
	return false;
}
char** creatMatrix(std::string s) 
{
	//std::transform(s.begin(), s.end(), s.begin(), toupper);
	char** m = new char*[5];
	for (int i = 0; i < 5; i++)
		m[i] = new char[5];
	std::list<char> symbolsList;
	int addedSymbols = 0;//позиция для добавления нового символа
	for (int i = 0; i < s.size();i++)
	{
		char ch = s[i];
		if (' '== s[i])
			continue;
		int row = addedSymbols / 5;
		int col = addedSymbols % 5;		
		if (findSimbolInList(symbolsList,s[i]))
			continue;

		symbolsList.push_back(s[i]);
		m[row][col] = s[i];
		++addedSymbols;
	}
	for (int i = 0; i  <= 25; i++)
	{
		if (addedSymbols > 25)
		{
			cout << "addedSymbols > 25 ,line 170" << '\n';
		}
		char ch = 65 + i;
		if ('Q' == ch)
			continue;

		if (findSimbolInList(symbolsList, ch))
			continue;

		int row = addedSymbols / 5;
		int col = addedSymbols % 5;
		m[row][col] = ch;
		++addedSymbols;
	}


	return m;
}
char** creatBigram(std::string s, int& coutBigram)
{
	//std::transform(s.begin(), s.end(), s.begin(), toupper);
	list<char*> l;
	int i = 0;
	do
	{
		while (' ' == s[i])
		{
			++i;
		}		

		char* c=new char[2];
		c[0] = s[i];
		++i;
		while (i<s.size() && ' ' == s[i] )
		{
			++i;
		}
		if (i >= s.size() || c[0] == s[i])
		{
			c[1] = 'X';
			l.push_back(c);
			continue;
		}
		//можно считать следующий и он не равен предыдущему
		c[1] = s[i];
		l.push_back(c);
		++i;

	} while (i<s.size());
	char** b = new char*[l.size()];
	coutBigram = l.size();
	list<char*>::iterator I= l.begin();
	for (int i = 0; i < l.size(); i++)
	{		
		b[i] = (*I);
		++I;
	}
	return b;
	
}
bool findSimbolInMatrix(char** m, char ch, int& row, int& col)
{	
	for (int i = 0; i < 5; i++)
	{		
		for (int j = 0; j < 5; j++)
		{
			if (m[i][j] == ch)
			{
				row = i;
				col = j;
				return true;
			}
		}
	}
	return false;
}





string* doubleSquareCoded(std::string s, std::string key)
{
	std::transform(s.begin(), s.end(), s.begin(), toupper);
	std::transform(key.begin(), key.end(), key.begin(), toupper);
	char** m = creatMatrix(key);
	cout << '\n' << "Matrix :" << '\n';
	for (int i = 0; i < 5; i++)
	{
		for (int j = 0; j < 5; j++)
		{
			cout << m[i][j] << " | ";
		}
		cout << '\n';
	}
	cout << "-------------------------------------" << '\n';
	int countBigram;
	char** b = creatBigram(s, countBigram);
	cout << '\n' << "Bigrams :" << '\n';
	for (int i = 0; i < countBigram; i++)
	{
		cout<<i<<". " << b[i][0] << " | " << b[i][1] << '\n';
	}
	cout << "-------------------------------------" << '\n';


	char** codedBigrams = new char*[countBigram];
	for (int i = 0; i < countBigram; i++)
		codedBigrams[i] = new char[2];

	for (int i = 0; i < countBigram; i++)
	{
		int r0, r1, c0, c1;//строка и столбец биграмм
		if (!findSimbolInMatrix(m, b[i][0], r0, c0))
		{
			string ss;
			ss += b[i][0];
			throw("dont find simbol : " + ss + " in keyMatrix");
		}
		if (!findSimbolInMatrix(m, b[i][1], r1, c1))
		{
			string ss;
			ss += b[i][1];
			throw("dont find simbol : " + ss + " in keyMatrix");
		}
			
		//проверка правила 2 (в одной строке)
		if (r0 == r1)
		{
			if (c0 + 1 == 5)
			{
				c0 = 0;
				c1 += 1;
			}				
			else
			{
				if (c1 + 1 == 5)
				{
					c1 = 0;
					c0 += 1;
				}
				else
				{
					c0 += 1;
					c1 += 1;
				}
			}

			codedBigrams[i][0] = m[r0][c0];
			codedBigrams[i][1] = m[r1][c1];
			continue;
		}
		
		//-------------------------

		//проверка правила 3(в одном столбце)
		if (c0 == c1)
		{
			if (r0 + 1 == 5)
			{
				r0 = 0;
				r1 += 1;
			}
			else
			{
				if (r1 + 1 == 5)
				{
					r1 = 0;
					r0 += 1;
				}
				else
				{
					r0 += 1;
					r1 += 1;
				}
			}

			codedBigrams[i][0] = m[r0][c0];
			codedBigrams[i][1] = m[r1][c1];
			continue;
		}

		//------------------------------------


		// правимла 4
		codedBigrams[i][0] = m[r0][c1];
		codedBigrams[i][1] = m[r1][c0];
	}
	
	string* ss = new string;

	cout << '\n' << "Coded Bigrams :" << '\n';
	for (int i = 0; i < countBigram; i++)
	{
		(*ss) += codedBigrams[i][0];
		(*ss) += codedBigrams[i][1];
		cout << i << ". " << codedBigrams[i][0] << " | " << codedBigrams[i][1] << '\n';
	}
	cout << "-------------------------------------" << '\n';
	cout << '\n' << "Result string :" << '\n';
	cout << (*ss) << '\n';
	cout << "-------------------------------------" << '\n';
	return ss;
}


string* doubleSquareDeCoded(std::string s, std::string key)
{
	cout << '\n' << "Decoded" << '\n';
	std::transform(s.begin(), s.end(), s.begin(), toupper);
	std::transform(key.begin(), key.end(), key.begin(), toupper);
	char** m = creatMatrix(key);	

	int countBigram;
	char** b = creatBigram(s, countBigram);


	char** codedBigrams = new char*[countBigram];
	for (int i = 0; i < countBigram; i++)
		codedBigrams[i] = new char[2];

	for (int i = 0; i < countBigram; i++)
	{
		int r0, r1, c0, c1;//строка и столбец биграмм
		if (!findSimbolInMatrix(m, b[i][0], r0, c0))
		{
			string ss;
			ss += b[i][0];
			throw("dont find simbol : " + ss + " in keyMatrix");
		}
		if (!findSimbolInMatrix(m, b[i][1], r1, c1))
		{
			string ss;
			ss += b[i][1];
			throw("dont find simbol : " + ss + " in keyMatrix");
		}

		//проверка правила 2 (в одной строке)
		if (r0 == r1)
		{
			if (c0 ==0)
			{
				c0 = 4;
				c1 -= 1;
			}
			else
			{
				if (c1 ==0)
				{
					c1 = 4;
					c0 -= 1;
				}
				else
				{
					c0 -= 1;
					c1 -= 1;
				}
			}

			codedBigrams[i][0] = m[r0][c0];
			codedBigrams[i][1] = m[r1][c1];
			continue;
		}

		//-------------------------

		//проверка правила 3(в одном столбце)
		if (c0 == c1)
		{
			if (r0 ==0)
			{
				r0 = 4;
				r1 -= 1;
			}
			else
			{
				if (0==r1 )
				{
					r1 = 4;
					r0 -= 1;
				}
				else
				{
					r0 -= 1;
					r1 -= 1;
				}
			}

			codedBigrams[i][0] = m[r0][c0];
			codedBigrams[i][1] = m[r1][c1];
			continue;
		}

		//------------------------------------


		// правимла 4
		codedBigrams[i][0] = m[r0][c1];
		codedBigrams[i][1] = m[r1][c0];
	}	

	cout << '\n' << "before replace :" << '\n';
	for (int i = 0; i < countBigram; i++)
	{		
		cout << i << ". " << codedBigrams[i][0] << " | " << codedBigrams[i][1] << '\n';
	}


	for (int i = 0; i < countBigram; i++)
	{
		if ('X' == codedBigrams[i][0])
			codedBigrams[i][0] = ' ';

		if ('X' == codedBigrams[i][1])
			codedBigrams[i][1] = ' ';
	}
	cout << "-------------------------------------" << '\n';
	string* ss = new string;

	cout << '\n' << "De Coded Bigrams :" << '\n';
	for (int i = 0; i < countBigram; i++)
	{
		(*ss) += codedBigrams[i][0];
		(*ss) += codedBigrams[i][1];
		cout << i << ". " << codedBigrams[i][0] << " | " << codedBigrams[i][1] << '\n';
	}
	cout << "-------------------------------------" << '\n';
	cout << '\n' << "Result string :" << '\n';
	cout << (*ss) << '\n';
	cout << "-------------------------------------" << '\n';
	return ss;
}
//------------------------------------------------------------------------


//-----------------------------------------------------------------------
//генерация ключа деф-хел
//class DHkey
//{
//	unsigned int modul;
//	unsigned int generator;
//	unsigned int a;
//	unsigned int b;
//
//public:
//	DHkey(){ srand(time(0)); }
//	DHkey(unsigned int g, unsigned int m)
//	{
//		modul = m;
//		generator = g;
//		srand(time(0));
//		do
//		{
//			a = rand()%100;
//		} while (0==a);
//		
//		do
//		{
//			b = rand()%100;
//		} while (0 == b);
//		cout << "Generate \"a\" :" << a << '\n';
//		cout << "Generate \"b\" :" << b << '\n';
//	}
//	void setGeneratorAndModule(unsigned int g, unsigned int m)
//	{
//		modul = m;
//		generator = g;
//	}
//	unsigned int getKey()
//	{
//		unsigned int A = generateA();
//		cout << "A : " << A << '\n';
//
//		unsigned int B = generateB();
//		cout << "B : " << B << '\n';
//
//		unsigned int k1 = pow(B, a); //% modul;
//		k1 = k1 % modul;
//
//		cout << "key 1 : " << k1 << '\n';
//
//		unsigned int k2 = pow(A, b); //% modul;
//		k2 = k2 % modul;
//
//		cout << "key 2 : " << k2 << '\n';
//
//		if (k1 == k2)
//		{
//			cout << "k1==k1 " << '\n';
//			return k1;
//		}
//		else
//		{
//			cout << "Problem" << '\n';
//				return 0;
//		}
//	}
//
//private:
//	unsigned int generateA()
//	{
//		unsigned int A = pow(generator, a);
//		A = A%modul;
//		return A;
//	}
//	unsigned int generateB()
//	{
//		unsigned int B = pow(generator, b);
//		B = B%modul;
//		return B;
//	}
//
//
//
//};




//----------------------------------------------------------------------

int main()
{
	/*cout << "write text" << '\n';	
	string s="nas_atakuyt";
	int r;
	string* t = skitCode(s, 3);
	cout << "coded: " << '\n';
	cout << (*t) << '\n';

	cout << "deCoded: " << '\n';
	t = skitDeCode((*t), 3);
	cout << (*t) << '\n';*/

	string key = "playfair example";
	string w = "Hide the gold in the tree stump v";
	string*ss = doubleSquareCoded(w, key);
	
	doubleSquareDeCoded((*ss), key);

		

	
	system("pause");
}

