#include <iostream>
#include <utility>
#include <thread>
#include <chrono>
#include <functional>
#include <atomic>
#include <mutex>
using namespace std;

void thread_task1(int n)
{
	cout << "my id is " << this_thread::get_id() << endl;
	for (int i = 0; i < 5; i++)
	{
		cout << "thread task【1】: " << n << endl;
		this_thread::sleep_for(chrono::milliseconds(10));
	}
		
}
void thread_task2(int& n)
{
	cout << "my id is " << this_thread::get_id() << endl;
	for (int i = 0; i < 5; i++)
	{
		cout << "thread task【2】: " << n << endl;
		++n;
		this_thread::sleep_for(chrono::milliseconds(10));
	}
		
}

void thread_task3(int n)
{
	this_thread::sleep_for(chrono::milliseconds(n));
	cout << "id " << this_thread::get_id() << " sleep " << n << " millseconds\n";
}

mutex tex;
void thread_task4(int& n)
{
	for (int i = 0; i < 5; i++)
	{
		/*
		lock()函数三个性质：1、如果当前互斥量被其他线程锁住，当前线程阻塞；2、如果当前互斥量没有上锁，互斥量被上锁；3、如果当前互斥量被当前线程锁住，产生死锁
		try_lock()函数的不同点在于，如果当前互斥量被其他线程锁住，函数返回false
		*/
		//try_lock()无法保证打印次数符合循环次数
		if(tex.try_lock())
		{
			cout << "n: " << n++ << endl;
			this_thread::sleep_for(chrono::milliseconds(1));
			tex.unlock();
		}
	}
}
int main()
{
	cout << "main's id is " << this_thread::get_id() << endl;//the thread id begin with 1 at main function
	int n = 0;
	thread t0; //ok:默认构造	default constructor
	thread t1(thread_task1, n); //ok:带参数的初始化构造	initialization constructor
	thread t2(thread_task2, ref(n)); //ok:带参数的初始化构造	initialization constructor
	thread t3(move(t2)); //ok:移动构造	move constructor
	//thread t3(t1); //err:thread没有拷贝构造！！！	thread does't have copy constructor!!!
	//t0 = t3; //err:thread没有拷贝赋值	thread does't have copy operator=!!!
	//t0 = move(t3); //ok:调用移动赋值，能通过编译，但是会使得t3.join()崩溃	it can pass conpiler, but if it works, t3.join() would crash!!!
	t1.join();
	t3.join();
	//t2.join(); //err:t2已经被移动赋值给t3了	t2 was moved to t3
	
	thread t[10];
	for (int i = 0; i < 10; i++)
	{
		t[i] = thread(thread_task3, i + 1);//ok:这里的等号是移动赋值	"=" is not copy operator=, it's move operator= (thread& operator= (thread&& rhs) noexcept)
	}
	for (int i = 0; i < 10; i++)
	{
		t[i].join();
	}
	
	cout << "****************mutex test***************" << endl;
	thread tt[10];
	n = 0;
	for (int i = 0; i < 10; ++i)
	{
		tt[i] = thread(thread_task4, ref(n));
	}
	for (int i = 0; i < 10; i++)
	{
		tt[i].join();
	}
	return 0;
}

