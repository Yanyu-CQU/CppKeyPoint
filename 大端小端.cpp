#include <iostream>

using namespace std;

int main()
{
	int num = 0x87654321;
	unsigned char *buf = (unsigned char*)&num;
	for (int i = 0; i < 4; ++i) // low address save low location
	{
		cout << hex << (int)*(buf + i) << "\t";
		cout << (int*)(buf + i) << endl;
	}
	
	return 1;
} 
