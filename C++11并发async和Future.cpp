#include <iostream>
#include <future>
#include <random>
#include <thread>
#include <chrono>
#include <vector>

using namespace std;

int doSth(char c)
{
	default_random_engine dre(c);
	uniform_int_distribution<int> id(10, 1000);
	for (int i = 0; i < 10; ++i)
	{
		this_thread::sleep_for(chrono::milliseconds(id(dre)));
		cout.put(c).flush();
	}
	return c;
}

int fun1()
{
	return doSth('.');
}

int fun2()
{
	return doSth('+');
}
void fun3()
{
	vector<int> v;
	while (true)
	{
		for (int i = 0; i < 1000000; ++i)
		{
			v.push_back(i);
		}
	}
}
int main()
{
	//future<int> result1(async(fun1)); //这种方式并不保证fun1立马在后台运行
	future<int> result1(async(launch::async, fun1)); //launch保证fun1立马运行
	//future<int> result1(async(fun1)); 
		//result1.wait(); //wait强制启动线程，并等待直到线程完成
		//result1.wait_for(chrono::milliseconds(5)); //不强制启动线程，最多等待5ms
	
	//	
	//while (result1.wait_for(chrono::milliseconds(200)) != future_status::ready) //判断wait返回的线程状态
	//{
		//cout << "wait for fun1 ready..." << endl;
	//}
	int result2 = fun2();
	/*get的三种情况
	1	线程尚未启动，立马启动
	2	线程正在执行，等待返回结果
	3	线程执行完毕，取出结果
	*/
	int res = result1.get() + result2;
	cout << res << endl;
	
	cout << "if thread endless loop" << endl;
	future<void> result3(async(launch::async, fun3)); //fun3会死循环，引发的异常智慧终端当前线程，直到调用get函数，该异常被抛出
	try
	{
		result3.get();
	}
	catch (const exception& e)
	{
		cerr << e.what() << endl;
	}
	
	return 0;
}
