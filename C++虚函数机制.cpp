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
/*注意！！这里取出来的是函数在内存中实实在在的地址（更正：代码段逻辑地址），存在指针变量中之后，如果试图转为函数指针，MMU会根据段页式内存管理方案，
  把这个在地址，当作逻辑地址映射（本来就是代码段地址，再加上段基址，偏移），如果调用这个函数指针，在内存中找不到这个函数！
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
	int* temp = ((int*)*(int*)&base2);//更正：这里是虚函数表指针
	//同一类的不同对象，共享同一个虚函数表
	//更正，这里是虚函数表指针，但是因为是int型指针，因此才每次跳过4字节，并不是存放的虚函数指针
	cout << "the address of virtual list" << temp << endl;
	temp = ((int*)*(int*)&base1);
	cout << "the address of virtual list" << temp << endl;
	//temp是虚函数表地址，不是虚函数地址！
	//虚函数表记录的虚函数地址，其实是逻辑地址（更正：代码段地址在数据段表示），这个地方转为函数指针时，MMU会把逻辑地址转为代码段表示方式
	Fun f_fun = (Fun)*temp;//这里是解引用虚函数表，取出里面存放的虚函数指针
	cout << "ues the first fun by address: " << endl;
	f_fun();
	cout << "ues the second fun by first address + 1: " << endl;
	f_fun = (Fun)*(temp + 1);
	f_fun();
	cout << "ues the third fun by first address + 2: " << endl;
	f_fun = (Fun)*(temp + 2);
	f_fun();
	
	Der der1;
	//Der的虚函数表地址
	temp = (int*)*(int*)&der1;
	f_fun = (Fun)*temp;
	cout << "ues the first fun of Der by address: " << endl;
	f_fun();
	f_fun = (Fun)*(temp + 1);
	cout << "ues the second fun of Der by address: " << endl;
	f_fun();
	f_fun = (Fun)*(temp + 2);
	cout << "ues the third fun of Der by address: " << endl;
	f_fun();
	if (*((int*)*(int*)&der1) != *((int*)*(int*)&base1))
	{
		cout << "although Der doesn't override the f(), but Der copy the f() in code memory" << endl;
	}
	else
	{
		cout << "Der does'n copy f()!!!!" << endl;
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
		cout << "It's same as virtual function, Der will not copy a usual function'" << endl;
	}
	//小实验：
	/*
	testAddress并不等于22220000，推测是MMU把22220000当做了物理地址，反过来映射成了逻辑地址（数据段的逻辑地址），然后转换为函数指针时，又转换为物理地址，
	但是这里会触发错误，难道是代码段和数据段不在一个内存空间，因此映射回物理地址时，并不等于22220000，而是超出内存空间？有待考证。
	*/
	/*
	新的推测。把22220000当作物理地址，映射到数据段，再映射到代码段，操作系统保护实地址
	*/
	/*int* testAddress = (int*)(void*)22220000;
	f_fun = (Fun)*testAddress;
	system("pause");*/ 

	return 0;
}  
