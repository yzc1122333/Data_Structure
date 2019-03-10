#ifndef __LINKQUEUE_H__
#define __LINKQUEUE_H__

#include "Exception.h"
#include "Queue.h"
#include "DualCircleList.h"

/*
静态队列与静态栈一样，堆复杂对象处理相对费时，因此推出链式队列
设计要点
	类模板，抽象父类Queue的直接子类
	在内部使用链式结构实现元素的存储
	只在链表的头部和尾部进行操作
	front 为链表头部，
	rear  在链表尾部
使用自定义的双向循环链表实现链式栈
*/
namespace YzcLib{

template<typename T>
class LinkQueue: public Queue<T>{
protected:
	DualCircleList<T> list;
public:
	void Add(const T& e);
	void Remove();
	T Front() const;
	unsigned int Length() const;
	void Clear();
};

template<typename T>
void LinkQueue<T>::Add(const T& e){
	list.Insert(e);
}

template<typename T>
void LinkQueue<T>::Remove(){
	if(list.Length() > 0){
		list.Remove(0);
	}
	else{
		THROW_EXCEPTION(InvalidOperationException , "No element in current queue ... ");
	}
}

template<typename T>
T LinkQueue<T>::Front() const{
	if(list.Length() > 0){
		return list.Get(0);
	}
	else{
		THROW_EXCEPTION(InvalidOperationException , "No element in current queue ... ");
	}
}

template<typename T>
unsigned int LinkQueue<T>::Length() const{
	return list.Length();
}

template<typename T>
void LinkQueue<T>::Clear(){
	dynamic_cast<DualLinkList<T>*>(&list)->Clear();
}

/*
Test code
	LinkQueue<int> queue;
	try{
		cout<<queue.Front()<<endl;
	}
	catch(Exception& e){
		cout<<e.GetMessage()<<endl;
		cout<<e.GetLocation()<<endl;
	}
	try{
		queue.Remove();
	}
	catch(Exception& e){
		cout<<e.GetMessage()<<endl;
		cout<<e.GetLocation()<<endl;
	}
	for(int i = 0; i < 8; i++){
		queue.Add(i);
		cout<<queue.Front()<<endl;
	}

	try{
		queue.Add(9);
	}
	catch(Exception& e){
		cout<<e.GetMessage()<<endl;
		cout<<e.GetLocation()<<endl;
	}

	while(queue.Length() > 0){
		cout<<queue.Front()<<endl;
		queue.Remove();
	}
*/	

}


#endif