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
	//future<int> result1(async(fun1)); //���ַ�ʽ������֤fun1�����ں�̨����
	future<int> result1(async(launch::async, fun1)); //launch��֤fun1��������
	//future<int> result1(async(fun1)); 
		//result1.wait(); //waitǿ�������̣߳����ȴ�ֱ���߳����
		//result1.wait_for(chrono::milliseconds(5)); //��ǿ�������̣߳����ȴ�5ms
	
	//	
	//while (result1.wait_for(chrono::milliseconds(200)) != future_status::ready) //�ж�wait���ص��߳�״̬
	//{
		//cout << "wait for fun1 ready..." << endl;
	//}
	int result2 = fun2();
	/*get���������
	1	�߳���δ��������������
	2	�߳�����ִ�У��ȴ����ؽ��
	3	�߳�ִ����ϣ�ȡ�����
	*/
	int res = result1.get() + result2;
	cout << res << endl;
	
	cout << "if thread endless loop" << endl;
	future<void> result3(async(launch::async, fun3)); //fun3����ѭ�����������쳣�ǻ��ն˵�ǰ�̣߳�ֱ������get���������쳣���׳�
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
