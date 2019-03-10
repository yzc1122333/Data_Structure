#ifndef __QUEUE_H__
#define __QUEUE_H__

#include "Object.h"

/*
队列是一种特殊的线性表
队列仅能在线性表的两端进行操作
	--对头（Front）：取出数据元素的一端
	--队尾（Rear）：插入数据元素的一端
队列的特性
	--First in first out
队列的操作
	创建队列
	销毁队列
	清空队列
	进队列
	出队列
	获取队头节点
	获取队列长度

*/

namespace YzcLib{

template<typename T>
class Queue: public Object{
public:
	virtual void Add(const T& e) = 0;
	virtual void Remove() = 0;
	virtual T Front() const = 0;
	virtual void Clear() = 0;
	virtual unsigned int Length() const = 0;
};


}


#endif
