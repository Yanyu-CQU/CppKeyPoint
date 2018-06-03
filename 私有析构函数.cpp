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
		cout << "A的构造函数" << endl;
	}	
private:
	~A()
	{
		cout << "A的析构函数" << endl;
	}		
};

int main()
{
	A* a = new A();
	//delete a;//error: delete would call ~A() 
	return 0;
} 
