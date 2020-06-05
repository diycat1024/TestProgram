#include "smart.hpp"

int main()
{
    //定义一个基础对象类指针
	Point *pa = new Point(10, 20);

	//定义三个智能指针类对象，对象都指向基础类对象pa
	//使用花括号控制三个指针指针的生命期，观察计数的变化

	{
		SmartPtr sptr1(pa);//此时计数count=1
		{
			SmartPtr sptr2(sptr1); //调用复制构造函数，此时计数为count=2
			{
				SmartPtr sptr3=sptr1; //调用赋值操作符，此时计数为conut=3
			}
			//此时count=2
		}
		//此时count=1；
	}
	//此时count=0；pa对象被delete掉

	cout << pa->getX ()<< endl;

	system("pause");
	return 0;
}