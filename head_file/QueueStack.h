#ifndef __QUEUESTACK_H__
#define __QUEUESTACK_H__

#include "Stack.h"
#include "LinkQueue.h"

/*
用队列先进先出的特性实现栈后进先出的特性
实现思路
	准备两个队列用于实现栈: queue_1[in]和queue_2[out]
		当有新元素入栈时，将其加入队列[in]
		当需要出栈时:
			将队列[in]的n-1个元素出队列，并进入队列[out]中
			将队列[in]中的最后一个元素出队列(出栈)
			交换两个队列的角色:queue_1[out]和queue_2[in]
*/

namespace YzcLib{

template<typename T>
class QueueStack: public Stack<T>{
protected:
	LinkQueue<T> queue_1;
	LinkQueue<T> queue_2;
	LinkQueue<T>* queue_in;
	LinkQueue<T>* queue_out;
	void _move() const;
	void _swap();

public:
	QueueStack();
	void Push(const T& e);
	void Pop();
	T Top() const;
	void Clear();
	unsigned int Size() const;
};

template<typename T>
void QueueStack<T>::_move() const{
	//这样循环不停调用函数，效率略低
	while(queue_in->Length() > 1){
		queue_out->Add(queue_in->Front());
		queue_in->Remove();
	}
}

template<typename T>
void QueueStack<T>::_swap(){
	LinkQueue<T>* temp = queue_in;
	queue_in = queue_out;
	queue_out = temp;
}

template<typename T>
QueueStack<T>::QueueStack(){
	queue_in = &queue_1;
	queue_out = &queue_2;
}

template<typename T>
void QueueStack<T>::Push(const T& e){
	queue_in->Add(e);
}

template<typename T>
void QueueStack<T>::Pop(){
	if(queue_in->Length() > 0){
		_move();
		queue_in->Remove();
		_swap();
	}
	else{
		THROW_EXCEPTION(InvalidOperationException, "No element in current stack ...");
	}
}

template<typename T>
T QueueStack<T>::Top() const{
	if(queue_in->Length() > 0){
		_move();
		return queue_in->Front();
	}
	else{
		THROW_EXCEPTION(InvalidOperationException, "No element in current stack ...");
	}
}

template<typename T>
void QueueStack<T>::Clear(){
	queue_1.Clear();
	queue_2.Clear();
}

template<typename T>
unsigned int QueueStack<T>::Size() const{
	return queue_1.Length() + queue_2.Length();
}


/*
Test code:
	QueueStack<int> qstack;

	try{
		qstack.Top();
	}
	catch(Exception& e){
		cout<<e.GetMessage()<<endl;
		cout<<e.GetLocation()<<endl;
	}

	try{
		qstack.Pop();
	}
	catch(Exception& e){
		cout<<e.GetMessage()<<endl;
		cout<<e.GetLocation()<<endl;
	}
	
	for(int i = 0; i < 10; i++){
		qstack.Push(i);
	}

	try{
		qstack.Push(66);
	}
	catch(Exception& e){
		cout<<e.GetMessage()<<endl;
		cout<<e.GetLocation()<<endl;
	}

	while(qstack.Size() > 0){
		cout<<qstack.Top()<<endl;
		qstack.Pop();
	}
	
*/


}


#endif