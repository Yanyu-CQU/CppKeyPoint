#include <iostream>

using namespace std;


typedef void (*Fun)(void); 
class Base
{
public:
	virtual void f()
	{
		cout << "Base:f()" << endl;
	}
	virtual void g()
	{
		cout << "Base:g()" << endl;
	}
	virtual void h()
	{
		cout << "Base:h()" << endl;
	}
	void i()
	{
		cout << "Base:i()" << endl;
	}
};
class Der : public Base
{
public:
	void g()
	{
		cout << "Der:g()" << endl;
	}
};

//取普通成员函数地址的方法，联合体中变量共用内存，所以指针d就是函数的地址
/*注意！！这里取出来的是函数在内存中实实在在的地址，存在指针变量中之后，如果试图转为函数指针，MMU会根据段页式内存管理方案，
  把这个在内存中真实的物理地址，当作逻辑地址映射，如果调用这个函数指针，在内存中找不到这个函数！
  代码中用的汇编语言直接call地址，只适用于Visual Studio!!!!!
*/
template<typename dst_type,typename src_type>
dst_type union_cast(src_type src)
{
    union{
        src_type s;
        dst_type d;
    }u;
    u.s = src;
    return u.d;
}
template<typename dst_type,typename src_type>
dst_type pointer_cast(src_type src)
{
    return *static_cast<dst_type*>(static_cast<void*>(&src));
}
int main()
{
	Base base1,base2;
	//两个对象的虚函数表地址不一样，说明同一类的不同对象，各自有自己的虚函数表！
	cout << "the address of virtual list of Base class" << (int*)&base1 << "-" << (int*)&base2 << endl;
	int* temp = ((int*)*(int*)&base2);
	//同一类的不同对象，虽然有各自的虚函数表，但是表里面存的虚函数地址是一样的
	cout << "the address of the 3 functions in virtual list of Base class" << temp << "-" << temp+1 << "-" << temp+2 << endl;
	temp = ((int*)*(int*)&base1);
	cout << "the address of the 3 functions in virtual list of Base class" << temp << "-" << temp+1 << "-" << temp+2 << endl;
	//temp是指针变量，它的值拷贝自虚函数表，虚函数表记录的虚函数地址，其实是逻辑地址，这个地方转为函数指针时，MMU会把逻辑地址转为物理地址
	Fun f_fun = (Fun)*temp;
	cout << "ues the first fun by address: " << endl;
	f_fun();
	cout << "ues the second fun by first address + 1: " << endl;
	f_fun = (Fun)*(temp + 1);
	f_fun();
	cout << "ues the third fun by first address + 2: " << endl;
	f_fun = (Fun)*(temp + 2);
	f_fun();
	
	Der der1;
	cout << "the address of virtual list of Der class" << (int*)&der1 << endl;
	temp = (int*)*(int*)&der1;
	//虽然子类只重写了g(),但是这里三个地址都是不一样的！也就是说，子类把f,h两个虚函数，一模一样的拷贝了，把g重新写了之后，替换了原本g的位置
	cout << "the address of the 3 functions in virtual list of Der class" << temp << "-" << temp+1 << "-" << temp+2 << endl;
	f_fun = (Fun)*temp;
	cout << "ues the first fun of Der by address: " << endl;
	f_fun();
	f_fun = (Fun)*(temp + 1);
	cout << "ues the second fun of Der by address: " << endl;
	f_fun();
	f_fun = (Fun)*(temp + 2);
	cout << "ues the third fun of Der by address: " << endl;
	f_fun();
	if (((int*)*(int*)&der1) != ((int*)*(int*)&base1))
	{
		cout << "although Der doesn't override the f(), but Der copy the f() in code memory" << endl;
	}
	//三个指针一摸一样
	int * pI1 = union_cast<int*>(&Base::i);
	int * pI2 = union_cast<int*>(&Der::i);
	int * pI3 = pointer_cast<int*>(&Base::i);
	//这个代码只能用VS编译才能用
	/*cout << "call i fun by address, it only can be used in Visual Studio!!" << endl;
	__asm
	{
		lea ecx, base1
		mov eax, Base::i
		call eax
	}*/
	if (pI1 != pI2)
	{
		cout << "although Der doesn't override the i(), but Der copy the f() in code memory" << endl;
	}
	else
	{
		cout << "It's not same as virtual function, Der will not copy a usual function'" << endl;
	}
	//小实验：
	/*
	testAddress并不等于22220000，推测是MMU把22220000当做了物理地址，反过来映射成了逻辑地址（数据段的逻辑地址），然后转换为函数指针时，又转换为物理地址，
	但是这里会触发错误，难道是代码段和数据段不在一个内存空间，因此映射回物理地址时，并不等于22220000，而是超出内存空间？有待考证。
	*/
	int* testAddress = (int*)(void*)22220000;
	f_fun = (Fun)*testAddress;
	system("pause");

	return 0;
}  
