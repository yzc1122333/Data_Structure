#ifndef __LIST_H__
#define __LIST_H__

#include "Object.h"
namespace YzcLib{
//线性表接口
template<typename T>
class List : public Object{
protected:
	/*默认的拷贝构造和赋值函数是浅拷贝，导致多个对象共用同一片内存。会导致两个问题
		(1)析构的时候可能导致释放两次同一片区域
		(2)某一个对象修改了内存的内容，其他的对象（共用同一片内存的）也会跟着改变
	拷贝构造还有一个问题关于基类拷贝构造的问题。若我们自己不写拷贝构造函数，那么编译器会默认自动调用基类，然后子类的拷贝构造函数。
	
	但是如果我们给出子类的拷贝构造函数，编译器默认调用的是父类的无参构造，而不再是父类的拷贝构造。这个时候需要手动将父类拷贝构造放到初始化列表中主动调用。

	将拷贝构造和赋值函数设置为protected的，这样就对于子类默认生成的拷贝构造函数无法访问父类的拷贝构造函数，因此无法正常生成拷贝构造函数。

	这种方式主要针对默认生成的浅拷贝
	*/
	List(const List&);
	// List& operator = (const List&);
public:
	//因为禁用了拷贝构造，因此需要手工添加默认构造函数
	List(){}
//线性表的基本操作
	//元素插入
	virtual bool Insert(int i, const T& e) = 0; 
	//尾部插入元素
	virtual bool Insert(const T& e) = 0; 
	//元素删除
	virtual bool Remove(int i) = 0;				
	//获取目标位置元素的值
	virtual bool Set(int i, const T& e) = 0;	
	//设置目标位置元素的值
	virtual bool Get(int i, T& e)const = 0;		
	//获取线性表的长度
	virtual unsigned int Length() const = 0;	
	//清空线性表
	virtual void Clear() = 0;
};


//Text code（测试拷贝和赋值操作是否能够禁止）
/*
DynamicList<int> l(5);
DynamicList<int> l1(5);
//l1 = l;
//DynamicList<int> n = l;
*/
}

#endif