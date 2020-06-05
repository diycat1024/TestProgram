#include <iostream>

using namespace std;

class Point                                       
{
public:
	Point(int xVal = 0, int yVal = 0) :x(xVal), y(yVal) { }
	int getX() const { return x; }
	int getY() const { return y; }
	void setX(int xVal) { x = xVal; }
	void setY(int yVal) { y = yVal; }

private:
	int x, y;
};

class U_Ptr
{
    private:
    friend class SmartPtr;
    U_Ptr(Point *ptr):p(ptr),count_(1){}
    ~U_Ptr(){delete p;}

    int count_;
    Point *p;
};


class SmartPtr
{
public:
	SmartPtr(Point *ptr) :rp(new U_Ptr(ptr)) { }    
	
	SmartPtr(const SmartPtr &sp) :rp(sp.rp) { ++rp->count_; }
	  
	SmartPtr& operator=(const SmartPtr& rhs) {    
		++rp->count_;    
		if (--rp->count_ == 0)    
			delete rp;
		rp = rhs.rp;
		return *this;
	}
	
	~SmartPtr() {       
		if (--rp->count_ == 0)   
			delete rp;
		else 
		cout << "还有" << rp->count_ << "个指针指向基础对象" << endl;
	}

    Point& operator * () //重载*操作符  
    {
        return *(rp->p);
    }
    Point* operator -> () //重载->操作符  
    {
        return rp->p;
    }

	
private:
        U_Ptr *rp;  
};
