#include <thread>
#include <iostream>
#include <chrono>
#include <mutex>
#include <vector>
#include <string>
#include <condition_variable>
using namespace std;

void fun1(char& i)
{
	//vector<int> vec;
	static int test = 10;
	int n = 9;
	while (n--)
	{
		cout.put(i).flush();
		this_thread::sleep_for(chrono::milliseconds(10));
	}
	cout << "done" << endl;

}
mutex mut;
void fun2(const string& str)
{
	lock_guard<mutex> l(mut); //
	for (char c : str)
	{
		cout << c;
	}
	cout << "\n";
}

mutex readyMutex;
bool readyFlag = false;
condition_variable readyConVar;
void fun3()
{
	cout << "input a data: ";
	cin.get();
	{
		lock_guard<mutex> lg(readyMutex);
		readyFlag = true;
	}
	readyConVar.notify_one();
}
void fun4()
{
	{
		lock_guard<mutex> lg(readyMutex);
		while (readyFlag != true)
			readyConVar.wait(lg);// []{ return readyFlag;});
	}
	cout << "done!" << endl;
}
int main()
{
	char a = 'y';
	thread t1 = thread(fun1, ref(a)), t2 = thread(fun1, ref(a));
	t1.detach();
	t2.detach();
	this_thread::sleep_for(chrono::milliseconds(1100)); //
	cout << "done2" << endl;
	
	thread t3(fun2, "hello A!");
	thread t4(fun2, "hello B!");
	thread t5(fun2, "hello C!");
	thread t6(fun2, "hello D!");
	t3.join();
	t4.join();
	t5.join();
	t6.join();
	
	thread t7(fun3), t8(fun4);
	t7.join();
	t8.join();
	return 0;
}
