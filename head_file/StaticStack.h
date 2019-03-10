#ifndef __STATICSTACK_H__
#define __STATICSTACK_H__

#include "Stack.h"
#include "Exception.h"

namespace YzcLib{

template <typename T, unsigned int N>
class StaticStack: public Stack<T>{
protected:
	//和类类型分布在一起的数组
	T stackarray[N];
	unsigned int size;
	unsigned int top;
public:
	StaticStack();
	void Push(const T& e);
	void Pop();
	T Top() const;
	unsigned int Size() const;
	unsigned int Capacity() const;
	void Clear();
};

template <typename T, unsigned int N>
StaticStack<T,N>::StaticStack(){
	size = 0;
	top = -1;
}

template <typename T, unsigned int N>
void StaticStack<T,N>::Push(const T& e){
	//前边声明了，所以可以直接用Capacity()
	if(size < Capacity()){
		//-1 + 1 =0,正好对应起始位置
		stackarray[top + 1] = e;
		top++;
		size++;
	}
	else{
		THROW_EXCEPTION(NotEnoughMemoryException,"No space to push element ...");
	}
}

template <typename T, unsigned int N>
void StaticStack<T,N>::Pop(){
	if(size > 0){
		top--;
		size--;
	}
	else{
		THROW_EXCEPTION(InvalidOperationException ,"No element to pop ...");
	}
}

template <typename T, unsigned int N>
T StaticStack<T,N>::Top() const{
	if(size > 0){
		return stackarray[top];

	}
	else{
		THROW_EXCEPTION(InvalidOperationException, "No element in current stack ...");
	}
}

template <typename T, unsigned int N>
unsigned int StaticStack<T,N>::Size() const{
	return size;
}

template <typename T, unsigned int N>
unsigned int StaticStack<T,N>::Capacity() const{
	return N;
}


template <typename T, unsigned int N>
void StaticStack<T,N>::Clear(){
	top = -1;
	size = 0;
}


/*
Test code:
StaticStack<int, 5> sstack;

	try{
		sstack.Top();
	}
	catch(Exception& e){
		cout<<e.GetMessage()<<endl;
		cout<<e.GetLocation()<<endl;
	}

	try{
		sstack.Pop();
	}
	catch(Exception& e){
		cout<<e.GetMessage()<<endl;
		cout<<e.GetLocation()<<endl;
	}
	
	for(int i = 0; i < sstack.Capacity(); i++){
		sstack.Push(i);
	}

	try{
		sstack.Push(66);
	}
	catch(Exception& e){
		cout<<e.GetMessage()<<endl;
		cout<<e.GetLocation()<<endl;
	}

	while(sstack.Size() > 0){
		cout<<sstack.Top()<<endl;
		sstack.Pop();
	}

*/
}


#endif