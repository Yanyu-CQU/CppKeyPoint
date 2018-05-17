//Definition, inhefit of class
#include <iostream>
using namespace std;
class Base{
public:
	virtual int fcn()
	{
		cout << "Base:fcn" << endl;
	};
};

class D1 :public Base{
public:
	int fcn(int)
	{
		cout << "D1:fcn(int)" << endl;
	}
	virtual void f2()
	{
		cout << "D1:f2" << endl;
	};
};

class D2 :public D1{
public:
	int fcn(int)
	{
		cout << "D2:fcn(int)" << endl;
	};
	int fcn()
	{
		cout << "D2:fcn" << endl;
	};
	void f2()
	{
		cout << "D2:f2" << endl;
	};
};

//Function call
int main()
{
	Base bobj; D1 d1obj; D2 d2obj;

	bobj.fcn();
	d1obj.Base::fcn();
	d2obj.fcn();

	Base *bp1 = &bobj, *bp2 = &d1obj, *bp3 = &d2obj;
	bp1->fcn();//base
	bp2->fcn();//base
	bp3->fcn();//D2

	D1 *d1p = &d1obj; D2 *d2p = &d2obj;
	//bp2->f2();//error
	d1p->f2();//D1
	d2p->f2();//D2
	return 0;
}

