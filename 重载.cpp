#include <iostream>
using namespace std;

int g = 1;
class A 
{
public:
	int function ()
	{
		cout << "A normal" << endl;
	}

	int function () const
	{
		cout << "A const" << endl;
	}

};

class B : public A
{
public:
	virtual int function()
	{
		cout << "B normal" << endl;
	}
	int function(int i) const
	{
		cout << "B const:" << i << endl;
	}
};
int f (int i)
{
	cout << "int" << endl;
	return i;
}
int main()
{
	int g = 2;
	cout << ::g << endl;//global
	cout << g << endl;//stack
	
	double f(double i);
	cout << ::f(1) << endl;
	cout << f(1) << endl;
	
	const A ca;
	A a;
	a.function();//normal
	ca.function();//const
	
	const B cb;
	B b;
	b.A::function();//A normal
	b.function();//B normal
	b.function(1);//B const
	//cb.function();
	cb.A::function();//A normal
	cb.function(1);//B const
	return 0;
}
double f(double i)
{
	cout << "double" << endl;
	return i;
}
