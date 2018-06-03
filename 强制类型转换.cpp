#include <iostream>
using namespace std;

int main()
{
	const int g = 20;
	//int *h = static_cast<int*>(&g); //error:invalid static_cast from type 'const int*' to type 'int*'
	int *h = (int*)&g; //it's OK 
	*h = 10;
	cout << *h << " " << g << endl;
	return 0;
} 
