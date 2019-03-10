#ifndef POINTER_H
#define POINTER_H

#include "Object.h"
#include "Exception.h"
#include <cstdlib>

//所有的智能指针
//Pointer抽象父类，SharedPointer and SmartPoint继承自Pointer
/*
智能指针的使用军规
--只能用来指向堆空间中的单个变量（对象），因为智能指针被设计出来就是为了管理堆空间的
--不同类型的智能指针对象不能混合使用
--不要使用delete释放智能指针指向的堆空间
*/


/*
const函数总结
const函数只要求在const成员函数内部不改变成员变量的值。但是可以返回指针或者引用来改变cosnt对象的成员变量，这里const成员函数是不限制的

为了保证const成员函数的值在外部依旧能够保持const特性，可以对返回值加一些限制
对于直接返回数值（T）的：
	可以直接作为const函数，因为返回的值与当前变量仅仅数值相同，分布在不同的内存上，这样在外界改变返回值并不会影响该对象本身的成员。
对于返回值为引用类型的(T&):
	1.改为返回值，而不是返回引用(返回 T 类型)。
	2.将返回值类型加上const修饰(返回const T&)
对于返回值为指针类型的(返回T*)
	1.将返回值加上const修饰(返回const T* 类型)

*/

namespace YzcLib {

template <typename T>
class Pointer : public Object
{
protected:
	T* pointer;
public:
	//给出默认参数，可以当作无参构造来使用
	Pointer(T* p = NULL){
		pointer = p;
	}

	T& operator *(){
		return *pointer;
	}
	T* operator ->(){
		return pointer;
	}
	//指针cosnt有左数右指的规矩，有的时候指针也需要保证所指向对象的内容不被修改
	//确保const对象无法修改所指对象的值，返回值用const
	//如果想修改cosnt修饰的智能指针对象，需要先通过Get函数获得指针地址，然后在外部通过其他指针变量对其修改。这样符合const在C/C++语言中的特性。
	const T& operator *()const{
		return *pointer;
	}
	const T* operator ->() const{
		return pointer;
	}


	bool IsNull() const{
		return pointer == NULL;
	}

	//虽然operator ->()返回值也是T*，但是该操作符必须有后续操作，不能够得到指针的值，因此需要通过Get函数来得到指针变量的值。
	//Get可以是const的，因为const属性的智能指针只保证成员变量（指针变量）的数值不变，不包括指针变量所指的地址的数值。
	//返回的T*变量数值相同，但是分布在不同的内存上
	T* Get() const{
		return pointer;
	}
	//Pointer是抽象类，因此析构函数为纯虚的
	//可能有的编译器可以不需要写析构函数，因为Pointer继承自Object，Object的析构函数是纯虚的，但是亲测需要加上Pointer的析构函数
	//纯虚析构函数需要给出实现
	//virtual ~Pointer() = 0{}标准C++是不合法的，VC++是合法的,需要在函数外部重新声明
	virtual ~Pointer() = 0;
};

template <typename T>
Pointer<T>::~Pointer(){}


//#####################SmartPointer##########################
/*
1.指针生命周期结束时自动释放空间
2.一片空间最多只能由一个指针标识
3.杜绝指针运算和指针比较
4.只适用于保存在堆空间中的类

注：
1.动态申请失败，不会抛出异常，而是返回NULL，所以使用SmartPointer每次需要进行判断返回值是否合法。
*/
//#####################SmartPointer##########################

template <typename T>
class SmartPointer : public Pointer<T>{
public:
  SmartPointer(T* p = NULL):Pointer<T>(p){}

  SmartPointer(const SmartPointer<T>& sp);

  SmartPointer<T>& operator = (const SmartPointer<T>& sp);

