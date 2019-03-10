#ifndef __EXCEPTION_H__
#define __EXCEPTION_H__
#include "Object.h"

/*
父类：
	Exception
子类异常：
	ArithmeticException
	NullPointException
	IndexOutOfBoundsException
	NotEnoughMemoryException
	InvalidParameterException 
	InvalidOperationException 
*/

namespace YzcLib{
//有的C++库不希望出异常，通过注释掉(throw e(m, __FILE__, __LINE__))，即可实现。
#define THROW_EXCEPTION(e, m) (throw e(m, __FILE__, __LINE__))

class Exception:public Object{
private:
	char* message;
	char* location;
	//string是C++标准库里边的东西不是C++原生的东西，所以用char*
	void init(const char* mes, const char* file, unsigned int line);
public:
	Exception(const char* mes);
	Exception(const char* file, unsigned int line);
	Exception(const char* mes, const char* file, unsigned int line);

	Exception(const Exception& excpt);
	
	Exception& operator = (const Exception& excpt);

	//非const对象在只有const成员函数的时候也可以访const\成员函数，为了兼容cosnt对象，采用const成员函数来实现
	virtual char* GetMessage() const;
	char* GetLocation() const;

	//一般父类的析构函数都会定义成虚函数，这样确保程序在调用子类析构函数后，调用父类虚构函数

	//virtual 
	~Exception();// = 0;
};

//算术计算异常
class ArithmeticException : public Exception{
public:
	ArithmeticException() : Exception(NULL){}
	ArithmeticException(const char* mes) : Exception(mes){}
	ArithmeticException(const char* file, int line) : Exception(file, line){}
	ArithmeticException(const char* mes, const char* file, int line) : Exception(mes, file, line){}

	ArithmeticException(const ArithmeticException& excpt) : Exception(excpt){}

	ArithmeticException& operator = (const ArithmeticException& excpt){
		Exception::operator = (excpt);
		return *this;
	}
};

//空指针异常
class NullPointException : public Exception{
public:
	NullPointException() : Exception(NULL){}
	NullPointException(const char* mes) : Exception(mes){}
	NullPointException(const char* file, int line) : Exception(file, line){}
	NullPointException(const char* mes, const char* file, int line) : Exception(mes, file, line){}

	NullPointException(const NullPointException& excpt) : Exception(excpt){}

	NullPointException& operator = (const NullPointException& excpt){
		Exception::operator = (excpt);
		return *this;
	}
};

//越界异常
class IndexOutOfBoundsException: public Exception{
public:
	IndexOutOfBoundsException() : Exception(NULL){}
	IndexOutOfBoundsException(const char* mes) : Exception(mes){}
	IndexOutOfBoundsException(const char* file, int line) : Exception(file, line){}
	IndexOutOfBoundsException(const char* mes, const char* file, int line) : Exception(mes, file, line){}

	IndexOutOfBoundsException(const IndexOutOfBoundsException& excpt) : Exception(excpt){}

	IndexOutOfBoundsException& operator = (const IndexOutOfBoundsException& excpt){
		Exception::operator = (excpt);
		return *this;
	}
};

//内存不足异常
class NotEnoughMemoryException : public Exception{
public:
	NotEnoughMemoryException() : Exception(NULL){}
	NotEnoughMemoryException (const char* mes) : Exception(mes){}
	NotEnoughMemoryException (const char* file, int line) : Exception(file, line){}
	NotEnoughMemoryException (const char* mes, const char* file, int line) : Exception(mes, file, line){}

	NotEnoughMemoryException (const NotEnoughMemoryException & excpt) : Exception(excpt){}

	NotEnoughMemoryException & operator = (const NotEnoughMemoryException & excpt){
		Exception::operator = (excpt);
		return *this;
	}
};

//参数错误异常
class InvalidParameterException : public Exception{
public:
	InvalidParameterException() : Exception(NULL){}
	InvalidParameterException(const char* mes) : Exception(mes){}
	InvalidParameterException(const char* file, int line) : Exception(file, line){}
	InvalidParameterException(const char* mes, const char* file, int line) : Exception(mes, file, line){}

	InvalidParameterException(const NullPointException& excpt) : Exception(excpt){}

	InvalidParameterException& operator = (const InvalidParameterException& excpt){
		Exception::operator = (excpt);
		return *this;
	}
};

//参数错误异常
class InvalidOperationException : public Exception{
public:
	InvalidOperationException() : Exception(NULL){}
	InvalidOperationException(const char* mes) : Exception(mes){}
	InvalidOperationException(const char* file, int line) : Exception(file, line){}
	InvalidOperationException(const char* mes, const char* file, int line) : Exception(mes, file, line){}

	InvalidOperationException(const InvalidOperationException& excpt) : Exception(excpt){}

	InvalidOperationException& operator = (const InvalidOperationException& excpt){
		Exception::operator = (excpt);
		return *this;
	}
};
}

/*
void Test_Exception(){
	try{
		THROW_EXCEPTION(InvalidParameterException, "123");
	}
	catch(const InvalidParameterException& e){
		cout<<"catch(InvalidParameterException& e)"<<endl;
		cout<<e.GetMessage()<<endl;
		cout<<e.GetLocation()<<'a'<<endl;		
	}	
	catch(const NotEnoughmemoryException& e){
		cout<<"catch(NotEnoughmemoryException& e)"<<endl;
		cout<<e.GetMessage()<<endl;
		cout<<e.GetLocation()<<endl;		
	}
	catch(const IndexOutOfBoundsException& e){
		cout<<"catch(IndexOutOfBoundsException& e)"<<endl;
		cout<<e.GetMessage()<<endl;
		cout<<e.GetLocation()<<endl;		
	}
	catch(const NullPointException& e){
		cout<<"catch(const NullPointException& e)"<<endl;
		cout<<e.GetMessage()<<endl;
		cout<<e.GetLocation()<<endl;		
	}	
	catch(const ArithmeticException& e){
		cout<<"catch(const ArithmeticException& e)"<<endl;
		cout<<e.GetMessage()<<endl;
		cout<<e.GetLocation()<<endl;		
	}
	catch(const Exception& e){
		cout<<"catch(const Exception& e)"<<endl;
		cout<<e.GetMessage()<<endl;
		cout<<e.GetLocation()<<endl;

	}
}

*/


#endif