#include <iostream>

using namespace std;

//1, A must be define at heap
//2, A can be static instance in a class
class A
{
public:
	static A a;
	A()
	{
		cout << "A�Ĺ��캯��" << endl;
	}	
private:
	~A()
	{
		cout << "A����������" << endl;
	}		
};

int main()
{
	A* a = new A();
	//delete a;//error: delete would call ~A() 
	return 0;
} 
