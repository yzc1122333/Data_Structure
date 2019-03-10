#ifndef __STATICARRAY_H__
#define __STATICARRAY_H__

#include "Array.h"

/*
StaticArray类
类模板：
	封装原生数组
	使用模板参数决定数组大小
	实现函数返回数组长度
	实现拷贝构造和赋值操作
*/

namespace YzcLib{
template<typename T,unsigned int N>
//父类带有泛指类型，父类成员需要用this指针或Base::来访问
class StaticArray: public Array<T>{
protected:
	T space[N];
public:
	StaticArray();

	//拷贝构造和赋值
	StaticArray(const StaticArray<T,N>& obj);
	StaticArray<T, N>& operator = (const StaticArray<T,N>& obj);

	//长度标识
	unsigned int Length() const;

};

template<typename T, unsigned int N>
StaticArray<T, N>::StaticArray(){
	this->m_array = space;
}

template<typename T, unsigned int N>
StaticArray<T, N>::StaticArray(const StaticArray<T,N>& obj){
	this->m_array = space;
	for(int i = 0; i < this->Length(); i++){
		space[i] = obj[i];
	}
}

template<typename T, unsigned int N>
StaticArray<T, N>& StaticArray<T, N>::operator = (const StaticArray<T,N>& obj){
	if (this != &obj){
		for(int i = 0; i < this->Length(); i++){
			space[i] = obj[i];
		}
	}
	return *this;
}

template<typename T, unsigned int N>
unsigned int StaticArray<T, N>::Length() const{
	return N;
}

//Test code
/*
	StaticArray<int, 5> sa;
	for (int i = 0; i < sa.Length(); i++){
		sa[i] = i;
	}
	for (int i = 0; i < sa.Length(); i++){
		cout<<sa[i]<<endl;
	}

	cout<<endl;

	StaticArray<int, 5> sa1 = sa;

	for (int i = 0; i < sa1.Length(); i++){
		cout<<sa1[i]<<endl;
	}

	cout<<endl;

	for (int i = 0; i < sa.Length(); i++){
		sa.Set(i, i*i);
	}
	for (int i = 0; i < sa.Length(); i++){
		int a = -1;
		sa.Get(i, a);
		cout<<a<<endl;
	}	

	cout<<endl;

	StaticArray<int, 5> sa2;
	sa2 = sa;

	for (int i = 0; i < sa2.Length(); i++){
		cout<<sa2[i]<<endl;
	}
*/

/*
result
0
1
2
3
4

0
1
2
3
4

0
1
4
9
16

0
1
4
9
16
*/
 
}

#endif