  ~SmartPointer();
  
};


template <typename T>
SmartPointer<T>::SmartPointer(const SmartPointer<T>& sp){
  this->pointer = sp.pointer;
  const_cast<SmartPointer<T>&> (sp).pointer = NULL;
}


template <typename T>
SmartPointer<T>& SmartPointer<T>::operator = (const SmartPointer<T>& sp){
  if (this != &sp){
	//为了保障异常安全性将pointer赋值给了p，然后在状态更新后执行delete p。这样可以防止delete pointer抛异常,此时pointer的内容被销毁，但是新的值还没有赋给pointer，导致pointer状态异常
    T* p = this->pointer;
    this->pointer = sp.pointer;
    const_cast<SmartPointer<T>&> (sp).pointer = NULL;
	delete p;
  }
  return *this;
}

template <typename T>
SmartPointer<T>::~SmartPointer(){
  delete this->pointer;
}

/*
Test code:
	class Test{
	public:
	Test(){
		cout<<"Test()"<<endl;
	}
	~Test(){
		cout<<"~Test()"<<endl;
	}
	};

	SmartPointer<Test> sp1 = new Test();
	SmartPointer<Test> sp2;

	cout<<"sp1 = "<<sp1.Get()<<endl;
	cout<<"sp2 = "<<sp2.Get()<<endl;

	sp2 = sp1;

	cout<<"*********************************"<<endl;  
	cout<<"sp1 = "<<sp1.Get()<<endl;
	cout<<"sp2 = "<<sp2.Get()<<endl;

result:
Test()
sp1 = 0xdf5c20
sp2 = 0
*********************************
sp1 = 0
sp2 = 0xdf5c20
~Test()
~Object()
~Object()
*/


//#####################SharedPointer##########################
/*
1.可以实现多个智能指针同时指向同一片内存
2.通过计数机制（ref）标识堆内存
	--堆内存被指向时：ref++
	--指针被置空时：ref--
	--ref == 0时：释放堆内存
注：计数标识分配在堆空间
有两种异常情况
1.输入指针为NULL
2.计数器申请内存失败
两者出现一个，pointer和计数器ref就都为NULL，计数器不计数
*/
//#####################SharedPointer##########################
template< typename T>
class SharedPointer: public Pointer<T>{
protected:
	//ref要分配在堆空间，确保对象销毁了计数还在。多个不同对象的m_ref共同指向ref空间，获得计数。
	unsigned int* m_ref; //计数机制指针成员
	void _assign(const SharedPointer<T>& p);
public:
	SharedPointer(T* p = NULL);

	SharedPointer(const SharedPointer<T>& obj);
	SharedPointer<T>& operator = (const SharedPointer<T>& obj);

	void Clear();	

