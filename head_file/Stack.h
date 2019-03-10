#ifndef __STACK_H__
#define __STACK_H__

#include "Exception.h"

/*
stack:
	栈是一种特殊的线性表
	栈仅能在线性表的一端进行操作
		栈顶(Top):		允许操作的一端
		栈底(Bottom):	不允许操作的一端
	栈的特性：Last in first out
	栈的操作：
		创建栈
		销毁栈
		清空栈
		进栈
		出栈
		获取栈顶元素
		获取栈的大小
	栈的实现：
	Stack为抽象父类。
			StasticStack
	Stack
			LinkStack

	栈的异常检测都是抛异常，所以用户要结合长度检测输入合法性
栈后进先出的特性适用于检测成对出现的符号

**栈非常适合于需要“就近匹配”的场合
*/

namespace YzcLib{

template <typename T>
class Stack: public Object{
public:
	virtual void Push(const T& e) = 0;
	virtual void Pop() = 0;
	virtual T Top() const = 0;
	virtual void Clear() = 0;
	virtual unsigned int Size() const = 0;


};

}
#endif