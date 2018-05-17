#include <iostream>
#include <memory>
using namespace std;

class Base
{
public:
	int val;
	Base(int v) : val(v) {};
	~Base()
	{
		cout << "release Base" << endl;
	}
};

int main()
{
	Base *pBase = new Base(10);
	auto_ptr<Base> autoBase(pBase);
	//autoBase.reset(); //error: delete object, but than read it.
	cout << autoBase->val << endl;
	//Base *pBase1 = autoBase.release(); // cannot auto manage momory
	//cout << pBase1->val << endl;
	//cout << autoBase->val << endl; // error: autoBase has been released
	auto_ptr<Base> newBase;
	newBase.reset(autoBase.release()); // give pointer to new auto_ptr, auto_ptr cannot be shared!!!
	cout << newBase->val << endl;
	
	unique_ptr<Base> uniBase = move(newBase); // only can move
	cout << uniBase->val << endl;
	shared_ptr<Base> shBase = move(uniBase);
	shared_ptr<Base> shBase1 = shBase; //when shared_ptr copy to another, the count of shared add 1, when any shared_ptr release, count sub 1. when last shared_ptr released, it will delete momory
	cout << shBase->val << endl; //can both use momory
	cout << shBase1->val << endl; 
	
	weak_ptr<Base> weBase = shBase;
	cout << "share count: " << weBase.use_count() << endl;
	shared_ptr<Base> shBase2 = weBase.lock(); // weak_ptr cannot use memory
	cout << "share count: " << weBase.use_count() << endl; // share count add 1
	return 0;
}
