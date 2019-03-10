#ifndef __OBJECT_H__
#define __OBJECT_H__

//cstddef主要定义了一些重要的类型和宏,size_t,NULL,等等，如果不引用，会有问题
#include <cstddef>

namespace YzcLib{

//C语言void*可以隐式转换成各种类型指针，但是C++是强类型管控语言，不允许这样，因此NULL与C语言定义方式2不兼容，需要重定义NULL，将NULL直接定义为0
//C++某些地方定义了NULL，为了不重复定义，需要加上ifndef

#ifndef NULL
#define NULL 0
#endif

//防止找不到size_t
#ifndef size_t
typedef unsigned long size_t;
#endif

//new申请内存失败，不同的编译器会有不一样的行为，有的编译\器返回一个NULL，有的编译器抛出异常，为了增加代码的可复用\性，确保在各个编译器上代码行为一致，自己去定义\一个顶层父\类实现new，并且通过throw(),禁止其抛出异常
class Object{
public:
	//申请失败返回NULL,不抛出异常
	//32-bit平台：unsigned int <--->size_t
	//64-bit平台：unsigned long <--->size_t
	void* operator new(size_t size)throw();
	void* operator new[](size_t size)throw();

	//对应的delete和delete[]
	void operator delete(void* p);
	void operator delete[](void* p);

	//必要的操作符重载
	/*主要解决自定义类型在LinkList中Find函数里，需要执行==操作。有的编译器会在编译的时候发现自定义类型没有==操作而报错。
	从用户的角度来看，那个类的定义并没有出现错误，所以报错是不合理的。
	因此为了解决这个问题，在Object中重载==，让自定义类类型继承Object，这样即使是自定义类类型没有重载==，编译期间依旧不会报错
	
	注：上述报错是依赖 编译器的，有的编译器只会在用Find函数的时候才会报错，编译期间不报任何错误。以为单链表和自定义类类型从自身定义上没有任何问题
	*/
	bool operator == (const Object& obj);
	bool operator != (const Object& obj);

	//将析构函数设置为虚函数，使其继承的类中均可使用虚函数表
	virtual ~Object() = 0;
};
}

/*Test case:
class Test:public Object{
public:
	int i;
	int j;
};

class Test_Child:public Test{
public:
	int k;
};

void Test_Object(){
	Object* obj1 = new Test();
	Object* obj2 = new Test_Child();

	cout<<"obj1 = "<<obj1<<endl;
	cout<<"obj2 = "<<obj2<<endl;

	delete obj1;
	delete obj2;
}
*/

#endif