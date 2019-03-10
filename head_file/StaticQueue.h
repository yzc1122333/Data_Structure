#ifndef __STATICQUEUE_H__
#define __STATICQUEUE_H__

#include "Queue.h"
/*
StaticQueue 设计要点
--类模板
	使用原生数组作为队列的存储空间
	使用模板参数决定队列的最大容量
StaticQueue实现要点（循环计数法）：
	--关键操作
		进队列：m_space[m_rear] = e; m_rear = (m_rear + 1)%N;
		出队列：m_front = (m_front + 1)%N
	--队列的状态
		队空：(m_length == 0)&&(m_rear == m_front)
		队满：(m_length == N)&&(m_rear + 1 == m_front)
这种循环计数法可以大大提高队列操作效率

__ 1 __ 2 __ 3 __ 4 __	下一个插入元素的位置 __
 front						rear
 出队						入队

 注意** 这里front就是对头节点，rear是对尾节点的下一个节点，这样设计的好处是恰好保证rear的范围是[0,N],不需要进行特殊处理
*/



namespace YzcLib{

template<typename T, unsigned int N>
class StaticQueue: public Queue<T>{
protected:
	T space[N];
	unsigned int front;
	unsigned int rear;
	unsigned int length;

	#define GETLOCATION(loc) ((loc + 1)%N)

	#define IS_FULL() ((front == rear) && (length == N))

	#define IS_EMPTY() ((front == rear) && (length == 0))
	
public:
	StaticQueue();

	void Add(const T& e);
	void Remove();
	T Front() const;
	void Clear();

	unsigned int Length() const;
	unsigned int Capacity() const;

};

template<typename T, unsigned int N>
StaticQueue<T,N>::StaticQueue(){
	front = rear = 0;
	length = 0;
}

template<typename T, unsigned int N>
void StaticQueue<T,N>::Add(const T& e){
	if(!IS_FULL()){
		// 数组赋值要放在最前边，防止出现异常，导致异常安全性问题
		space[rear] = e;
		rear = GETLOCATION(rear);
		length++;
	}
	else{
		THROW_EXCEPTION(NotEnoughMemoryException, "No space in current queue");
	}
}

template<typename T, unsigned int N>
void StaticQueue<T,N>::Remove(){
	if(!IS_EMPTY()){
		front = GETLOCATION(front);
		length--;
	}
	else{
		THROW_EXCEPTION(NotEnoughMemoryException, "No element in current queue ... ");
	}
}

template<typename T, unsigned int N>
T StaticQueue<T,N>::Front() const{
	if(!IS_EMPTY()){
		return space[front];
	}
	else{
		THROW_EXCEPTION(NotEnoughMemoryException, "No element in current queue ... ");
	}
}

template<typename T, unsigned int N>
void StaticQueue<T,N>::Clear(){
	front = rear = 0;
	length = 0;
}

template<typename T, unsigned int N>
unsigned int StaticQueue<T,N>::Length() const{
	return length;
}

template<typename T, unsigned int N>
unsigned int StaticQueue<T,N>::Capacity() const{
	return N;
}



/*
Test code：
	StaticQueue<int, 8> queue;
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
result ：
No element in current queue ...
./head_file/StaticQueue.
~Object()
No element in current queue ...
./head_file/StaticQueue.h:81
~Object()
0
0
0
0
0
0
0
0
No space in current queue
./head_file/StaticQueue.h:70
~Object()
0
1
2
3
4
5
6
7
~Object()
*/
}

#endif