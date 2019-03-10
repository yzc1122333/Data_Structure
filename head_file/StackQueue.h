#ifndef __STACKQUEUE_H__
#define __STACKQUEUE_H__

#include "Queue.h"
#include "LinkStack.h"
/*
用栈实现队列等价于用后进先出的特性实现先进先出的特性
解决方案
两个栈一个负责输入（stack_in），一个负责输出(stack_out)
实现思路
准备两个栈用于实现队列：stack_in 和 stack_out
当有新元素入队时，将其压入stack_in
当需要出队时：
	stack_out.size() = 0:
		将 stack_in 中的元素逐一弹出并压入stack_out
		将 stack_out 的栈顶元素弹出
	stack_out.size() > 0:
		将stack_out的栈顶元素弹出

*/

namespace YzcLib{

template<typename T>
class StackQueue: public Queue<T>{
protected:
	//Pop不是const的，所以要保证LinkStack是mutable的
	mutable LinkStack<T> stack_in;
	mutable LinkStack<T> stack_out;
	//Front 是const的
	void _move() const;
public:
	void Add(const T& e);
	void Remove();
	T Front() const;
	void Clear();
	unsigned int Length() const;
};

template<typename T>
void StackQueue<T>:: _move() const{
	if(stack_out.Size() == 0){
		while(stack_in.Size() > 0){
			stack_out.Push(stack_in.Top());
			stack_in.Pop();
		}
	}
}
template<typename T>
void StackQueue<T>::Add(const T& e){
	stack_in.Push(e);
}

template<typename T>
void StackQueue<T>::Remove(){
	_move();
	if(stack_out.Size() > 0){
		stack_out.Pop();
	}
	else{
		THROW_EXCEPTION(InvalidOperationException , "No element in current queue ... ");
	}
}

template<typename T>
T StackQueue<T>::Front() const{
	_move();
	if(stack_out.Size() > 0){
		return stack_out.Top();
	}
	else{
		THROW_EXCEPTION(InvalidOperationException , "No element in current queue ... ");
	}
}

template<typename T>
void StackQueue<T>::Clear(){
	stack_in.Clear();
	stack_out.Clear();
}

template<typename T>
unsigned int StackQueue<T>::Length() const{
	return stack_in.Size() + stack_out.Size();
}

/*
Test code
	StackQueue<int> queue;
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

	for(int i = 0; i < 10; i++){
		queue.Add(i);
		cout<<queue.Front()<<endl;
	}


	while(queue.Length() > 0){
		cout<<queue.Front()<<endl;
		queue.Remove();
	}


result：
No element in current queue ...
./head_file/StackQueue.h:72
~Object()
No element in current queue ...
./head_file/StackQueue.h:61
~Object()
new():size = 32
1
new():size = 32
1
~Object()
delete:pointor = 0x555c4ef62f10
0
new():size = 32
1
0
new():size = 32
1
0
new():size = 32
1
0
new():size = 32
1
0
new():size = 32
1
0
new():size = 32
1
0
new():size = 32
1
0
new():size = 32
1
0
new():size = 32
1
0
0
~Object()
delete:pointor = 0x555c4ef62f40
new():size = 32
1
~Object()
delete:pointor = 0x555c4ef634d0
new():size = 32
1
~Object()
delete:pointor = 0x555c4ef634a0
new():size = 32
1
~Object()
delete:pointor = 0x555c4ef63470
new():size = 32
1
~Object()
delete:pointor = 0x555c4ef63440
new():size = 32
1
~Object()
delete:pointor = 0x555c4ef63410
new():size = 32
1
~Object()
delete:pointor = 0x555c4ef633e0
new():size = 32
1
~Object()
delete:pointor = 0x555c4ef633b0
new():size = 32
1
~Object()
delete:pointor = 0x555c4ef63380
new():size = 32
1
~Object()
delete:pointor = 0x555c4ef62f10
1
~Object()
delete:pointor = 0x555c4ef63380
2
~Object()
delete:pointor = 0x555c4ef633b0
3
~Object()
delete:pointor = 0x555c4ef633e0
4
~Object()
delete:pointor = 0x555c4ef63410
5
~Object()
delete:pointor = 0x555c4ef63440
6
~Object()
delete:pointor = 0x555c4ef63470
7
~Object()
delete:pointor = 0x555c4ef634a0
8
~Object()
delete:pointor = 0x555c4ef634d0
9
~Object()
delete:pointor = 0x555c4ef62f40
~Object()
~Object()
~Object()
~Object()
~Object()
~Object()
~Object()
*/

}

#endif