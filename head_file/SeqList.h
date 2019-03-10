#ifndef __SEQLIST_H__
#define __SEQLIST_H__

#include "Exception.h"
#include "List.h"
//Seqlist是一个抽象类，存储空间的位置和大小由子类完成
//实现顺寻存储结构线性表的关键操作（增删改查）
//提供数组操作符，方便快速获取元素

/*
SeqList主要的复杂度在insert和remove操作。
对于insert操作而言在0位置插入时算法复杂度达到最大，O（n）
对于remove操作而言同样在0位置删除时算法复杂度达到最大，O（n）
但是对于线性表而言，执行所消耗的时间很大程度上取决于泛指类型。如果泛指类型是string类型或者更加复杂的类型，每次执行拷贝所花费的时间要大于线性表的操作的时间。（for example string需要用strcpy进行拷贝）
*/

//注意！！！线性表必须要先插入元素，才能使用操作符[]访问元素，否则抛异常。这一点区别于数组，数组即使越界依旧能够访问内存。

namespace YzcLib{

template <typename T>
class SeqList:public List<T>{
protected:
	T* m_array;//顺序存储空间位置
	unsigned int length;//当前线性表长度
public:
	bool Insert(int i, const T& e);	//O(n)
	bool Insert(const T& e);		//O(1)
	bool Remove(int i);				//O(n)
	bool Set(int i, const T& e);	//O(1)
	bool Get(int i, T& e)const;		//O(1)
	unsigned int Length()const;		//O(1)
	void Clear();					//O(1)

	//数组访问方式
	T& operator [](int i);			//O(1)
	T  operator [](int i)const;		//O(1)

	//设置顺序存储空间容量
	virtual unsigned int Capacity()const = 0;
};
/*
顺序表指定位置插入操作
1. 判断输入参数是否合法
2. 将目标位置之后所有元素后移一个位置
3. 将新元素插入目标位置
4. 线性表长度+1

i是[0, n)
*/
template <typename T>
bool SeqList<T>::Insert(int i, const T& e){
	//是否越界
	bool rst = (i>= 0) and (i<= Length());
	//是否超过容量
	rst  = rst&&(length < Capacity());

	if(rst){
		for(int k = length - 1; k>= i; k--){
			m_array[k + 1] = m_array[k];
		}
		m_array[i] = e;
		length++;
	}
	return rst;
}


//在尾部插入元素
template <typename T>
bool SeqList<T>::Insert(const T& e){
	return Insert(this->length, e);
}
/*
删除指定位置元素
1. 判断目标位置是否合法
2. 将目标位置后的所有元素前移一个位置
3. 长度-1
*/
template <typename T>
bool SeqList<T>::Remove(int i){
	bool rst = (i >= 0) && (i <= 0);
	if (rst){
		for(int k = i; i < length; i++){
			m_array[k] = m_array[k + 1];
		}
		length--;
	}
	return rst;
}

template <typename T>
bool SeqList<T>::Set(int i, const T& e){
	bool rst = (i >= 0) && (i <= 0);
	if (rst){
		m_array[i] = e;
	}
	return rst;
}

template <typename T>
bool SeqList<T>::Get(int i, T& e)const{
	bool rst = (i >= 0) && (i <= 0);
	if (rst){
		e = m_array[i];
	}
	return rst;
}

template <typename T>
unsigned int SeqList<T>::Length()const{
	return length;
}

template <typename T>
void SeqList<T>::Clear(){
	length = 0;
}

//operator [] 限定参数，无法通过返回值得到操作是否合法，因此需要在非法时抛出异常。
template <typename T>
T& SeqList<T>::operator [](int i){
	if(i >= 0 && i < length){
		return m_array[i];
	}
	else{
		THROW_EXCEPTION(IndexOutOfBoundsException,"Parameter i is invalid ...");
	}
}

//const成员函数不能返回引用
//复用operator[]
//const_cast强制转换返回类型只能是指针或引用类型
template <typename T>
T SeqList<T>::operator[](int i)const{
	return const_cast<SeqList<T>&>(*this)[i];
}

}
#endif