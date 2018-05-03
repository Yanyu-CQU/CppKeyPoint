#include <iostream>
#include <cstring>
#include <cstdlib>
#include <vector>
using namespace std;

class Str{
public:
	char *str;
	Str(char value[])
	{
		cout<<"constructor"<<endl;
		str = NULL;
		int len = strlen(value);
		str = (char *)malloc(len + 1);
		memset(str,0,len + 1);
		strcpy(str,value);
	}
	Str(const Str &s)
	{
		cout<<"copy constructor"<<endl;
		str = NULL;
		int len = strlen(s.str);
		str = (char *)malloc(len + 1);
		memset(str,0,len + 1);
		strcpy(str,s.str);
	}
	Str(Str &&s)
	{
		cout<<"move constructor"<<endl;
		str = NULL;
		str = s.str;//执行浅拷贝，然后断开原对象和内存的连接
		s.str = NULL;
	}
	~Str()
	{
		cout<<"clear"<<endl;
		if(str != NULL)
		{
			free(str);
			str = NULL;
		}
	}
};
int main()
{
	char value[] = "I love zx";
	Str s(value);
	vector<Str> vs;
	vs.push_back(move(s));
	//vs.push_back(s);
	cout<<vs[0].str<<endl;
	if(s.str != NULL)
		cout<<s.str<<endl;
	return 0;
}
