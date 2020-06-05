#include <iostream>

using namespace std;

template <typename T>
class SmartPtr;


template <typename T>
class U_Ptr
{
    private:
    friend class SmartPtr<T>;
    U_Ptr(T *ptr):p(ptr),count_(1){}
    ~U_Ptr(){delete p;}

    int count_;
    T *p;
};



template <typename T>
class SmartPtr
{
public:
	SmartPtr(T *ptr) :rp(new U_Ptr<T>(ptr)) { }    
	
	SmartPtr(const SmartPtr<T> &sp) :rp(sp.rp) { ++rp->count_; }
	  
	SmartPtr& operator=(const SmartPtr<T>& rhs) {    
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

    T& operator * () //重载*操作符  
    {
        return *(rp->p);
    }
    T* operator -> () //重载->操作符  
    {
        return rp->p;
    }

	
private:
        U_Ptr<T> *rp;  
};
