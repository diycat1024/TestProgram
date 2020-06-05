#include "smart.hpp"

int main()
{
    //定义一个基础对象类指针
	int *i = new int(1);

	//定义三个智能指针类对象，对象都指向基础类对象pa
	//使用花括号控制三个指针指针的生命期，观察计数的变化

	{
		SmartPtr<int> sptr1(i);//此时计数count=1
		{
			SmartPtr<int> sptr2(sptr1); //调用复制构造函数，此时计数为count=2
			{
				SmartPtr<int> sptr3=sptr1; //调用赋值操作符，此时计数为conut=3
                cout << *sptr1 << endl;
				*sptr1 = 20;
				cout << *sptr2 << endl;
			}
			//此时count=2
		}
		//此时count=1；
	}
	//此时count=0；pa对象被delete掉


	system("pause");
	return 0;
}