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
		cout << "thread task��1��: " << n << endl;
		this_thread::sleep_for(chrono::milliseconds(10));
	}
		
}
void thread_task2(int& n)
{
	cout << "my id is " << this_thread::get_id() << endl;
	for (int i = 0; i < 5; i++)
	{
		cout << "thread task��2��: " << n << endl;
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
		lock()�����������ʣ�1�������ǰ�������������߳���ס����ǰ�߳�������2�������ǰ������û����������������������3�������ǰ����������ǰ�߳���ס����������
		try_lock()�����Ĳ�ͬ�����ڣ������ǰ�������������߳���ס����������false
		*/
		//try_lock()�޷���֤��ӡ��������ѭ������
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
	thread t0; //ok:Ĭ�Ϲ���	default constructor
	thread t1(thread_task1, n); //ok:�������ĳ�ʼ������	initialization constructor
	thread t2(thread_task2, ref(n)); //ok:�������ĳ�ʼ������	initialization constructor
	thread t3(move(t2)); //ok:�ƶ�����	move constructor
	//thread t3(t1); //err:threadû�п������죡����	thread does't have copy constructor!!!
	//t0 = t3; //err:threadû�п�����ֵ	thread does't have copy operator=!!!
	//t0 = move(t3); //ok:�����ƶ���ֵ����ͨ�����룬���ǻ�ʹ��t3.join()����	it can pass conpiler, but if it works, t3.join() would crash!!!
	t1.join();
	t3.join();
	//t2.join(); //err:t2�Ѿ����ƶ���ֵ��t3��	t2 was moved to t3
	
	thread t[10];
	for (int i = 0; i < 10; i++)
	{
		t[i] = thread(thread_task3, i + 1);//ok:����ĵȺ����ƶ���ֵ	"=" is not copy operator=, it's move operator= (thread& operator= (thread&& rhs) noexcept)
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

