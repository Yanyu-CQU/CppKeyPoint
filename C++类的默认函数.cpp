#include <iostream>
using namespace std;
//C++类的6个默认函数
//构造:没有构造（除了拷贝构造）,则隐式生成无参构造；定义了有参构造，不会隐式生成无参构造；如果定义了全缺省构造，就不能定义无参构造，不然不知道调用哪个！ 
//析构：析构没有重载，只有一个！ 
//拷贝构造：参数必须是常引用，如果不是引用，会无限递归调用;如果引用不是const能通过编译，但是不能这么做！ 
//“=”操作符重载
//“&”取地址操作符
//“const &”取常量地址操作符 
class Time
{
public:
	Time(int hour, int minute, int second)
	{
		m_hour = hour;
		m_minute = minute;
		m_second = second;
		cout << "construct Time with 3 para" << endl;
	}
	Time()//无参构造和全缺省构造不能同时存在 
	{
		cout << "construct Time" << endl;
	}
	virtual ~Time() //虚析构的意义在于，什么类型的对象，调用什么类型的析构，防止指针向上转型之后，调用了父类的析构
	{
		cout << "distruct Time" << endl;
	}
	virtual void print()
	{
		cout << m_hour << "-" << m_minute << "-" << m_second << endl;
	}
	Time(const Time& t)//拷贝构造 
	{
		m_hour = t.m_hour;
		m_minute = t.m_minute;
		m_second = t.m_second;
		cout << "copy construct Time" << endl;
	} 
private:
	int m_hour;
	int m_minute;
	int m_second;
};
class TimeSon : public Time
{
private:
	~TimeSon()
	{
		cout << "distruct TimeSon" << endl;
	}
	void print()
	{
		cout << "output from TimeSon print" << endl;	
	}	
};
class Date
{
public:
	Date(int year, int month, int day,Time t)//这里的t由于是值拷贝，因此构造执行完之后，这个t会析构 
		:m_year(year), 
		m_month(month),
		m_day(day),
		c(10),//常量要不在声明处定义值，要不在初始化列表赋值
		test(t),//同常量 
		m_t(t) //拷贝构造 
	{
		/*m_year = year;
		m_month = month;
		m_day = day;*/ 
	    //m_t = t;//如过不使用列表初始化，而是在这里幅值，该对象首先隐式调用无参构造或者全缺省构造，然后调用=赋值操作符 ，这种方式新能差很多！！！
	}
	void print()
	{
		cout << m_year << "-" << m_month << "-" << m_day << endl;
		m_t.print();
		test.print();
		//test2.print();
	}
private:
	int m_year;
	int m_month;
	int m_day;
	Time m_t;
	const int c; 
	Time& test; 
	//Time test2; 
};
int main()
{
	{
		Time t(10,36,20);//这个t析构一次 
		Date d(2016,7,6,t);//这里的t是值拷贝，里面的形参析构一次，然后Date的成员m_t析构一次 
		d.print();
		Time *ts = new TimeSon;
		ts->print();
		delete ts;
		//关于析构函数的调用顺序：首先调用子类析构，然后调用父类析构；构造函数是首先调用父类构造，然后是子类构造（盖房子自基到顶，拆房子自顶到基）
		//如果父类析构没有virtual，子类有，那么delete父类指针只能调用父类析构。
	}
	return 0;
}