	// void Display();//测试显示类成员状态用
	// bool operator ==( const SharedPointer<T>& r);
	~SharedPointer();

};

// template< typename T>
// void SharedPointer<T>::Display(){
// 	cout<<endl;
// 	cout<<this->pointer<<endl;
// 	cout<<m_ref<<endl;
// 	if(m_ref){
// 		cout<<*(m_ref)<<endl;
// 	}
// 	cout<<endl;
// }

template< typename T>
void SharedPointer<T>:: _assign(const SharedPointer<T>& obj){
	//如果出现异常，poiner和m_ref赋值为NULL，计数器不计数
	this->pointer = obj.pointer;
	m_ref = obj.m_ref;
	if(m_ref){
		//++优先级高于*，所以要（*m_ref）
		(*m_ref)++;		
	}
}

//智能指针主要针对堆空间，所以当指针为NULL的时候，是异常情况，将m_ref赋值为NULL，智能指针的异常状况可以通过检测m_ref来获取。
template< typename T>
SharedPointer<T>::SharedPointer(T* p): m_ref(NULL){
	//如果指针为NULL，不需要做任何处理
	if(p){
		//不使用new unsigned int，而使用malloc。主要是unsigned int的new不是Object重载的，原生的new可能会在申请失败的时候抛出异常，为了禁止异常的出现，使用malloc来申请内存。
		m_ref = static_cast<unsigned int*>(malloc(sizeof(unsigned int)));
		//检测m_ref是否申请成功
		if(m_ref){
			this->pointer = p;
			*(m_ref) = 1;
		}
		else{
			THROW_EXCEPTION(NotEnoughMemoryException,"No memory to create sharedpointer object...");
		}
	}
}

//Pointer(NULL)必须要写在初始化列表上，因为默认调用的是Pointer无参构造函数，但是Pointer自定义了构造参数，编译器不会再给出无参构造，因此而报警告。
template< typename T>
SharedPointer<T>::SharedPointer(const SharedPointer<T>& obj): Pointer<T>(NULL){
	_assign(obj);
}


template< typename T>
SharedPointer<T>& SharedPointer<T>::operator = (const SharedPointer<T>& obj){
	if(this != &obj){
		Clear();
		_assign(obj);
	}
	return *this;
}

template< typename T>
void SharedPointer<T>::Clear(){
	T* toDel = this->pointer;
	unsigned int* ref = m_ref;
	this->pointer = NULL;
	this->m_ref = NULL;

	if(ref){
		(*ref)--;
		if(*ref == 0){
			free(ref);
			delete toDel;
		}
	}
}

template< typename T>
SharedPointer<T>::~SharedPointer(){
	// cout<<"~SharedPointer()"<<endl;
	// Display();
	Clear();
}
/*
template < class T, class U > 
bool operator==( const shared_ptr<T>& lhs, const shared_ptr<U>& rhs ) noexcept;
stl库函数中share_ptr也是用的全局重载，因为模板为template < class T, class U > ，如果在类内部重载，只有一个函数会用到，但是影响整个模板的泛指类型。
*/
//如果operator==在类内部重载，operator ==并不会和全局重载函数相冲突，另外成员函数在没有重载全局函数的情况下可以将形参隐式转换成类类型，而全局重载函数不能。
// template<typename T>
// bool SharedPointer<T>::operator ==( const SharedPointer<T>& r){
// 	return (this->Get() == r.Get());
// }
//重载operator ==和operator !=，使用全局函数而不在类内部实现。这样的话NULL == pointer和pointer == NULL，因为模板函数是不允许隐式类型转换的。但是类模板成员函数支持隐式类型转换，因此如果将operator ==定义为成员函数，可以执行pointer == NULL
template<typename T>
bool operator ==(const SharedPointer<T>& l, const SharedPointer<T>& r){
	return (l.Get() == r.Get());
}

template<typename T>
bool operator !=(const SharedPointer<T>& l, const SharedPointer<T>& r){
	return (l.Get() != r.Get());
}


/*
Test code:
	class Test{
	public:
	int value;
	Test():value(0){
		cout<<"Test()"<<endl;
	}
	~Test(){
		cout<<"~Test()"<<endl;
	}
	};

	SharedPointer<Test> sp1 = new Test();
	sp1.Display();
	SharedPointer<Test> sp2 = sp1;
	sp2.Display();
	SharedPointer<Test> sp3 = NULL;
	sp3.Display();

	sp3 = sp1;
	sp1.Display();
	sp3.Display();
	sp1->value = 100;
	

	cout<<sp1->value<<endl;
	cout<<sp2->value<<endl;
	cout<<sp3->value<<endl;

	cout<<((sp1 == sp2)&&(sp2 == sp3))<<endl;
	// cout<<(sp1 == NULL)<<endl;
	//cout<<(NULL == sp1)<<endl;
result:
Test()

0x1d34c20
0x1d35050
1


0x1d34c20
0x1d35050
2


0
0


0x1d34c20
0x1d35050
3


0x1d34c20
0x1d35050
3

100
100
100
1
~SharedPointer()

0x1d34c20
0x1d35050
3

~Object()
~SharedPointer()

0x1d34c20
0x1d35050
2

~Object()
~SharedPointer()

0x1d34c20
0x1d35050
1

~Test()
~Object()

*/

}

#endif 